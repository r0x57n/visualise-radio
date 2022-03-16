#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

class Logger {
public:
    Logger();

    friend void operator<<(Logger log, const char* msg);
    friend void operator<<(Logger log, const int num);
private:
    void write();
};


#endif // LOGGER_H_
