CXX = g++
CPPFLAGS = -Wall -g -c
LIBS= -l boost_iostreams -l boost_system -l boost_filesystem -lfftw3 -lm

obj_files = build/main.o build/device.o

.PHONY: all
all: $(obj_files)
	$(CXX) -o build/test $(obj_files) $(LIBS) /usr/lib/librtlsdr.so

build/%.o : src/%.cpp
	$(CXX) -c $(CPPFLAGS) $< -o $@
