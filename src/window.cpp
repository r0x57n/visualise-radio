#include "window.h"

Window::Window() {
    resize(400, 300);


    /* Layout */
    mainLayout = new QVBoxLayout(this);
    buttons = new QGroupBox(this);
    setLayout(mainLayout);


    /* Graphs */
    timeDomain = new QwtPlot;
    timeCurve = new QwtPlotCurve;

    timeCurve->attach(timeDomain);
    timeDomain->replot();

    freqDomain = new QwtPlot;
    freqCurve = new QwtPlotCurve;

    freqCurve->attach(freqDomain);
    freqDomain->replot();

    mainLayout->addWidget(timeDomain);
    mainLayout->addWidget(freqDomain);


    /* Buttons */
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    refresh = new QPushButton("Refresh", this);
    run = new QPushButton("Run", this);

    btnsLayout->addWidget(refresh);
    btnsLayout->addWidget(run);

    buttons->setLayout(btnsLayout);
    mainLayout->addWidget(buttons);
}

Window::~Window() { }
