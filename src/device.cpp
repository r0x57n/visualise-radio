#include "device.h"
#include <iostream>
#include <string>

/* Helpers */

void fail(std::string msg) {
    std::cout << "\n" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void msg(std::string msg) {
    std::cout << "\n" << msg << "\n" << std::endl;
}


/* Constructors/initialisation/destructors */

Device::Device() {
    index = find_devices_get_first_index();
    init();
}

Device::Device(int index) {
    index = index;
    init();
}

Device::~Device() {
    if (rtlsdr_close(dev) < 0)
        fail("Failed to close device, already closed or never opened...");
}

void Device::init() {
    if (rtlsdr_open(&dev, index) < 0) {
        printf("Failed to open device %d\n", index);
        exit(EXIT_FAILURE);
    }

    asyncReading = false;

    rtlsdr_reset_buffer(dev); // has to be reset before reading samples
}


/* Setters */

/**
 * Sets center frequency.
 * Returns 0 on success.
 */
int Device::center_freq(int freq) {
    return rtlsdr_set_center_freq(dev, freq);
}

/**
 * Sets sample rate.
 * Returns 0 on success.
 */
int Device::sample_rate(int sr) {
    return rtlsdr_set_sample_rate(dev, sr);
}

/**
 * Sets frequency correction.
 * Returns 0 on success.
 */
int Device::freq_corr(int fc) {
    return rtlsdr_set_freq_correction(dev, fc);
}

/**
 * Sets tuner bandwidth.
 * Returns 0 on success.
 */
int Device::tuner_bandwidth(int tb) {
    return rtlsdr_set_tuner_bandwidth(dev, tb);
}

void Device::samples_per_read(int amount) {
    samplesPerRead = amount;
}


/* Getters */

int Device::center_freq() {
    return rtlsdr_get_center_freq(dev);
}

int Device::sample_rate() {
    return rtlsdr_get_sample_rate(dev);
}

int Device::freq_corr() {
    return rtlsdr_get_freq_correction(dev);
}

rtlsdr_dev_t* Device::device() {
    return dev;
}

int Device::samples_per_read() {
    return samplesPerRead;
}

int Device::find_devices_get_first_index() {
    int count = rtlsdr_get_device_count();

    if (count < 0)
        fail("No devices found...");

    printf("Found %d devices:\n", count);

    for (int i = 0; i < count; i++)
        std::cout << rtlsdr_get_device_name(i) << std::endl;

    return 0;
}


/* Reading */

static void callback(uint8_t *buf, uint32_t len, void *ctx) {
    Device *dev = static_cast<Device*>(ctx);
    dev->samples.push_back(dev->raw_samples_to_iq(buf, len));
    emit dev->new_samples(); // send a QT signal that the device contains new samples

    if (!dev->asyncReading)
        rtlsdr_cancel_async(dev->device());
}

void Device::read_samples_async() {
    std::thread first(&Device::start_async, this);
    first.detach();
}

void Device::start_async() {
    asyncReading = true;

    if (rtlsdr_read_async(dev, callback, this, 0, samplesPerRead))
        asyncReading = false;
}

void Device::stop_async() {
    if (asyncReading)
        asyncReading = false;
}

int Device::read_samples_sync() {
    uint8_t *buf = new uint8_t[samplesPerRead];
    int read = 0;

    if (rtlsdr_read_sync(dev, buf, samplesPerRead, &read) || read != samplesPerRead)
        return 1;

    samples.push_back(raw_samples_to_iq(buf, samplesPerRead));

    return 0;
}

complex<double>* Device::raw_samples_to_iq(uint8_t *buf, int N) {
    complex<double>* iq = new complex<double>[N];

    for (int i = 0; i < N/2; i++) {
        float byte1 = buf[2*i];
        float byte2 = buf[2*i+1];

        // convert to IQ sample and normalize
        iq[i] = complex<double> ((byte1 / 127.5 - 1.0), (byte2 / 127.5 - 1.0));
    }

    return iq;
}
