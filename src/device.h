#ifndef DEVICE_H_
#define DEVICE_H_

#include <complex>
#include <vector>
#include "rtl-sdr.h"

class Device {
private:
    rtlsdr_dev_t *dev; // actual device
    int index;     // index of device from rtl-sdr.h ordering

    /* Initialize/cleanup */
    void init();
    int getDeviceIndex();
    void close();
public:
    std::vector<uint8_t> samples;
    int samplesToRead;

    Device();
    Device(int index);
    ~Device();

    std::vector<std::complex<double>> readSamples(int amount);
    std::vector<std::complex<double>> bytesToIq(uint8_t *buf, int size);
    void stopReading();
    void printDebug();

    /* Getters */
    int getGain();
    int getFreq();
    int getSR();
    rtlsdr_dev_t* getDev();
};

#endif // DEVICE_H_
