#include <fftw3.h>
#include <rtl-sdr.h>
#include <QApplication>
#include <iostream>
#include <complex>
#include <cmath>
#include <vector>
#include "device.h"
#include "window.h"

using std::vector;
using std::complex;

void fft(vector<complex<double>>& data, fftw_complex* out) {
    int N = data.size();
    fftw_complex* in;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(&data[0]);

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    fftw_destroy_plan(p);
}

int main(int argc, char *argv[]) {
    int N = pow(2, 14);

    Device dev(0);
    vector<complex<double>> samps = dev.read_samples_sync(N);

    QApplication app (argc, argv);

    Window* window = new Window;

    double xData[samps.size()];
    double yDataTime[samps.size()];
    double yDataFreq[samps.size()];

    for (int i = 0; i < (int)samps.size(); i++) {
        xData[i] = i;
        yDataTime[i] = samps[i].real();
    }

    fftw_complex out[N];
    fft(samps, out);

    for (int i = 0; i < N; i++) {
        yDataFreq[i] = fabs(out[i][0]);
    }

    window->timeCurve->setSamples(xData, yDataTime, samps.size());
    window->freqCurve->setSamples(xData, yDataFreq, samps.size());
    window->show();

    return app.exec();
}
