#include <iostream>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include "../include/gnuplot-iostream.h"
#include "rtl-sdr.h"
#include "device.h"
#include <complex>
#include <fftw3.h>

int main(int argc, char *argv[]) {

    Device dev(0);
    std::vector<std::complex<double>> test = dev.readSamples(16384);
    int N = test.size();

    std::vector<double> data;
    fftw_complex in[N];

    for (int i = 0; i < test.size(); i++) {
        data.push_back(test[i].real());
        in[i][0] = test[i].real();
        in[i][1] = test[i].imag();
    }

    Gnuplot gp_time;
    gp_time << "plot " << gp_time.file1d(data) << " with lines title 'test'\n" << std::endl;


    fftw_complex out[N];
    fftw_plan p;

    //in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * data.size());
    //in = reinterpret_cast<fftw_complex*>(&test);
    //memcpy( &in, &test, sizeof( fftw_complex ) * N);
    //out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    std::vector<double> data2;

    for (int i = 0; i < N; i++) {
        data2.push_back(abs(out[i][0]));
    }

    Gnuplot gp_freq;
    gp_freq << "plot " << gp_freq.file1d(data2) << " with lines title 'test'\n" << std::endl;

    fftw_destroy_plan(p);
    //fftw_free(in);
    //fftw_free(out);

    return EXIT_SUCCESS;
}
