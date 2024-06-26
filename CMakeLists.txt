cmake_minimum_required(VERSION 3.20)

include_guard()

project(XorString)

set(CMAKE_CXX_STANDARD 26)

add_library(XorString INTERFACE)
target_include_directories(XorString INTERFACE "${PROJECT_SOURCE_DIR}/Include")
target_compile_features(XorString INTERFACE cxx_std_20)

if (PROJECT_IS_TOP_LEVEL)
    enable_testing()
    add_subdirectory("Test")
endif ()