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

    /**
     * Populates settings fields with values
     * from the given rtl-sdr device.
     */
    void populate_with_device(Device *sdr);

    QwtPlot *timeDomain;
    QwtPlotCurve *timeCurve;
    QwtPlot *freqDomain;
    QwtPlotCurve *freqCurve;

    QPushButton *refresh;
    QPushButton *run;
private:
    QGridLayout* get_interactive_layout();
    QVBoxLayout* get_graphs_layout();

    QLineEdit *freqInput;
};


#endif // WINDOW_H_
