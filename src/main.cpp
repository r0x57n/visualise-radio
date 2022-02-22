#include <iostream>
#include <stdlib.h>
#include <vector>
#include "../include/gnuplot-iostream.h"
#include "rtl-sdr.h"
#include "device.h"

// check freq 99.8e6 (radio fm)
// use these defaults https://pyrtlsdr.readthedocs.io/en/latest/rtlsdr.html
// set with rtlsdr_set_opt_string()

int main(int argc, char *argv[]) {
    /*std::vector<std::pair<int, int>> data;

    for (int i = 0; i < 100; i++) {
        data.push_back(std::make_pair(rand() % 10, - rand() % 50));
    }

    Gnuplot gp;

    gp << "plot " << gp.file1d(data) << " with lines title 'test'\n" << std::endl;*/

    Device dev;
    dev.init(0);
    dev.printDebug();
    dev.close();

    return EXIT_SUCCESS;
}
