cmake_minimum_required(VERSION 3.10)

project(dllist VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(dllist
  main.cpp
  dllist.cpp
)
