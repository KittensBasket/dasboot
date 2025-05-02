# source:

set(SOURCES cli.cpp commands.cpp)
set(HEADERS cli.hpp commands.hpp)
add_library(cli ${SOURCES} ${HEADERS})
apply_compile_flags(cli)
target_link_libraries(cli PUBLIC CLI11::CLI11 ProtobufMessages)

# tests:

add_executable(cli_ut cli_ut.cpp)
apply_compile_flags(cli_ut)

target_link_libraries(
    cli_ut
    PRIVATE
    GTest::gtest
    cli
    ProtobufMessages
)

add_test(cli_gtests cli_ut)
