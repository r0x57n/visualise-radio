#ifndef DEVICE_H_
#define DEVICE_H_

#include <rtl-sdr.h>
#include <complex>
#include <vector>

using std::vector;
using std::complex;

class Device {
private:
    int index;
    rtlsdr_dev_t *dev;

    void init();
    vector<complex<double>> bytes_to_iq(uint8_t *buf, int size);
public:
    vector<uint8_t> samples;

    Device();
    Device(int index);
    ~Device();

    /* Reading samples */
    vector<complex<double>> read_samples_sync(int amount);
    void read_samples_async();
    void stop_reading_async();

    /* Setters */
    void center_freq(int freq);
    void sample_rate(int sr);
    void freq_corr(int fc);
    void tuner_bandwidth(int tb);

    /* Getters */
    int center_freq();
    int sample_rate();
    int freq_corr();
    rtlsdr_dev_t* device();
    int find_devices_get_first_index();
};

#endif // DEVICE_H_
