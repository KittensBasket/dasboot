add_subdirectory(container)
add_subdirectory(coordinator)
add_subdirectory(daemon)

target_link_libraries(coordinator PUBLIC container ProtobufMessages)
# target_link_libraries(daemon PUBLIC coordinator)
