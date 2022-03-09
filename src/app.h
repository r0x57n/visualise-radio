#ifndef APP_H_
#define APP_H_

#include <fftw3.h>
#include <vector>
#include <complex>
#include <QApplication>
#include <QObject>
#include "device.h"
#include "window.h"

using std::vector;
using std::complex;
using std::unique_ptr;

/**
 * This class handles the communication between the
 * rtl-sdr device and the UI.
 */
class App : public QObject {
    Q_OBJECT
public:
    /**
     * Initializes the device and necessary QT things for the UI.
     */
    App(int argc, char *argv[]);

    /**
     * Empty.
     */
    ~App();

    /**
     * Enters the QApplication event loop.
     */
    int start();

    unique_ptr<Device> sdr; // The rtl-sdr device.
    unique_ptr<Window> window; // The application window (a QWidget).

public slots:
    /**
     * Gets called when the refresh button is pressed from the UI.
     * Refreshes the data for the graphs.
     */
    void refresh_data();
private:
    unique_ptr<QApplication> app;
    void fft(vector<complex<double>>& data, fftw_complex* out);
    void toggle_async_read();
    void refresh_graph();
};

#endif // APP_H_
