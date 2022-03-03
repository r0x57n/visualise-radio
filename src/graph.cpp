#include "graph.h"
#include "../include/gnuplot-iostream.h"

void Graph::plot(std::vector<double> data) {
    Gnuplot gp_time;
    gp_time << "plot " << gp_time.file1d(data) << " with lines title 'Time domain'\n" << std::endl;
}
