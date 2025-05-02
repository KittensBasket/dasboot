# source:

set(SOURCES cru.cpp)
set(HEADERS cru.hpp)
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

add_test(cru_gtests cru_ut)
