cmake_minimum_required (VERSION 3.22.1)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(dasboot)

# Find compilers

find_program(CMAKE_CXX_COMPILER NAMES clang++ REQUIRED)
find_program(CMAKE_CXX_COMPILER NAMES clang REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_COMPILER "clang++" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_C_COMPILER "clang" CACHE STRING "C compiler" FORCE)

# ========================================== Start install dependencies ==========================================

include(FetchContent)

# <ZeroMQ>
set(ZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(CPPZMQ_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    zeromq_c
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG        v4.3.5
)

FetchContent_Declare(
    zeromq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG        v4.10.0
)

FetchContent_MakeAvailable(zeromq_c zeromq)

include_directories(${zeromq_c_SOURCE_DIR}/include)
include_directories(${zeromq_SOURCE_DIR}/include)
# </ZeroMQ>

# <Cli11>
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        v2.5.0
)

FetchContent_MakeAvailable(cli11)

include_directories(${cli11_SOURCE_DIR}/include)
# </Cli11>

# <GTest>
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)
# </GTest>

# <Protobuf>
set(protobuf_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(protobuf_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    protobuf
    GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
    GIT_TAG        v21.4
    SOURCE_SUBDIR  cmake
)

FetchContent_MakeAvailable(protobuf)

include_directories(${protobuf_INCLUDE_DIR})
include_directories(${protobuf_SOURCE_DIR}/src)
# </Protobuf>

# <nlohmann_json>
set(json_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(json_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    json 
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

include_directories(${json_SOURCE_DIR}/include)
# </nlohmann_json>

# ========================================== End install dependencies ==========================================

# Start flags

function(apply_compile_flags target)
    if (CMAKE_CXX_COMPILER)
        target_compile_options(${target} PRIVATE
            -Werror
            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            -Wconversion
            -Wctor-dtor-privacy
            -Wenum-compare
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wsign-conversion
            -Wsign-promo
            -Wzero-as-null-pointer-constant
            -Wnonnull
            
            -O3
            #-ffast-math - could not be used here, because json headers include uses of INF

            # For coverage:
            -fprofile-instr-generate
            -fcoverage-mapping
        )
        # For coverage
        add_link_options(-fprofile-instr-generate -fcoverage-mapping)
    endif()
endfunction()

# End flags

# Start clang-tidy

find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)

 if(NOT CLANG_TIDY_COMMAND)
     message(FATAL_ERROR "Clang-tidy is not found!")
 else()
     message(STATUS "Clang-tidy is ON")

     # Google C++ Style
     set(CLANGTIDY_EXTRA_ARGS
         "-checks=google-*"
         "-warnings-as-errors=*"
     )

     set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_COMMAND};
        -p=${CMAKE_BINARY_DIR};
        --header-filter='*/dasboot/*';
        ${CLANGTIDY_EXTRA_ARGS};)
 endif()

# End clang-tidy

include_directories(.)
enable_testing()
add_subdirectory(dasboot)
