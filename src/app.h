#ifndef APP_H_
#define APP_H_

#include <fftw3.h>
#include <vector>
#include <complex>
#include <QObject>
#include "device.h"
#include "window.h"

using std::vector;
using std::complex;

class App : public QObject {
    Q_OBJECT
public:
    App(int argc, char *argv[]);
    ~App();

    int start();

    Device *dev;
    Window *window;

    int samplesPerRead;
public slots:
    void refreshData();
private:
    QApplication* app;
    void fft(vector<complex<double>>& data, fftw_complex* out);
};

#endif // APP_H_
