#include "app.h"

#include <iostream>
#include <complex.h>
#include <math.h>

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
    // if(sdr->center_freq(99.8e6)) // SR P4 Östergötland
    //     log << "couldn't set center frequency...";
    if(sdr->center_freq(101.1e6)) // SR P4 Östergötland
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
    int N = sdr->samples_per_read() / 2; // IQ samples are complex

    complex<double>* samples = sdr->samples.front();
    hamming_window(samples, N);

    double xData[N];
    double timePlotData[N];
    for (int i = 0; i < (int)N; i++) {
        xData[i] = i;
        timePlotData[i] = samples[i].real();
    }

    // Setup x range for frequency domain plot
    int sampleRate = sdr->sample_rate();
    int centerFreq = sdr->center_freq();
    vector<double> xDataFreq;
    for (int i = sampleRate/-2; i <= sampleRate/2; i += sampleRate/N) {
        xDataFreq.push_back(centerFreq + i);
    }

    // Performs the FFT
    complex<double> freqData[N];
    fft(samples, freqData, N);

    rotate(freqData, N);

    double freqPlotData[N];
    for (int i = 0; i < N; i++) {
        freqPlotData[i] = abs(freqData[i]);
    }

    double* xDataFreqPoint = &xDataFreq[0];

    // Display the graphs
    window->timeCurve->setSamples(xData, timePlotData, N);
    window->freqCurve->setSamples(xDataFreqPoint, freqPlotData, N);

    window->timeDomain->replot();
    window->freqDomain->replot();

    // We need to initalize the zoom base when we have real values in the plot
    // for correct behaviour
    if (window->unitializedZoom) {
        window->timeZoomer->setZoomBase(true);
        window->freqZoomer->setZoomBase(true);
        window->unitializedZoom = false;
    }

    free(samples);
    sdr->samples.erase(sdr->samples.begin());
}

void App::hamming_window(complex<double> *samples, int N) {
    // see https://pysdr.org/content/frequency_domain.html#windowing
    // also https://en.wikipedia.org/wiki/Window_function#Hann_and_Hamming_windows

    // Apply a hamming window to the samples
    double pi = std::acos(-1);
    //double a0 = 25.0/46.0;
    double a0 = 0.54;

    for (int i = 0; i < N; i++) {
        double multiplier = a0 * (1 - std::cos(2*pi*i/N));
        samples[i] *= multiplier;
    }
}

void App::rotate(complex<double> *freqData, int N) {
    // see https://pysdr.org/content/frequency_domain.html#fft-in-python

    complex<double> temp[N];
    memcpy(temp, freqData, N*sizeof(complex<double>));

    // rotate right segment
    memcpy(freqData, &freqData[N/2], (N/2)*sizeof(complex<double>));

    // rotate left segment
    memcpy(&freqData[N/2], temp, (N/2)*sizeof(complex<double>));
}

void App::fft(complex<double> *iqSamples, complex<double> *out, int N) {
    fftw_complex *in, *freqData;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(iqSamples);
    freqData = reinterpret_cast<fftw_complex*>(out);

    p = fftw_plan_dft_1d(N, in, freqData, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    memcpy(out, freqData, N*sizeof(complex<double>));

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
