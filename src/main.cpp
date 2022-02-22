#include <iostream>
#include <stdlib.h>
#include <vector>
#include <mutex>
#include "../include/gnuplot-iostream.h"
#include "rtl-sdr.h"
#include "device.h"

int main(int argc, char *argv[]) {
    /*std::vector<std::pair<int, int>> data;

    for (int i = 0; i < 100; i++) {
        data.push_back(std::make_pair(rand() % 10, - rand() % 50));
    }

    Gnuplot gp;

    gp << "plot " << gp.file1d(data) << " with lines title 'test'\n" << std::endl;*/

    Device dev(0);
    dev.readSamples();

    return EXIT_SUCCESS;
}
