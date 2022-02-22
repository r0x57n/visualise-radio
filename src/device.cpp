#include "device.h"
#include <iostream>
#include <string>

void fail(std::string msg) {
    std::cout << "\n" << msg << std::endl;
    exit(EXIT_FAILURE);
}

void Device::init(int index = -1) {
    if (index < 0)
        index = this->getDeviceIndex();

    int ret = rtlsdr_open(&this->dev, index);

    if (ret < 0) {
        printf("Failed to open device %d\n", 0);
        exit(EXIT_FAILURE);
    }

    if (rtlsdr_set_center_freq(this->dev, 99.8e6) < 0)
        fail("Failed to set center freq");

    if (rtlsdr_set_sample_rate(this->dev, 1.024e6) < 0)
        fail("Failed to set sample rate");
}

int Device::getDeviceIndex() {
    int count = rtlsdr_get_device_count();

    if (count < 0) {
        printf("No devices found...");
        exit(EXIT_FAILURE);
    }

    printf("Found %d devices:\n", count);

    for (int i = 0; i < count; i++) {
        std::cout << rtlsdr_get_device_name(i) << std::endl;
    }

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
}

void Device::close() {
    int ret = rtlsdr_close(dev);

    if (ret < 0) {
        printf("Failed to close device, already closed or never opened...\n");
        exit(EXIT_FAILURE);
    }
}
