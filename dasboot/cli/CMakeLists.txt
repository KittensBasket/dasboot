# source:

set(SOURCES cli.cpp)
set(HEADERS cli.hpp)
add_library(cli ${SOURCES} ${HEADERS})
target_include_directories(cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


# tests:

add_executable(cli_ut cli_ut.cpp)

target_link_libraries(
    cli_ut
    PRIVATE
    GTest::gtest
    cli
)

add_test(cli_gtests cli_ut)
