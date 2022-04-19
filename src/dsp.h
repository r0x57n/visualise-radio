#ifndef DSP_H_
#define DSP_H_

#include <fftw3.h>
#include <cstring>
#include <math.h>
#include <vector>
#include <complex>


using std::complex;

namespace dsp {
    void hamming_window(complex<double> *samples, int N);
    void fft(complex<double> *iqSamples, complex<double> *out, int N);
    void fft_shift(complex<double> *freqData, int N);
}


#endif // DSP_H_
