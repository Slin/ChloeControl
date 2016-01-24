cmake_minimum_required(VERSION 3.4)

add_subdirectory("vendor/enet")

project(ChloeControl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(ChloeControl ${SOURCE_FILES})