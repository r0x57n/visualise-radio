#ifndef DSP_H_
#define DSP_H_

#include <vector>
#include <complex>

using std::complex;

namespace dsp {
    void hamming_window(complex<double> *samples, int N);
    void fft(complex<double> *iqSamples, complex<double> *out, int N);
    void fftshift(complex<double> *freqData, int N);
}


#endif // DSP_H_
