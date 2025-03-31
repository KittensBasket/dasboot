#!/bin/bash

if ! dpkg-query -l "clang" &> /dev/null; then
	sudo apt install -y clang
fi

if ! dpkg-query -l "clang-tidy" &> /dev/null; then
	sudo apt install -y clang-tidy
fi

if [ ! -n "$(find /usr -name 'CLI' -print -quit)" ]; then 
	cd .. && \
	git clone https://github.com/CLIUtils/CLI11 && \
	cd CLI11/ && \
	mkdir build && \
	cd build && \
	git submodule update --init && \
	cmake .. && \
	cmake --build . && \
	sudo cmake --install . && \
	cd ../../ && \
	rm -rf CLI11/
fi

if ! dpkg-query -l "libgtest-dev" &> /dev/null; then
	sudo apt install -y libgtest-dev
fi
