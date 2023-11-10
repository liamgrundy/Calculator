cmake_minimum_required(VERSION 3.26)
project(Calculator)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS -O3)

add_executable(Calculator src/main.cpp
        src/parser.cpp
        src/parser.h
        src/log.h
        src/calculator.cpp
        src/calculator.h)
