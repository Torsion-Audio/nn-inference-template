cmake_minimum_required(VERSION 3.16)
project(minimal-onnxruntime C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimal-onnxruntime
  minimal-onnxruntime.cpp
)

target_link_libraries(minimal-onnxruntime
  onnxruntime
)