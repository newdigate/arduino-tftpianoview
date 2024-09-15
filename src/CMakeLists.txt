cmake_minimum_required(VERSION 3.5)
project(arduino_tftpianoview C CXX)
set(arduino_tftpianoview_VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)

option(OPT_BUILD_FOR_LINUX "Build for linux emulator" ON)
if (OPT_BUILD_FOR_LINUX)
	set(BUILD_FOR_LINUX 1)
endif()

set(SOURCE_FILES
		TFTPianoDisplay.cpp)

set(HEADER_FILES
		TFTPianoDisplay.h)

if (DEFINED BUILD_FOR_LINUX)
	include(CMakeLists.linux.cmake.in)
else()
	include(CMakeLists.teensy.cmake.in)
endif()