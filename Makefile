.PHONY: build run clean test debug doc all

all: build doc test

build:
	if [ -d build ]; then cd build; make; else mkdir build; cd build; cmake ..; make; fi

debug:
	if [ -d build ]; then cd build; make; else mkdir build; cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make; fi

run: build
	build/garbanzo

clean:
	rm -rf build

doc: 
	doxygen doxygen_settings.cfg