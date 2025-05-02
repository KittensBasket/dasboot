add_subdirectory(cru)
add_subdirectory(cli)
add_subdirectory(app)
add_subdirectory(proto)
add_subdirectory(controller)

add_executable(dasboot app/main.cpp)
target_link_libraries(dasboot PUBLIC cli)
