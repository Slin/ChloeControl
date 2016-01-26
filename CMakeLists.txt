cmake_minimum_required(VERSION 3.4)

add_subdirectory("vendor/enet")

project(ChloeControl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        main.cpp
        CLCNetwork.cpp
        CLCInput.cpp)

if(APPLE)
    set(SOURCE_FILES ${SOURCE_FILES}
            vendor/hidapi/mac/hid.c)

    #find_library(COCOA_LIBRARY Cocoa)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(IOKIT_LIBRARY IOKit)

    #set(COMPILE_OPTIONS "-xobjective-c++")

        set(LINK_LIBRARIES ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY})
endif()

set(LINK_LIBRARIES ${LINK_LIBRARIES} enet)

add_executable(ChloeControl ${SOURCE_FILES})
target_link_libraries(ChloeControl ${LINK_LIBRARIES})
include_directories(
        "vendor/enet/include"
        "vendor/hidapi/hidapi")