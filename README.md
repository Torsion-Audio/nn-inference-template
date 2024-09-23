#  A Template Audio Plugin for Real-time Neural Network Inference

> This repository was started as a companion repository to the talk **Real-time inference of neural networks: a practical approach for DSP engineers** at the Audio Developer Conference 2023. The video can be found [here](https://www.youtube.com/watch?v=z_RKgHU59r0).

> Since the conference, we have continued to refine and extend the codebase. For more flexible and easier use of the inference architecture, we have consolidated this work into a library called [anira](https://github.com/tu-studio/anira), which is now used in this repository. For those interested in the state of the repository as presented at ADC23, it can be found under the tag [ADC23-talk](https://github.com/Torsion-Audio/nn-inference-template/tree/ADC23-talk).

> Authors: **Valentin Ackva** & **Fares Schulz**

## Overview

![mockup](assets/graphics/mockup.png)

![Build Status](https://github.com/Torsion-Audio/nn-inference-template/actions/workflows/build.yml/badge.svg)

This repository provides a comprehensive JUCE plugin template that demonstrates the use of [anira](https://github.com/tu-studio/anira) to implement neural network inference in real-time audio applications. In this template, we use all three inference engines currently supported by the library:

- TensorFlow Lite
- LibTorch
- ONNXRuntime.

[anira](https://github.com/tu-studio/anira), an architecture for neural network inference in real-time audio applications, covers all critical aspects of ensuring real-time safety and seamless signal flow for real-time inference. Detailed information can be found on the library's [github repo](https://github.com/tu-studio/anira).

## Build instruction

**Build with CMake**

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

To run the plugin or standalone application with the default model, you need to copy the model files to the correct location. The application will attempt to load the neural models from the user's application data directory at runtime. Therefore, you must copy the folder ```./modules/GuitarLSTM/*``` to the following locations, depending on your operating system:

Linux: ```~/.config/nn-inference-template/GuitarLSTM/*```

macOS: ```~/Library/Application Support/nn-inference-template/GuitarLSTM/*```

Windows: ```%APPDATA%\nn-inference-template\GuitarLSTM\*```


## Install from release

To install the plugin, please follow the instructions provided for your operating system:

| [Linux](assets/docs/install_linux.md) | [macOS](assets/docs/install_mac.md) | [Windows](assets/docs/install_win.md) |
|---|---|---|

## Unit Test / Benchmark

The previous unit test for benchmarking the plugin performance across different audio configurations and inference backends is replaced by the benchmarking options within [anira](https://github.com/tu-studio/anira). These new benchmarks have been improved in many ways and provide a range of simple to complex benchmarks that can be used to compare the inference time for different models, inference engines, and audio configurations.

## Licenses

The primary license for the code of this project is the MIT license, but be aware of the licenses of the submodules:


 - The *anira* library is licensed under the [Apache 2.0](https://github.com/tensorflow/tensorflow/blob/master/LICENSE)
 - The *GuitarLSTM* fork is licensed under the [GPLv3](https://github.com/GuitarML/GuitarLSTM/blob/main/LICENSE.txt)
 - The *JUCE* library is licensed under the [JUCE License](https://github.com/juce-framework/JUCE/blob/master/LICENSE.md)
 - The *ONNXRuntime* library is licensed under the [MIT](https://github.com/microsoft/onnxruntime/blob/main/LICENSE)
 - The *Libtorch* library is licensed under the [Modified BSD](https://github.com/pytorch/pytorch/blob/main/LICENSE)
 - The *TensorflowLite* library is licensed under the [Apache 2.0](https://github.com/tensorflow/tensorflow/blob/master/LICENSE)
 - All other code within this project is licensed under the MIT License.
