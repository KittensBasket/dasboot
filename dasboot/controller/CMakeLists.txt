# source:

find_package(protobuf REQUIRED)
set(PROTO_SOURCES message.proto)
add_library(proto ${PROTO_SOURCES})
target_include_directories(proto INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(
    proto
    PUBLIC
    protobuf::libprotobuf
)
protobuf_generate(TARGET proto LANGUAGE cpp)

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
    proto
    controller
)

add_test(tape_gtests controller_ut)
