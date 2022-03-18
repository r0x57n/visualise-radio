#include "window.h"
#include <QFormLayout>

Window::Window() {
    resize(400, 300);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addLayout(get_interactive_layout(), 0, 0); // settings and buttions
    mainLayout->addLayout(get_graphs_layout(), 0, 1); // the main graphs

    mainLayout->setColumnStretch(1, 3); // stretch the graphs
    setLayout(mainLayout);

    unitializedZoom = true;
}

Window::~Window() { }

QGridLayout* Window::get_interactive_layout() {
    QGridLayout *interactive = new QGridLayout(this);
    QGridLayout *interactiveSettings = new QGridLayout(this);
    QGridLayout *interactiveButtons = new QGridLayout(this);

    /* Interactive */
    QFormLayout *interactiveSettingForm = new QFormLayout(this);

    center_freq = new QSpinBox();
    center_freq->setGroupSeparatorShown(true);
    center_freq->setRange(0, 2200*10e6);
    center_freq->setSingleStep(10e4);

    sample_rate = new QSpinBox();
    sample_rate->setGroupSeparatorShown(true);
    sample_rate->setRange(0, 3.5*10e6);

    freq_corr = new QSpinBox();
    freq_corr->setGroupSeparatorShown(true);
    freq_corr->setRange(0, 100);

    samples_per_read = new QSpinBox();
    samples_per_read->setGroupSeparatorShown(true);
    samples_per_read->setRange(0, 10e6);

    interactiveSettingForm->addRow("Center freq", center_freq);
    interactiveSettingForm->addRow("Sample rate", sample_rate);
    interactiveSettingForm->addRow("Freq corr", freq_corr);
    interactiveSettingForm->addRow("Samples per read", samples_per_read);

    refresh = new QPushButton("[F]etch once", this);
    run = new QPushButton("[R]un", this);

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

    timeZoomer = new QwtPlotZoomer( timeDomain->canvas() );
    timeZoomer->setAxes(QwtPlot::xBottom, QwtPlot::yRight);

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

void Window::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_R:
            emit start_async();
        break;
        case Qt::Key_F:
            emit fetch_once();
        break;
        case Qt::Key_L:
            if (event->modifiers() & Qt::ShiftModifier) {
                timeZoomer->moveBy(1, 0);
            } else {
                emit increase_center_freq();
            }
        break;
        case Qt::Key_H:
            if (event->modifiers() & Qt::ShiftModifier) {
                timeZoomer->moveBy(-1, 0);
            } else {
                emit decrease_center_freq();
            }
        break;
        case Qt::Key_K:
            if (event->modifiers() & Qt::ShiftModifier) {
                timeZoomer->zoom(100);
            } else {
                timeZoomer->zoom(1);
            }
        break;
        case Qt::Key_J:
            if (event->modifiers() & Qt::ShiftModifier)
                timeZoomer->zoom(0);
            else
                timeZoomer->zoom(-1);
        break;
    }
}
