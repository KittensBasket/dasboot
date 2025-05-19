# source:

set(SOURCES imagemanager.cpp)
set(HEADERS imagemanager.hpp)
add_library(imagemanager ${SOURCES} ${HEADERS})
apply_compile_flags(imagemanager)
target_link_libraries(imagemanager PUBLIC curl archive)

# tests:

add_executable(imagemanager_ut imagemanager_ut.cpp)
apply_compile_flags(imagemanager_ut)

target_link_libraries(
    imagemanager_ut
    PRIVATE
    GTest::gtest
    imagemanager
    archive
    curl
)

gtest_discover_tests(imagemanager_ut)
