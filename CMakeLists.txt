cmake_minimum_required(VERSION 3.22)
project(IPMutex)

#Set Language settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setting for test
find_package(Catch2 CONFIG REQUIRED)
set(CMAKE_CXX_FLAGS "-fcoverage-mapping -fprofile-instr-generate")

#Example target
add_executable(
        example
        main.cpp
        src/IPMutex.h
)

#Add Test target
add_executable(
        tests
        src/IPMutex.h
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
enable_testing()
add_test(NAME tests COMMAND tests)