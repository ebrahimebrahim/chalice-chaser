# This makefile is meant for quick testing during development.
# This is not meant to be used to build the application.
# See readme for how to build the application.

.PHONY: build run clean test debug doc all showdoc

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

showdoc: doc
	firefox doc/html/index.html

experiment: build
	build/experiment
