.PHONY: clean build rebuild test test-clean

clean:
	rm -rf build

build:
	cmake -B build -S . -DCMAKE_CXX_COMPILER=g++-15 && \
	cmake --build build 

rebuild: clean build

test: build
	cd build && ctest --output-on-failure

test-strict: build
	cd build && ctest --output-on-failure --stop-on-failure

test-clean: rebuild
	cd build && ctest --output-on-failure
