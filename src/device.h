#ifndef DEVICE_H_
#define DEVICE_H_

#include <rtl-sdr.h>
#include <complex>
#include <vector>

using std::vector;
using std::complex;

/**
 * Handles all the communication with the rtl-sdr device
 * through the rtl-sdr.h library.
 */
class Device {
public:
    /**
     * Sets index to the first found device, calls init().
     */
    Device();

    /**
     * Sets index to arg. index, calls init().
     */
    Device(int index);

    /**
     * Closes the opened device.
     */
    ~Device();

    /*
     * Reading samples
     */

    /**
     * Reads the given amount of samples and returns them.
     */
    vector<complex<double>> read_samples_sync(int amount);

    /**
     * Not implemented...
     */
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
private:
    int index;
    rtlsdr_dev_t *dev;

    /**
     * Opens and initialises the device with hardcoded values.
     */
    void init();

    /**
     * Takes a given buffer of given size that contains
     * raw samples, turn them into normalized IQ samples and
     * returns them.
     */
    vector<complex<double>> bytes_to_iq(uint8_t *buf, int size);
};

#endif // DEVICE_H_
