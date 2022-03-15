#include "window.h"
#include <QFormLayout>

Window::Window() {
    resize(400, 300);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addLayout(get_interactive_layout(), 0, 0); // settings and buttions
    mainLayout->addLayout(get_graphs_layout(), 0, 1); // the main graphs

    mainLayout->setColumnStretch(1, 3); // stretch the graphs
    setLayout(mainLayout);
}

Window::~Window() { }

QGridLayout* Window::get_interactive_layout() {
    QGridLayout *interactive = new QGridLayout(this);
    QGridLayout *interactiveSettings = new QGridLayout(this);
    QGridLayout *interactiveButtons = new QGridLayout(this);

    /* Interactive */
    QFormLayout *interactiveSettingForm = new QFormLayout(this);
    freqInput = new QLineEdit();
    fsInput = new QLineEdit();
    freqCorrInput = new QLineEdit();
    sprInput = new QLineEdit();

    interactiveSettingForm->addRow("Center freq", freqInput);
    interactiveSettingForm->addRow("Sample rate", fsInput);
    interactiveSettingForm->addRow("Freq corr", freqCorrInput);
    interactiveSettingForm->addRow("Samples per read", sprInput);

    refresh = new QPushButton("Refresh", this);
    run = new QPushButton("Run", this);

    interactiveSettings->addLayout(interactiveSettingForm, 0, 0);
    interactiveButtons->addWidget(refresh, 1, 0);
    interactiveButtons->addWidget(run, 1, 1);
    interactive->addLayout(interactiveSettings, 0, 0);
    interactive->addLayout(interactiveButtons, 1, 0);

    return interactive;
}

QVBoxLayout* Window::get_graphs_layout() {
    QVBoxLayout *graphsLayout = new QVBoxLayout(this);

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

    graphsLayout->addWidget(timeDomain);
    graphsLayout->addWidget(freqDomain);

    return graphsLayout;
}
