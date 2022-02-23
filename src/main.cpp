#include <iostream>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include "../include/gnuplot-iostream.h"
#include "rtl-sdr.h"
#include "device.h"
#include <complex>

int main(int argc, char *argv[]) {
    /*std::vector<std::pair<int, int>> data;

    for (int i = 0; i < 100; i++) {
        data.push_back(std::make_pair(rand() % 10, - rand() % 50));
    }

    Gnuplot gp;

    gp << "plot " << gp.file1d(data) << " with lines title 'test'\n" << std::endl;*/

    Device dev(0);
    dev.readSamples(256);

    for (int i = 0; i < (int)dev.samples[0].size() / 2; i += 2) {
        double byte1 = (dev.samples[0][i] / 127.5) - 1;
        double byte2 = (dev.samples[0][i+1] / 127.5) - 1;
        std::complex<double> test;
        test.real(byte1);
        test.imag(byte2);

        //std::complex<double> test(byte1, byte2);
        std::cout << test << " (" << abs(test) << ", " << arg(test) << ")" << std::endl;
    }

    return EXIT_SUCCESS;
}
