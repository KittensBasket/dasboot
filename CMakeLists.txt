cmake_minimum_required(VERSION 3.10)
project(dasboot)

set(CMAKE_CXX_STANDART 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
