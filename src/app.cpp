#include "app.h"

#include <iostream>

using std::make_unique;

App::App(int argc, char *argv[]) {
    sdr = make_unique<Device>(0);
    app = make_unique<QApplication>(argc, argv);
    window = make_unique<Window>();

    sdr->samples_per_read(pow(2, 16));

    QObject::connect(window->refresh, &QPushButton::pressed, this, [this](){
            sdr->read_samples_sync();
            refresh_graph();
        });
    QObject::connect(window->run, &QPushButton::pressed, this, &App::toggle_async_read);

    /* Connects to the SDR device object's 'new_samples' QT signal. */
    QObject::connect(sdr.get(), &Device::new_samples, this, &App::refresh_graph);

    window->show();
}

App::~App() { }

int App::start() {
    return app->exec();
}

void App::fft(vector<complex<double>>& data, fftw_complex* out) {
    int N = data.size();
    fftw_complex *in;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(&data[0]);

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    fftw_destroy_plan(p);
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

    fftw_complex out[N];
    fft(samps, out);

    for (int i = 0; i < N; i++) {
        yDataFreq[i] = fabs(out[i][0]);
    }

    window->timeCurve->setSamples(xData, yDataTime, N);
    window->freqCurve->setSamples(xData, yDataFreq, N);

    window->timeDomain->replot();
    window->freqDomain->replot();
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
