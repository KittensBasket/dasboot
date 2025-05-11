# Set paths for proto files

set(PROTO_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENERATED_PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${GENERATED_PROTO_SRC_DIR})

set(PROTO_FILES messages.proto)

# Initialize lists for generated files
set(GENERATED_PROTO_SRCS)
set(GENERATED_PROTO_HDRS)

foreach(proto_file ${PROTO_FILES})
    get_filename_component(proto_name ${proto_file} NAME_WE)
    list(APPEND GENERATED_PROTO_SRCS ${GENERATED_PROTO_SRC_DIR}/${proto_name}.pb.cc)
    list(APPEND GENERATED_PROTO_HDRS ${GENERATED_PROTO_SRC_DIR}/${proto_name}.pb.h)

    add_custom_command(
        OUTPUT ${GENERATED_PROTO_SRC_DIR}/${proto_name}.pb.cc
               ${GENERATED_PROTO_SRC_DIR}/${proto_name}.pb.h
        COMMAND ${protobuf_BINARY_DIR}/protoc-3.21.4.0
        ARGS --proto_path=${PROTO_FILES_DIR}
             --cpp_out=${GENERATED_PROTO_SRC_DIR}
             ${proto_file}
        DEPENDS ${proto_file} ${protobuf_BINARY_DIR}/protoc-3.21.4.0
    )
endforeach()

add_library(ProtobufMessages STATIC ${GENERATED_PROTO_SRCS} ${GENERATED_PROTO_HDRS})
# We do not want clang-tidy to check protobuf generated files
set_target_properties(ProtobufMessages PROPERTIES CXX_CLANG_TIDY "")
target_link_libraries(ProtobufMessages PRIVATE protobuf::libprotobuf)
# SYSTEM here is required cause we do not want -Werror to be applied to protobuf headers
target_include_directories(ProtobufMessages SYSTEM PUBLIC ${GENERATED_PROTO_SRC_DIR})
