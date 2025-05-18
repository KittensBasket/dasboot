set(HEADERS coordinator.hpp)
set(SOURCES coordinator.cpp)
add_library(coordinator ${SOURCES} ${HEADERS})
apply_compile_flags(coordinator)

target_link_libraries(
    coordinator
    PUBLIC
    cru
    container
)

