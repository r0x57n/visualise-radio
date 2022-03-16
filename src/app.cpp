#include "app.h"

#include <iostream>
#include <complex.h>

using std::make_unique;

App::App(int argc, char *argv[]) {
    sdr = make_unique<Device>(0);
    app = make_unique<QApplication>(argc, argv);
    window = make_unique<Window>();

    sdr->samples_per_read(pow(2, 16));

    connect_signals();
    populate_window_device();

    window->show();
}

App::~App() {
    sdr->stop_async(); // stops async reading if any is running
}

void App::connect_signals() {
    /* Connects to the SDR device object's 'new_samples' QT signal. */
    QObject::connect(sdr.get(), &Device::new_samples, this, &App::refresh_graph);

    /* Refresh/run buttons */
    QObject::connect(window->refresh, &QPushButton::pressed, this, [this](){
            sdr->read_samples_sync();
            refresh_graph();
        });
    QObject::connect(window->run, &QPushButton::pressed, this, &App::toggle_async_read);

    /* Settings changed signals */
    QObject::connect(window->freqInput, &QLineEdit::editingFinished, this, [this](){
            update(SDR::Settings::centerFreq, window->freqInput->text().toInt());
        });
    QObject::connect(window->fsInput, &QLineEdit::editingFinished, this, [this](){
            update(SDR::Settings::fs, window->fsInput->text().toInt());
        });
    QObject::connect(window->freqCorrInput, &QLineEdit::editingFinished, this, [this](){
            update(SDR::Settings::freqCorr, window->freqCorrInput->text().toInt());
        });
    QObject::connect(window->sprInput, &QLineEdit::editingFinished, this, [this](){
            update(SDR::Settings::spr, window->sprInput->text().toInt());
        });
}

void App::populate_window_device() {
    window->freqInput->setText(QString::number(sdr->center_freq()));
    window->fsInput->setText(QString::number(sdr->sample_rate()));
    window->freqCorrInput->setText(QString::number(sdr->freq_corr()));
    window->sprInput->setText(QString::number(sdr->samples_per_read()));
}

int App::start() {
    return app->exec();
}

void App::toggle_async_read() {
    bool running = true;

    if(window->run->text() == "Run") {
        window->run->setText("Stop");
        window->refresh->setDisabled(true);
        sdr->read_samples_async();
    } else {
        running = false;
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

void App::update(SDR::Settings setting, int value) {
    switch (setting) {
        case SDR::Settings::centerFreq:
            sdr->center_freq(value);
            break;
        case SDR::Settings::freqCorr:
            sdr->freq_corr(value);
            break;
        case SDR::Settings::fs:
            sdr->sample_rate(value);
            break;
        case SDR::Settings::spr:
            sdr->samples_per_read(value);
            break;
        default:
            break;
    }
}
