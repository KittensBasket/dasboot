# source:

set(HEADERS daemon.hpp)
set(SOURCES daemon.cpp)
add_library(daemon ${SOURCES} ${HEADERS})
apply_compile_flags(daemon)
target_link_libraries(daemon
    PUBLIC
    cru
    cppzmq
    ProtobufMessages
    container
    coordinator
)

# tests:

#add_executable(daemon_ut daemon_ut.cpp)
#apply_compile_flags(daemon_ut)

#target_link_libraries(
#    daemon_ut
#    PRIVATE
#    GTest::gtest
#    daemon
#)

#gtest_discover_tests(daemon_ut)
