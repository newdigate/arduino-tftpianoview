cmake_minimum_required(VERSION 3.5)
project(arduino_tftpianoview CXX)

set(CMAKE_CXX_STANDARD 11)
IF (NOT DEFINED BUILD_FOR_TEENSY)
    set(BUILD_FOR_LINUX 1)
    include(cmake_declare_and_fetch.cmake.in)
    DeclareAndFetch(teensy_x86_stubs                    git@github.com:/newdigate/teensy-x86-stubs.git feature/shared_serial src)
    DeclareAndFetch(teensy_gfx                          git@github.com:/newdigate/teensy-gfx.git noinstall src)
    DeclareAndFetch(teensy_st7735_linux_stubs           git@github.com:/newdigate/teensy-st7735-linux-stubs.git noinstall src)
ENDIF ()

add_subdirectory(src)