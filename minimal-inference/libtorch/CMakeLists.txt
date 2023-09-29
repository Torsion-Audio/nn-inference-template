cmake_minimum_required(VERSION 3.16)
project(minimal-libtorch C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimal-libtorch
  minimal-libtorch.cpp
)

target_link_libraries(minimal-libtorch
  torch
)
