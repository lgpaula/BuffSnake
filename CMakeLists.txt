cmake_minimum_required(VERSION 3.26)
project(snake)

set(CMAKE_CXX_STANDARD 17)

add_executable(snake main.cpp
        Snake.cpp
        Snake.hpp)
