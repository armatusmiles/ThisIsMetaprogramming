cmake_minimum_required(VERSION 2.8)

project(ThisIsMetaprogramming)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} main.cpp metastuff.hpp)
