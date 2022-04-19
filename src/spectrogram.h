#ifndef SPECTOGRAM_H_
#define SPECTOGRAM_H_

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_spectrogram.h>
#include <qwt/qwt_matrix_raster_data.h>
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_interval.h>

#include "device.h"

/**
 * The main window of the application.
 */
class Spectrogram : public QwtPlot {
    Q_OBJECT
public:
    /**
     * Creates all the components and the layout.
     */
    Spectrogram();

    /**
     * Empty.
     */
    ~Spectrogram();

private:
    QwtLinearColorMap *colorMap;
    QwtPlotSpectrogram *spectroPlot;
    QwtMatrixRasterData *matrixData;
};

#endif // SPECTOGRAM_H_
