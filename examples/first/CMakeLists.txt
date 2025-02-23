cmake_minimum_required(VERSION 3.10)
project(arduino_tftpianoview_example_first C CXX)
set(CMAKE_CXX_STANDARD 11)

if (DEFINED BUILD_FOR_LINUX)
    message(STATUS "BUILD_FOR_LINUX: Build the example for linux is ON")
    include(../../cmake/opengl.cmake.in)
    include_directories(../../src)
    add_executable(first first.cpp)

    target_link_libraries(first teensy_x86_stubs)
    target_link_libraries(first teensy_st7735_linux_stubs)
    target_link_libraries(first teensy_st7735_linux_opengl ${LIBS})
    target_link_libraries(first arduino_tftpianoview)
else()
    teensy_include_directories(../../src)
    teensy_add_executable(arduino_tftpianoview_example_first first.cpp)
    teensy_target_link_libraries(arduino_tftpianoview_example_first arduino_tftpianoview TeensyGFX ST7735_t3 SD SdFat SPI cores)
endif()



