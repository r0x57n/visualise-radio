#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>

class Window : public QWidget {
    Q_OBJECT
public:
    Window();
    QVBoxLayout *layout;

    QwtPlot *timeDomain;
    QwtPlotCurve *timeCurve;
    QwtPlot *freqDomain;
    QwtPlotCurve *freqCurve;

    QPushButton *refresh;
public slots:
    void refreshData();
signals:
    void refreshData(bool val);
private:
};


#endif // WINDOW_H_
