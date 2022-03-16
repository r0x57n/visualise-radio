#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

#include "device.h"

/**
 * The main window of the application.
 */
class Window : public QWidget {
public:
    /**
     * Creates all the components and the layout.
     */
    Window();

    /**
     * Empty.
     */
    ~Window();

    QwtPlot *timeDomain;
    QwtPlotCurve *timeCurve;
    QwtPlot *freqDomain;
    QwtPlotCurve *freqCurve;

    QPushButton *refresh;
    QPushButton *run;

    QLineEdit *center_freq;
    QLineEdit *sample_rate;
    QLineEdit *freq_corr;
    QLineEdit *samples_per_read;
private:
    QGridLayout* get_interactive_layout();
    QVBoxLayout* get_graphs_layout();
};


#endif // WINDOW_H_
