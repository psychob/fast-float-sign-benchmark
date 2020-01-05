cmake_minimum_required(VERSION 3.15)
project(fast_float_sign_extract)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

set(CMAKE_CXX_STANDARD 14)
add_executable(fast_float_sign_extract main.cpp ring_buffer.hpp single.cpp unrolled.cpp)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
add_subdirectory(vendor/google/benchmark)
add_subdirectory(vendor/google/googletest)

target_link_libraries(fast_float_sign_extract benchmark)
target_link_libraries(fast_float_sign_extract gtest)
