# Dasboot

A small containerization utility, written in C/C++. Made as uni project.

# Build

```bash
sudo apt update
```

## Download cmake

TODO

## Download compliler

TODO

## Download gtest

```bash
sudo apt install libgtest-dev
```

## Download Cli11

```bash
cd ~/
git clone https://github.com/CLIUtils/CLI11
cd CLI11/
mkdir build
cd build
git submodule update --init
cmake ..
cmake --build .
sudo cmake --install .
cd ~/
rm -rf CLI11/
```

## Build dasboot

```bash
mkdir build
cd build
cmake ..
make
```
