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

## Licenses

The primary license for the code of this project is the MIT license, but be aware of the licenses of the submodules:
 - The *GuitarLSTM* fork located at ```modules/GuitarLSTM/``` is licensed under the [GPLv3](https://github.com/GuitarML/GuitarLSTM/blob/main/LICENSE.txt)
 - The *JUCE* library located at ```modules/JUCE/``` is licensed under the [JUCE License](https://github.com/juce-framework/JUCE/blob/master/LICENSE.md)
 - The *ONNXRuntime* library  at ```modules/onnxruntime-1.15.1/``` is licensed under the [MIT](https://github.com/microsoft/onnxruntime/blob/main/LICENSE)
 - The *Libtorch* library at ```modules/libtorch-2.0.1/``` is licensed under the [Modified BSD](https://github.com/pytorch/pytorch/blob/main/LICENSE)
 - The *TensorflowLite* library at ```modules/tensorflowlite-2.14.0``` is licensed under the [Apache 2.0](https://github.com/tensorflow/tensorflow/blob/master/LICENSE)
 - All other code within this project is licensed under the MIT License.
