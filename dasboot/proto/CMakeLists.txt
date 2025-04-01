# source:

find_package(protobuf REQUIRED)
set(PROTO_SOURCES messages.proto)
add_library(proto ${PROTO_SOURCES})
target_include_directories(proto INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(
    proto
    PUBLIC
    protobuf::libprotobuf
)
protobuf_generate(TARGET proto LANGUAGE cpp)
