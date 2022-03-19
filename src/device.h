#ifndef DEVICE_H_
#define DEVICE_H_

#include <QObject>
#include <rtl-sdr.h>
#include <complex>
#include <vector>
#include <thread>

using std::vector;
using std::complex;

namespace SDR {
    enum Settings { centerFreq, freqCorr, sampleRate, samplesPerRead };
}

/**
 * Handles all the communication with the rtl-sdr device
 * through the rtl-sdr.h library.
 *
 * Sets signal newSamples when having read new samples
 * asynchrously.
 */
class Device : public QObject {
    Q_OBJECT
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

    /**
     * Reads "samplesPerRead" amount of samples into "samples" vector
     * synchronously.
     */
    int read_samples_sync();

    /**
     * Starts async reading samples from device and stores
     * them inside of this->samples.
     */
    void read_samples_async();

    /**
     * Stops async reading.
     */
    void stop_async();

    /**
     * Takes a given buffer of given size that contains
     * raw samples, turn them into normalized IQ samples and
     * returns them.
     */
    complex<double>* raw_samples_to_iq(uint8_t *buf, int N);

    bool asyncReading; // flag to show if we're reading async currently

    /* Setters. */
    int center_freq(int freq);
    int sample_rate(int sr);
    int freq_corr(int fc);
    int tuner_bandwidth(int tb);
    void samples_per_read(int amount);

    /* Getters. */
    int center_freq();
    int sample_rate();
    int freq_corr();
    int samples_per_read();
    rtlsdr_dev_t* device();
    int find_devices_get_first_index();

    vector<complex<double>*> samples; // The samples that have been read.
    //complex<double> *samples;
signals:
    void new_samples();
private:
    rtlsdr_dev_t *dev;
    int index;

    // We actually retrieve half of the requested amount since
    // IQ samples are complex.
    int samplesPerRead;

    /**
     * Opens and initialises the device with hardcoded values.
     */
    void init();

    void start_async();
};

#endif // DEVICE_H_
