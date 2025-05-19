# sources:

set(SOURCES imagemanager.cpp coordinator.hpp)
set(HEADERS imagemanager.hpp coordinator.cpp)
add_library(coordinator ${SOURCES} ${HEADERS})
apply_compile_flags(coordinator)
target_link_libraries(coordinator 
    PUBLIC
    cru
    container
    CURL::libcurl 
    archive
    nlohmann_json
)

# tests:

add_executable(imagemanager_ut imagemanager_ut.cpp)
apply_compile_flags(imagemanager_ut)

target_link_libraries(
    imagemanager_ut
    PRIVATE
    GTest::gtest
    coordinator
    CURL::libcurl
    archive
    nlohmann_json
)

gtest_discover_tests(imagemanager_ut)
