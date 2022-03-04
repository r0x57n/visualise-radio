#include "app.h"

#include <QApplication>

App::App(int argc, char *argv[]) {
    dev = new Device(0); // open the rtl-sdr device
    app = new QApplication(argc, argv);
    window = new Window; // create the main window

    samplesPerRead = pow(2, 14);

    QObject::connect(window->refresh, &QPushButton::pressed, this, &App::refreshData);

    window->show();
}

App::~App() { }

int App::start() {
    return app->exec();
}

void App::fft(vector<complex<double>>& data, fftw_complex* out) {
    int N = data.size();
    fftw_complex* in;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(&data[0]);

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    fftw_destroy_plan(p);
}

void App::refreshData() {
    vector<complex<double>> samps = dev->read_samples_sync(samplesPerRead);
    int N = samps.size();

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