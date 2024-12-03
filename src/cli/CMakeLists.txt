set(SOURCES cli.cpp)
set(HEADERS cli.hpp)
add_library(cli ${SOURCES} ${HEADERS})
target_include_directories(cli PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
