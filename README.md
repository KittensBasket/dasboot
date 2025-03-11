# Dasboot

A small containerization utility, written in C/C++. Made as team pet project.

## Build (Ubuntu)

To build this project first time, the following programs should be installed in your operating system:
* [CMake](#install-CMake);
* [Clang](#install-clang);
* [CLI11](#install-cli11);
* [GTest](#install-gtest).

After that, you build project.

Before installing the programs, do not forget to do:

```bash
sudo apt update
```

### Install CMake

```bash
sudo apt install -y cmake
```

### Install Clang

```bash
sudo apt install -y clang
```

### Install CLI11

```bash
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
```

### Install GTest

```bash
sudo apt install -y libgtest-dev
```

### Build and compile

```bash
git clone https://github.com/flown4qqqq/dasboot.git && \
cd dasboot && \
mkdir build && \
cd build && \
cmake .. && \
make
```

You are great!
