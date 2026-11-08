cmake_minimum_required(VERSION 3.14)
project(myregex VERSION 1.0.0 LANGUAGES CXX)

add_library(myregex INTERFACE)
add_library(myregex::myregex ALIAS myregex)  # namespace recomendado

target_include_directories(myregex INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(myregex INTERFACE cxx_std_17)