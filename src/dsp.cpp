#include "dsp.h"

#include <cstring>
#include <math.h>
#include <fftw3.h>

void dsp::hamming_window(complex<double> *samples, int N) {
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

void dsp::fft(complex<double> *iqSamples, complex<double> *out, int N) {
    // see https://pysdr.org/content/frequency_domain.html#fast-fourier-transform-fft
    fftw_complex *in, *freqData;
    fftw_plan p;

    in = reinterpret_cast<fftw_complex*>(iqSamples);
    freqData = reinterpret_cast<fftw_complex*>(out);

    p = fftw_plan_dft_1d(N, in, freqData, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    memcpy(out, freqData, N*sizeof(complex<double>));

    fftw_destroy_plan(p);
}


void dsp::fft_shift(complex<double> *freqData, int N) {
    // see https://pysdr.org/content/frequency_domain.html#fft-in-python

    complex<double> temp[N];
    memcpy(temp, freqData, N*sizeof(complex<double>));

    // rotate right segment
    memcpy(freqData, &freqData[N/2], (N/2)*sizeof(complex<double>));

    // rotate left segment
    memcpy(&freqData[N/2], temp, (N/2)*sizeof(complex<double>));
}
