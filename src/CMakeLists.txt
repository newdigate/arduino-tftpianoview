cmake_minimum_required(VERSION 3.10)
project(arduino_tftpianoview C CXX)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
		TFTPianoDisplay.cpp)

set(HEADER_FILES
		TFTPianoDisplay.h)

if (DEFINED BUILD_FOR_LINUX)
	include(CMakeLists.linux.cmake.in)
else()
	include(CMakeLists.teensy.cmake.in)
endif()
