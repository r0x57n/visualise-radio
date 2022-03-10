#include "window.h"

Window::Window() {
    resize(400, 300);

    /* Layout */
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *buttons = new QGroupBox(this);
    QHBoxLayout *btnsLayout = new QHBoxLayout;

    QGroupBox *graphs = new QGroupBox(this);
    QVBoxLayout *graphsLayout = new QVBoxLayout;


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
    refresh = new QPushButton("Refresh", this);
    run = new QPushButton("Run", this);


    /* Add widgets to layouts */
    graphsLayout->addWidget(timeDomain);
    graphsLayout->addWidget(freqDomain);
    graphs->setLayout(graphsLayout);
    mainLayout->addWidget(graphs);

    btnsLayout->addWidget(refresh);
    btnsLayout->addWidget(run);
    buttons->setLayout(btnsLayout);
    mainLayout->addWidget(buttons);

    setLayout(mainLayout);
}

Window::~Window() { }
