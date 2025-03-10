find_package(GTest REQUIRED)

add_executable(cli_ut cli_ut.cpp)

target_link_libraries(
    cli_ut
    PRIVATE
    GTest::GTest
    cli
)

add_test(tape_gtests cli_ut)
