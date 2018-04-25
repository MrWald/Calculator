cmake_minimum_required(VERSION 3.10)
project(Calculator)

set(CMAKE_CXX_STANDARD 11)

include_directories(Calculator)

add_executable(Calculator
        Calculator/AComplex.h
        Calculator/Double.h
        Calculator/Evaluator.h
        Calculator/Functions.h
        Calculator/HashMap.h
        Calculator/Exception.h
        Calculator/Iterator.h
        Calculator/Exception.cpp
        Calculator/main.cpp
        Calculator/AComplex.cpp
        Calculator/Double.cpp Calculator/Functions.cpp)
