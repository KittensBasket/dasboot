cmake_minimum_required (VERSION 3.22.1)

# Install dependencies

include(FetchContent)

execute_process(COMMAND sudo apt-get update)

execute_process(COMMAND sudo apt-get install -y clang)

find_program(CMAKE_CXX_COMPILER NAMES clang++)
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")

execute_process(COMMAND sudo apt-get install -y clang-tidy)

execute_process(COMMAND sudo apt-get install -y libgtest-dev)

FetchContent_Declare(
        cli11
        QUIET
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG v2.5.0
        OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(cli11)

add_executable(app ./dasboot/app/main.cpp)
target_include_directories(app PRIVATE ./dasboot/cli)
target_link_libraries(app PRIVATE CLI11::CLI11)

# End Install dependencies



# Find compilers

find_program(CMAKE_CXX_COMPILER NAMES clang++)
if (NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "Compiler is not found!")
else()
    message("-- Current compiler : ${CMAKE_CXX_COMPILER}")
endif()

# Start flags

if (CMAKE_CXX_COMPILER)
    add_compile_options(
        -Werror

        -Wall
        -Wextra
        -Wpedantic

        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wctor-dtor-privacy
        -Wenum-compare
        -Wfloat-equal
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Woverloaded-virtual
        -Wredundant-decls
        -Wsign-conversion
        -Wsign-promo
        -Wzero-as-null-pointer-constant

        -Ofast
    )
endif()

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# End flags


# Start  clang-tidy

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

if(NOT CLANG_TIDY_COMMAND)
    message(FATAL_ERROR "Clang-tidy is not found!")
else()
    message(STATUS "Clang-tidy is ON")

    # Google C++ Style
    set(CLANGTIDY_EXTRA_ARGS
        "-checks=google-*"
        "-warnings-as-errors=*"
    )

    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};-p=${CMAKE_BINARY_DIR};${CLANGTIDY_EXTRA_ARGS}" CACHE STRING "" FORCE)
endif()


# End clang-tidy


project(dasboot LANGUAGES CXX)

include_directories(.)

add_subdirectory(dasboot)

# Unit-tests

option(MYLIB_TESTING "Turn on ut-testing" ON)

if(NOT MYLIB_TESTING)
    message(STATUS "Turned off ut-testing")
else()
    enable_testing()
endif()
