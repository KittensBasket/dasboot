# source:

find_package(nlohmann_json 3.11.3 REQUIRED)

set(SOURCES controller.cpp)
set(HEADERS controller.hpp)
add_library(controller ${SOURCES} ${HEADERS})
target_include_directories(controller PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
    controller 
    PRIVATE 
    nlohmann_json::nlohmann_json
    )

# tests:

find_package(GTest REQUIRED)

add_executable(controller_ut controller_ut.cpp)

target_link_libraries(
    controller_ut
    PRIVATE
    GTest::GTest
    controller
)

add_test(tape_gtests controller_ut)
