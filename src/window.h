#ifndef WINDOW_H_
#define WINDOW_H_

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_plot_zoomer.h>
#include <QKeyEvent>

#include "device.h"

/**
 * The main window of the application.
 */
class Window : public QWidget {
    Q_OBJECT
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
    QwtPlotZoomer *timeZoomer;
    QwtPlot *freqDomain;
    QwtPlotCurve *freqCurve;

    QPushButton *refresh;
    QPushButton *run;

    QSpinBox *center_freq;
    QSpinBox *sample_rate;
    QSpinBox *freq_corr;
    QSpinBox *samples_per_read;

    bool unitializedZoom; // we need to set the soom base after we have some real values
signals:
    void start_async();
    void fetch_once();
    void increase_center_freq();
    void decrease_center_freq();
private:
    QGridLayout* get_interactive_layout();
    QVBoxLayout* get_graphs_layout();

    void keyPressEvent(QKeyEvent *event) override;
};


#endif // WINDOW_H_
