#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

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
private:
};


#endif // WINDOW_H_
