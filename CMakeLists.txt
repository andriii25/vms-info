cmake_minimum_required(VERSION 3.7)
project(vms_info)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES vms_info.c color16.h color16.c lodepng.c lodepng.h)
add_executable(vms-info ${SOURCE_FILES})