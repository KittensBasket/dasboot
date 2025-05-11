# source:

set(HEADERS common.hpp os.hpp)
set(SOURCES common.cpp os.cpp)
add_library(cru ${SOURCES} ${HEADERS})
apply_compile_flags(cru)

# tests:

add_executable(cru_ut cru_ut.cpp)
apply_compile_flags(cru_ut)

target_link_libraries(
    cru_ut
    PRIVATE
    GTest::gtest
    cru
)

add_executable(fs_ut fs_ut.cpp)
apply_compile_flags(fs_ut)

target_link_libraries(
    fs_ut
    PRIVATE
    GTest::gtest
    cru
)

gtest_discover_tests(cru_ut)
gtest_discover_tests(fs_ut)
