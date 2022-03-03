#include <iostream>
#include <fftw3.h>
#include <complex>
#include <cmath>
#include <vector>
#include <rtl-sdr.h>
#include "device.h"
#include "graph.h"

void fft(std::vector<std::complex<double>>& data, fftw_complex* out) {
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
    std::vector<std::complex<double>> samps = dev.readSamples(N);

    std::vector<double> data;

    for (int i = 0; i < (int)samps.size(); i++) {
        data.push_back(samps[i].real());
    }

    fftw_complex out[N];
    fft(samps, out);

    std::vector<double> data2;

    for (int i = 0; i < N; i++) {
        data2.push_back(fabs(out[i][0]));
    }

    Graph graph;
    graph.plot(data);
    graph.plot(data2);

    return EXIT_SUCCESS;
}
