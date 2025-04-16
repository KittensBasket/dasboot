#!/usr/bin/env bash

git clone https://github.com/protocolbuffers/protobuf && \
cd protobuf && \
cmake . && \
cmake --build . --parallel 8 && \
sudo cmake --install . && \
cd .. && \
rm -rf protobuf
