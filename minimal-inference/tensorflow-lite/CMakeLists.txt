cmake_minimum_required(VERSION 3.16)
project(minimal-tflite C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimal-tflite
  minimal-tflite.cpp
)

target_link_libraries(minimal-tflite
  tensorflowlite_c
)