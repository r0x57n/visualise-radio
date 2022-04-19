#include "spectrogram.h"

Spectrogram::Spectrogram() {
    // Color map to be used
    colorMap = new QwtLinearColorMap(QwtColorMap::Format::RGB);
    colorMap->setColorInterval(QColorConstants::Blue, QColorConstants::Red);

    matrixData = new QwtMatrixRasterData();
    matrixData->setInterval(Qt::YAxis, QwtInterval(0.0, 1.0));

    for (int i = 0; i < 102; i++) {
        matrixData->setValue(i, 0, 1.0);
    }

    for (int i = 0; i < 102; i++) {
        matrixData->setValue(0, i, 0.0);
    }

    // The actual spectrogram
    spectroPlot = new QwtPlotSpectrogram;
    spectroPlot->setDisplayMode(QwtPlotSpectrogram::ImageMode, true);
    spectroPlot->setColorMap(colorMap);
    spectroPlot->setData(matrixData);
}

Spectrogram::~Spectrogram() { }
