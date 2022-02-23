#ifndef DEVICE_H_
#define DEVICE_H_

#include <mutex>
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
    std::vector<std::vector<double>> samples;
    int samplesToRead;

    Device();
    Device(int index);
    ~Device();

    void readSamples(int amount);
    void stopReading();
    void printDebug();

    /* Getters */
    int getGain();
    int getFreq();
    int getSR();
    rtlsdr_dev_t* getDev();
};

#endif // DEVICE_H_
