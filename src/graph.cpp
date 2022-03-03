#include "graph.h"
#include "../include/gnuplot-iostream.h"

void Graph::plot(vector<double> data, string name) {
    Gnuplot gp_time;
    gp_time << "plot " << gp_time.file1d(data) << " with lines title '" << name << "'\n" << std::endl;
}
