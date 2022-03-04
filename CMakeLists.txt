cmake_minimum_required(VERSION 3.1.0)

project(vradio
        VERSION 1.0.0
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for LSP to find headers

set (SRC_LIST src/main.cpp src/graph.cpp src/device.cpp)
set (LIBS librtlsdr.so boost_iostreams boost_system boost_filesystem fftw3)

OPTION(BUILD_UI "Build with QT support to allow an actual UI" ON)
if(BUILD_UI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)

    find_package(Qt5 COMPONENTS
                 Widgets REQUIRED
                 Charts REQUIRED)

    set(SRC_LIST ${SRC_LIST} src/qtChart.cpp)

    set (${LIBS} Qt5::Widgets Qt5::Charts)
endif()

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
find_package(FFTW3 REQUIRED)

add_executable(vradio
    ${SRC_LIST}
)

target_link_libraries(vradio ${LIBS})
