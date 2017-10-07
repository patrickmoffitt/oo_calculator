cmake_minimum_required(VERSION 3.8)
project(9_calculator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp calculator.h calculator.cpp calculator_testing.h calculator_testing.cpp
        basic_argv_parse.h basic_argv_parse.cpp)
add_executable(9_calculator ${SOURCE_FILES})