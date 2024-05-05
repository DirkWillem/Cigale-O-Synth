cmake_minimum_required(VERSION 3.28)
project(cigalespp C CXX ASM)

set(CMAKE_CXX_STANDARD 23)

include(cmake/project_settings.cmake)

add_subdirectory(src)
