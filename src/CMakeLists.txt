cmake_minimum_required(VERSION 3.5)

project(arduino_tftpianoview C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(teensy_x86_stubs)
include_directories(${teensy_x86_stubs_INCLUDE_DIR})

find_package(teensy_st7735_linux_stubs)
include_directories(${teensy_st7735_linux_stubs_INCLUDE_DIR})

add_library(common STATIC
		TFTPianoDisplay.h
		TFTPianoDisplay.cpp)
