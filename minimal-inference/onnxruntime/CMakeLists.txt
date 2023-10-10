cmake_minimum_required(VERSION 3.16)
project(minimal-onnxruntime C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(minimal-onnxruntime
  minimal-onnxruntime.cpp
)

target_link_libraries(minimal-onnxruntime
  onnxruntime
)

target_compile_definitions(minimal-onnxruntime PUBLIC MODELS_PATH_TENSORFLOW="${CMAKE_SOURCE_DIR}/modules/GuitarLSTM/tensorflow-version/models/")
target_compile_definitions(minimal-onnxruntime PUBLIC MODELS_PATH_PYTORCH="${CMAKE_SOURCE_DIR}/modules/GuitarLSTM/pytorch-version/models/")