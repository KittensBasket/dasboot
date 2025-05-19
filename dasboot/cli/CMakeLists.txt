# source:

set(SOURCES cli.cpp)
set(HEADERS cli.hpp)
add_library(cli ${SOURCES} ${HEADERS})
apply_compile_flags(cli)
target_link_libraries(cli PUBLIC CLI11::CLI11)
target_link_libraries(cli PUBLIC ProtobufMessages)
target_link_libraries(cli PUBLIC controller)
target_link_libraries(cli PUBLIC nlohmann_json)
target_link_libraries(cli PUBLIC cru)

# tests:

add_executable(cli_ut cli_ut.cpp)
apply_compile_flags(cli_ut)

target_link_libraries(
    cli_ut
    PRIVATE
    GTest::gtest
    cli
    ProtobufMessages
    controller
    nlohmann_json
    cru
)

gtest_discover_tests(cli_ut)
