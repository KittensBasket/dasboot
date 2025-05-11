# source:

set(SOURCES controller.cpp)
set(HEADERS controller.hpp)
add_library(controller ${SOURCES} ${HEADERS})
apply_compile_flags(controller)
target_link_libraries(controller PUBLIC ProtobufMessages)
add_dependencies(controller ProtobufMessages)
target_link_libraries(controller PRIVATE libzmq)

# tests:

add_executable(controller_ut controller_ut.cpp)
apply_compile_flags(controller_ut)

target_link_libraries(
    controller_ut
    PRIVATE
    GTest::gtest
    controller
    ProtobufMessages
    libzmq
    cli
)

gtest_discover_tests(controller_ut)
