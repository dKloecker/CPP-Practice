cmake_minimum_required(VERSION 3.10)

project(expected VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(expected
  main.cpp
  expected.cpp
)
