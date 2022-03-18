#include "app.h"

#include <iostream>
#include <complex.h>

using std::make_unique;

App::App(int argc, char *argv[]) {
    /* SDR stuff */
    sdr = make_unique<Device>(0);
    init_device();
    readingAsync = false;

    /* Window stuff */
    app = make_unique<QApplication>(argc, argv);
    window = make_unique<Window>();

    connect_signals();
    populate_interface();

    window->show();
}

App::~App() {
    sdr->stop_async(); // stops async reading if any is running
}

void App::init_device() {
    // Set sane defaults
    if(sdr->center_freq(99.8e6)) // SR P4 Östergötland
        log << "couldn't set center frequency...";

    if(sdr->sample_rate(1.024e6))
        log << "couldn't set sample rate...";

    if(sdr->tuner_bandwidth(0))
       log << "couldn't set tuner bandwidth...";

    if(sdr->freq_corr(60))
       log << "couldn't set frequency correction...";

    sdr->samples_per_read(pow(2, 16));
}

void App::connect_signals() {
    /* Connects to the SDR device object's 'new_samples' QT signal. */
    QObject::connect(sdr.get(), &Device::new_samples, this, &App::refresh_graph);

    keyboard_signals();
    interface_signals();
}

void App::keyboard_signals() {
    /* Keyboard shortcuts */
    QObject::connect(window.get(), &Window::start_async, this, &App::toggle_async_read);
    QObject::connect(window.get(), &Window::fetch_once, this, [this](){
            fetch_samples_once();
        });
    QObject::connect(window.get(), &Window::increase_center_freq, this, [this](){
            window->center_freq->setValue(window->center_freq->value() +
                                          window->center_freq->singleStep());
            update(SDR::Settings::centerFreq,
                   window->center_freq->value(),
                   sdr->center_freq());
        });
    QObject::connect(window.get(), &Window::decrease_center_freq, this, [this](){
            window->center_freq->setValue(window->center_freq->value() -
                                          window->center_freq->singleStep());
            update(SDR::Settings::centerFreq,
                   window->center_freq->value(),
                   sdr->center_freq());
        });
}

void App::interface_signals() {
    /* Refresh/run buttons */
    QObject::connect(window->refresh, &QPushButton::pressed, this, [this](){
            fetch_samples_once();
        });
    QObject::connect(window->run, &QPushButton::pressed, this, &App::toggle_async_read);

    /* Settings changed signals */
    QObject::connect(window->center_freq, &QSpinBox::editingFinished, this, [this](){
            update(SDR::Settings::centerFreq,
                   window->center_freq->value(),
                   sdr->center_freq());
        });
    QObject::connect(window->sample_rate, &QSpinBox::editingFinished, this, [this](){
            update(SDR::Settings::sampleRate,
                   window->sample_rate->value(),
                   sdr->sample_rate());
        });
    QObject::connect(window->freq_corr, &QSpinBox::editingFinished, this, [this](){
            update(SDR::Settings::freqCorr,
                   window->freq_corr->value(),
                   sdr->freq_corr());
        });
    QObject::connect(window->samples_per_read, &QSpinBox::editingFinished, this, [this](){
            update(SDR::Settings::samplesPerRead,
                   window->samples_per_read->value(),
                   0);
        });
}

void App::populate_interface() {
    window->center_freq->setValue(sdr->center_freq());
    window->sample_rate->setValue(sdr->sample_rate());
    window->freq_corr->setValue(sdr->freq_corr());
    window->samples_per_read->setValue(sdr->samples_per_read());
}

int App::start() {
    return app->exec();
}

void App::toggle_async_read() {
    if(!readingAsync) {
        readingAsync = true;
        window->run->setText("Stop");
        window->refresh->setDisabled(true);
        sdr->read_samples_async();
    } else {
        readingAsync = false;
        window->run->setText("Run");
        window->refresh->setDisabled(false);
        sdr->stop_async();
    }
}

void App::refresh_graph() {
    int N = sdr->samples.front().size();

    vector<complex<double>> samps = sdr->samples.front();
    sdr->samples.erase(sdr->samples.begin());

    double xData[N];
    double yDataTime[N];
    double yDataFreq[N];

    for (int i = 0; i < (int)N; i++) {
        xData[i] = i;
        yDataTime[i] = samps[i].real();
    }

    // Setup x range for freq domain plot
    int Fs = sdr->sample_rate();
    vector<double> xDataFreq;
    for (int i = Fs/-2; i <= Fs/2; i += Fs/N) {
        xDataFreq.push_back(i);
    }
    double* xDataFreqPoint = &xDataFreq[0];

    // Perform FFT on the samples to get the frequency domain representation.
    complex<double> out[N];
    fft(samps, out, N);

    for (int i = 0; i < N; i++) {
        yDataFreq[i] = abs(out[i]);
    }

    // Rotate the values.
    double yDataFreqShifted[N];
    for (int i = 0; i <= N/2; i++) {
        int src = i;
        int dst = i + N/2;
        yDataFreqShifted[dst] = yDataFreq[src];
    }

    for (int i = (N/2) + 1; i < N; i++) {
        int src = i;
        int dst = i - N/2;
        yDataFreqShifted[dst] = yDataFreq[src];
    }

    // Display the graphs
    window->timeCurve->setSamples(xData, yDataTime, N);
    window->freqCurve->setSamples(xDataFreqPoint, yDataFreqShifted, N);

    window->timeDomain->replot();
    window->freqDomain->replot();

    // We need to initalize the zoom base when we have real values in the plot
    // for correct behaviour
    if (window->unitializedZoom) {
        window->timeZoomer->setZoomBase(true);
        window->unitializedZoom = false;
    }
}

void App::fft(vector<complex<double>>& data, complex<double> *out, int size) {
    int N = data.size();
    fftw_complex *in, *outTemp;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(&data[0]);
    outTemp = reinterpret_cast<fftw_complex*>(out);

    p = fftw_plan_dft_1d(N, in, outTemp, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    memcpy(out, outTemp, size);

    fftw_destroy_plan(p);
}

void App::update(SDR::Settings setting, int value, int oldValue) {
    switch (setting) {
        case SDR::Settings::centerFreq:
            if(sdr->center_freq(value)) {
                window->center_freq->setValue(oldValue);
                log << "Couldn't set center frequency.";
            }
            break;
        case SDR::Settings::freqCorr:
            if(sdr->freq_corr(value)) {
                window->freq_corr->setValue(oldValue);
                log << "Couldn't set frequency correction.";
            }
            break;
        case SDR::Settings::sampleRate:
            if(sdr->sample_rate(value)) {
                window->sample_rate->setValue(oldValue);
                log << "Couldn't set sample rate.";
            }
            break;
        case SDR::Settings::samplesPerRead:
            sdr->samples_per_read(value);
            break;
        default:
            break;
    }
}

void App::fetch_samples_once() {
    if (!readingAsync) {
        if(sdr->read_samples_sync())
            log << "Couldn't read the requested amount.";
        else
            refresh_graph();
    }
}
