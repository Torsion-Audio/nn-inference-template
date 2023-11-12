#  Neural Network Inference Template for Real-Time Cricital Audio Environments

> This is a companion repository for the presentation **Real-time inference of neural networks: a practical approach for DSP engineers** at the Audio Developer Conference 2023.

> Authors: **Valentin Ackva** & **Fares Schulz**

## Overview

![mockup](assets/graphics/mockup.png)

This repository provides a comprehensive template and practical guide for implementing neural network inference within real-time audio processing environments. For this, we utilize three common inference engines: TensorFlow Lite, LibTorch, and ONNX Runtime.

It covers crucial aspects of maintaining real-time safety and ensuring a seamless signal flow. We delve into the delicate balance between latency, performance, and stability.

## Build instruction

Build with CMake
```bash
# clone the repository
git clone https://github.com/Torsion-Audio/nn-inference-template/
cd nn-inference-template/

# initialize and set up submodules
git submodule update --init --recursive

# use cmake to build debug
cmake . -B cmake-build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --config Debug

# use cmake to build release
cmake . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --config Release
```

## Unit Test / Benchmark

To benchmark your plugin across different audio configuratioans and inference backends, you can use our test:

```bash
# make shure all submodules are installed

# use cmake to build the test
make . -B cmake-build-release -DCMAKE_BUILD_TYPE=Release
cmake --build cmake-build-release --target Test_NN_Inference_Template --config Release

# start the test
cd cmake-build-release
ctest -VV
```

Once the the test finished, all test details can be found: `cmake-build-release\Test\Temporary\LastTest.log`
