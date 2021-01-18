cmake_minimum_required(VERSION 3.10)
project(arduino_tftpianoview_example)
set(CMAKE_CXX_STANDARD 11)
include(../CMakeLists.cmake.in)
include(../examples.cmake.in)

teensy_add_executable(arduino_tftpianoview_example
        basic.cpp)