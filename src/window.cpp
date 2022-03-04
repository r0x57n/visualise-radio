#include "window.h"

Window::Window() {
    layout = new QVBoxLayout(this);
    refresh = new QPushButton("Refresh", this);

    timeDomain = new QwtPlot;
    timeCurve = new QwtPlotCurve;

    timeCurve->attach(timeDomain);
    timeDomain->replot();

    freqDomain = new QwtPlot;
    freqCurve = new QwtPlotCurve;

    freqCurve->attach(freqDomain);
    freqDomain->replot();

    layout->addWidget(timeDomain);
    layout->addWidget(freqDomain);
    layout->addWidget(refresh);

    resize(400, 300);
}

void Window::refreshData() {

}
