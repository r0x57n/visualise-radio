#include "window.h"

Window::Window() {
    resize(400, 300);


    /* Layout */
    mainLayout = new QVBoxLayout(this);
    buttons = new QGroupBox(this);
    setLayout(mainLayout);


    /* Graph for time domain */
    timeDomain = new QwtPlot;
    timeDomain->setTitle("Time Domain");
    timeDomain->setAxisScale(QwtPlot::Axis::yLeft, -1, 1, 0);
    timeDomain->setAxisTitle(QwtPlot::Axis::yLeft, "Amplitude");
    timeDomain->setAxisTitle(QwtPlot::Axis::xBottom, "Time");

    timeCurve = new QwtPlotCurve;
    timeCurve->attach(timeDomain);

    timeDomain->replot();


    /* Graph for freq domain */
    freqDomain = new QwtPlot;
    freqDomain->setTitle("Freq Domain");
    freqDomain->setAxisScale(QwtPlot::Axis::yLeft, 0, 2000, 0);
    freqDomain->setAxisTitle(QwtPlot::Axis::yLeft, "Magnitude");
    freqDomain->setAxisTitle(QwtPlot::Axis::xBottom, "Frequency (MHz)");

    freqCurve = new QwtPlotCurve;
    freqCurve->attach(freqDomain);

    freqDomain->replot();


    /* Buttons */
    QHBoxLayout *btnsLayout = new QHBoxLayout;
    refresh = new QPushButton("Refresh", this);
    run = new QPushButton("Run", this);

    btnsLayout->addWidget(refresh);
    btnsLayout->addWidget(run);


    /* Add widgets to layouts */
    mainLayout->addWidget(timeDomain);
    mainLayout->addWidget(freqDomain);
    buttons->setLayout(btnsLayout);
    mainLayout->addWidget(buttons);
}

Window::~Window() { }
