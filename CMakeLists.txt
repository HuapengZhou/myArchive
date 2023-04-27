cmake_minimum_required(VERSION 3.24)
project(141a_Assignment5)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(141a_Assignment5
        Archive.cpp
        Archive.hpp
        main.cpp
        Testable.hpp
        Testing.hpp
        Timer.hpp
        Tracker.hpp)
