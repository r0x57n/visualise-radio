#include "logger.h"

Logger::Logger() { }

void operator<<(Logger log, const char* msg) {
    std::cout << "LOG: " << msg << std::endl;
}

void operator<<(Logger log, const int num) {
    std::cout << "LOG: " << num << std::endl;
}
