cmake_minimum_required(VERSION 3.22)
project(IPMutex)

#Set Language settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Example target
add_executable(
        example
        src/main.cpp
        src/ipm/IPMutex.h
)