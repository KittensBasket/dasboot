# source:

find_package(CLI11 REQUIRED)

set(SOURCES cli.cpp)
set(HEADERS cli.hpp)
add_library(cli ${SOURCES} ${HEADERS})
target_include_directories(cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# tests:

find_package(GTest REQUIRED)

add_executable(cli_ut cli_ut.cpp)

target_link_libraries(
    cli_ut
    PRIVATE
    GTest::GTest
    cli
)

add_test(tape_gtests cli_ut)
