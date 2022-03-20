#ifndef APP_H_
#define APP_H_

#include <fftw3.h>
#include <vector>
#include <complex>
#include <QApplication>
#include <QObject>
#include "logger.h"
#include "device.h"
#include "window.h"
#include "dsp.h"

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

private:
    unique_ptr<QApplication> app;
    Logger log;
    bool readingAsync;

    void toggle_async_read();
    void fetch_samples_once();

    void connect_signals();
    void keyboard_signals();
    void interface_signals();

    void init_device();

    /**
     * Refreshes the graphs with the first vector sample inside of
     * the SDR device's sample buffer.
     */
    void refresh_graph();

    /**
     * Populates settings fields in window with values
     * from the given rtl-sdr device.
     */
    void populate_interface();

    /**
     * Updates the SDR device, sets oldValue if not succesfull.
     */
    void update(SDR::Settings setting, int value, int oldValue);
};

#endif // APP_H_
