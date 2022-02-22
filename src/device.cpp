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
    this->val = 0;
    this->init();
}

Device::Device(int index) {
    this->index = index;
    this->val = 0;
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

    if (rtlsdr_set_tuner_gain_mode(this->dev, 0) < 0)
        fail("Failed to set gain mode automatic");

    if (rtlsdr_set_tuner_bandwidth(this->dev, 0) < 0)
        fail("Failed to set bandwidth automatic");
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
    printf("entered callback");
    fflush(stdout);

    Device *dev = static_cast<Device *>(ctx);
    dev->val += 1;
}

void startReading(void *ctx) {
    msg("reading...");

    Device *dev = static_cast<Device *>(ctx);
    //const std::lock_guard<std::mutex> lock(dev->reading);

    rtlsdr_read_async(dev->getDev(), callback, dev, 0, 1);

    msg("done reading...");
}

void Device::readSamples() {
    msg("start reading thread");

    rtlsdr_reset_buffer(this->dev);

    std::thread first (startReading, this);
    first.detach();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    rtlsdr_cancel_async(this->dev);

    printf("val: %d", this->val);

    /*while (true) {
        msg("waiting for samples");
        const std::lock_guard<std::mutex> lock(reading);
        msg("released from reading");
    }*/
}

void Device::stopReading() {
    rtlsdr_cancel_async(this->dev);
}

rtlsdr_dev_t* Device::getDev() {
    return this->dev;
}
