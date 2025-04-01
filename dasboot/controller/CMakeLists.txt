# source:

set(SOURCES controller.cpp)
set(HEADERS controller.hpp)
add_library(controller ${SOURCES} ${HEADERS})
target_include_directories(controller PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# tests:

find_package(GTest REQUIRED)

add_executable(controller_ut controller_ut.cpp)

target_link_libraries(
    controller_ut
    PRIVATE
    GTest::GTest
    controller
    proto
)

add_test(tape_gtests controller_ut)
