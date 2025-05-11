# Dasboot

A small containerization utility, written in C/C++. Made as team pet project.

## Dependencies
Make sure to have these installed:
* Clang - compiler
* CMake - generates build files
* Ninja - builds
```bash
sudo apt update && \
sudo apt install -y clang cmake ninja-build
```
All other dependencies will be fetched during CMake run.

## Clone the repo
Get the project:
```bash
git clone https://github.com/KittensBasket/dasboot.git && \
cd dasboot
```

## Build the project
Run CMake and then build with Ninja:
```bash
mkdir build && \
cd build && \
cmake .. -G Ninja && \
ninja
```
## Tests
Run 
```bash
ctest
```
to check all modules or for individual module tests - there is an executable file inside its directory ending with _ut.
