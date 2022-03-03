#include "device.h"
#include <iostream>
#include <string>
#include <thread>

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
    this->index = this->find_devices_get_first_index();
    this->init();
}

Device::Device(int index) {
    this->index = index;
    this->init();
}

Device::~Device() {
    int ret = rtlsdr_close(this->dev);

    if (ret < 0)
        fail("Failed to close device, already closed or never opened...");
}

void Device::init() {
    int ret = rtlsdr_open(&this->dev, index);

    if (ret < 0) {
        printf("Failed to open device %d\n", 0);
        exit(EXIT_FAILURE);
    }

    this->center_freq(99.8e6);
    this->sample_rate(1.024e6);
    this->tuner_bandwidth(0);
    this->freq_corr(60);

    rtlsdr_reset_buffer(this->dev); // has to be reset before reading samples
}


/* Setters */

void Device::center_freq(int freq) {
    if (rtlsdr_set_center_freq(this->dev, freq) < 0)
        fail("Failed to set center frequency.");
}

void Device::sample_rate(int sr) {
    if (rtlsdr_set_sample_rate(this->dev, sr) < 0)
        fail("Failed to set sample rate.");
}

void Device::freq_corr(int fc) {
    if (rtlsdr_set_freq_correction(this->dev, fc) < 0)
        fail("Failed to set frequency correction.");
}

void Device::tuner_bandwidth(int tb) {
    if (rtlsdr_set_tuner_bandwidth(this->dev, tb) < 0)
        fail("Failed to set tuner bandwidth.");
}


/* Getters */

int Device::center_freq() {
    return rtlsdr_get_center_freq(this->dev);
}

int Device::sample_rate() {
    return rtlsdr_get_sample_rate(this->dev);
}

int Device::freq_corr() {
    return rtlsdr_get_freq_correction(this->dev);
}

rtlsdr_dev_t* Device::device() {
    return this->dev;
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


/* Async reading */

static void callback(uint8_t *buf, uint32_t len, void *ctx) {
    //Device *dev = static_cast<Device *>(ctx);

    std::vector<uint8_t> read;
    for (int i = 0; i < (int)len; i++) {
        read.push_back(buf[i]);
    }

    std::cout << "read: " << len << std::endl;

    //dev->samples.push_back(read);
}

void start_reading(void *ctx) {
    msg("reading...");

    Device *dev = static_cast<Device *>(ctx);

    rtlsdr_read_async(dev->device(), callback, dev, 0, 0);

    printf("\nRead %ld samples...\n", dev->samples.size());
    fflush(stdout);
}

void Device::read_samples_async() {
    // std::thread first (startReading, this);
    // first.join();
}

void Device::stop_reading_async() {
    rtlsdr_cancel_async(this->dev);
}


/* Sync reading */

vector<complex<double>> Device::read_samples_sync(int amount = 1024) {
    amount *= 2; // samples are complex (real/imag)

    uint8_t *buf = (uint8_t*)malloc(amount * sizeof(uint8_t));
    int read = 0;

    if (rtlsdr_read_sync(this->dev, buf, amount, &read) < 0 || read != amount)
        fail("Couldn't read the requested amount.");

    return bytes_to_iq(buf, amount);
}

vector<complex<double>> Device::bytes_to_iq(uint8_t *buf, int size) {
    vector<complex<double>> iq;

    for (int i = 0; i < size/2; i++) {
        float byte1 = buf[2*i];
        float byte2 = buf[2*i+1];

        // convert to IQ sampling and normalize
        iq.push_back(complex<double> ((byte1 / 127.5 - 1.0), (byte2 / 127.5 - 1.0)));
    }

    free(buf);

    return iq;
}
