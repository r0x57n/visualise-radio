cmake_minimum_required(VERSION 3.1.0)

project(vradio
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for LSP to find headers
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/") # qwt doesn't have a find cmake module

find_package(FFTW3 REQUIRED)

find_package(Qt5 COMPONENTS
             Widgets REQUIRED)

find_package(Qwt REQUIRED)

set(SRC_LIST src/main.cpp src/app.cpp src/device.cpp src/window.cpp src/logger.cpp src/dsp.cpp src/spectrogram.cpp)
set(LIBS librtlsdr.so fftw3 qwt Qt5::Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

add_executable(vradio
    ${SRC_LIST}
)

target_link_libraries(vradio ${LIBS})
