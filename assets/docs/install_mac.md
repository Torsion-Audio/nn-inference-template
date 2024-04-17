# nn-inference-template - macOS Installation Guide

## Copy Model Files

At runtime, the **nn-inference-template** ```vst3```, ```component``` and ```standalone``` attempt to load the neural models from the user's application data directory. Therefore, you must copy the models as follows:

| **source** | ```path/to/release/extras/nn-inference-template/*```        |
|------------|-------------------------------------------------------------|
| **target** | ```~/Library/Application Support/nn-inference-template/*``` |

## Shared libs

The macOS versions of the **nn-inference-template** are all bundled with the dynamic libraries for anira and the inference engines. However, on **Apple Silicon Macs** you need to install the OpenMP library via Homebrew (``` brew install libomp ```)
