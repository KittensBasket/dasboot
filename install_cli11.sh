#!/usr/bin/env bash

git clone https://github.com/CLIUtils/CLI11 && cd CLI11/ && \
mkdir build && \
cd build && \
git submodule update --init && \
cmake .. && \
cmake --build . && \
sudo cmake --install . && \
cd ../../ && \
rm -rf CLI11/