# source:

set(SOURCES controller.cpp)
set(HEADERS controller.hpp)
add_library(controller ${SOURCES} ${HEADERS})
target_link_libraries(controller PUBLIC ProtobufMessages)
add_dependencies(controller ProtobufMessages)

# tests:

add_executable(controller_ut controller_ut.cpp)

target_link_libraries(
    controller_ut
    PRIVATE
    GTest::gtest
    controller
    ProtobufMessages
)

add_test(tape_gtests controller_ut)
