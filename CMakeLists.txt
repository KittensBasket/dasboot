#################################################################################
##
##                        Information about project
##
#################################################################################
cmake_minimum_required(VERSION 3.22.1)
project(dasboot LANGUAGES CXX)

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

#################################################################################
##
##                         Project's properties
##
#################################################################################

option(MYLIB_TESTING "Turn on ut-testing" ON)

#################################################################################
##
##                       Compilation's properties
##
#################################################################################

set(CMAKE_CXX_COMPILER "/usr/bin/clang++") # This should be removed in the future
set(CMAKE_CXX_STANDARD 23) # This too

if((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
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

#################################################################################
##
##                         Get executable file
##
#################################################################################

add_executable(dasboot src/main.cpp)

#################################################################################
##
##                           Compiling sources
##
#################################################################################

add_subdirectory(src)

#################################################################################
##
##                              Testing
##
#################################################################################

if(NOT MYLIB_TESTING)
    message(STATUS "Turned off ut-testing")
else()
    enable_testing()
    add_subdirectory(ut)
endif()
