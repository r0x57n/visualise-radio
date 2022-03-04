#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

class Window : public QWidget {
public:
    Window();
    QVBoxLayout *layout;
    QwtPlotCurve *timeCurve;
    QwtPlotCurve *freqCurve;

private:
    QwtPlot *timeDomain;
    QwtPlot *freqDomain;
    QPushButton *refresh;
};


#endif // WINDOW_H_
