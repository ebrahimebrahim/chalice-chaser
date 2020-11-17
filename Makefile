.PHONY: run clean test debug doc

build:
	mkdir build
	cd build; cmake ..;	make

debug:
	mkdir build
	cd build; cmake -DCMAKE_BUILD_TYPE=Debug ..; make

run: build
	build/garbanzo

clean:
	rm -rf build

doc: 
	doxygen doxygen_settings.cfg