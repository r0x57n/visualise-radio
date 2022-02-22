#ifndef DEVICE_H_
#define DEVICE_H_

#include "rtl-sdr.h"

class Device {
private:
    rtlsdr_dev_t *dev;
public:
    void init(int index);
    int getDeviceIndex();
    void printDebug();
    int getGain();
    int getFreq();
    int getSR();
    void close();
};

#endif // DEVICE_H_
