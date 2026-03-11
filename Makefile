.PHONY: clean build rebuild test test-clean

clean:
	rm -rf build

build:
	cmake -B build -S . && \
	cmake --build build

rebuild: clean build

test: build
	cd build && ctest --output-on-failure

test-clean: rebuild
	cd build && ctest --output-on-failure
