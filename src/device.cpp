#include "device.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void fail(std::string msg) {
    std::cout << "\n" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void msg(std::string msg) {
    std::cout << "\n" << msg << "\n" << std::endl;
}

Device::Device() {
    this->index = this->getDeviceIndex();
    this->init();
}

Device::Device(int index) {
    this->index = index;
    this->init();
}

Device::~Device() {
    this->close();
}

void Device::init() {
    int ret = rtlsdr_open(&this->dev, index);

    if (ret < 0) {
        printf("Failed to open device %d\n", 0);
        exit(EXIT_FAILURE);
    }

    if (rtlsdr_set_center_freq(this->dev, 99.8e6) < 0)
        fail("Failed to set center freq");

    if (rtlsdr_set_sample_rate(this->dev, 1.024e6) < 0)
        fail("Failed to set sample rate");

    // if (rtlsdr_set_tuner_gain_mode(this->dev, 0) < 0)
    //     fail("Failed to set gain mode automatic");

    // if (rtlsdr_set_tuner_bandwidth(this->dev, 0) < 0)
    //     fail("Failed to set bandwidth automatic");

    if (rtlsdr_set_freq_correction(this->dev, 60) < 0)
        fail("Failed to set freq correction");
}

int Device::getDeviceIndex() {
    int count = rtlsdr_get_device_count();

    if (count < 0)
        fail("No devices found...");

    printf("Found %d devices:\n", count);

    for (int i = 0; i < count; i++)
        std::cout << rtlsdr_get_device_name(i) << std::endl;

    return 0;
}


int Device::getGain() {
    return rtlsdr_get_tuner_gain(this->dev);
}

int Device::getFreq() {
    return rtlsdr_get_center_freq(this->dev);
}

int Device::getSR() {
    return rtlsdr_get_sample_rate(this->dev);
}

void Device::printDebug() {
    printf("\nGain: %d\nFreq: %d\nSample rate: %d\n", this->getGain(), this->getFreq(), this->getSR());
    fflush(stdout);
}

void Device::close() {
    int ret = rtlsdr_close(this->dev);

    if (ret < 0)
        fail("Failed to close device, already closed or never openede...");
}

static void callback(unsigned char *buf, uint32_t len, void *ctx) {
    Device *dev = static_cast<Device *>(ctx);
    dev->samplesToRead -= len;

    if (dev->samplesToRead <= 0)
        dev->stopReading();

    std::vector<unsigned char> read;
    for (int i = 0; i < (int)len; i++) {
        read.push_back(buf[i]);
    }

    std::cout << "read: " << len << std::endl;

    //dev->samples.push_back(read);
}

void startReading(void *ctx) {
    msg("reading...");

    Device *dev = static_cast<Device *>(ctx);

    rtlsdr_read_async(dev->getDev(), callback, dev, 0, 0);

    printf("\nRead %ld samples...\n", dev->samples.size());
    fflush(stdout);
}

void Device::stopReading() {
    rtlsdr_cancel_async(this->dev);
}

std::vector<std::complex<double>> Device::readSamples(int amount) {
    // buffer has to be reset before reading
    rtlsdr_reset_buffer(this->dev);

    amount *= 2; // complex is real+imag
    this->samplesToRead = amount;

    // std::thread first (startReading, this);
    // first.join();

    uint8_t *buf = (uint8_t*)malloc(amount * sizeof(uint8_t));
    int nread = 0;
    int res = rtlsdr_read_sync(this->dev, buf, amount, &nread);

    if (nread != amount)
        fail("couldn't read the requested amount");

    return bytesToIq(buf, amount);
}

std::vector<std::complex<double>> Device::bytesToIq(uint8_t *buf, int size) {
    std::vector<std::complex<double>> test;

    for (int i = 0; i < size/2; i++) {
        float byte1 = (float) buf[2*i];
        float byte2 = (float) buf[2*i+1];

        std::complex<double> test2((byte1 / 127.5 - 1.0), (byte2 / 127.5 - 1.0));
        test.push_back(test2);

        //std::cout << test << std::endl;
    }

    free(buf);

    return test;
}

rtlsdr_dev_t* Device::getDev() {
    return this->dev;
}
