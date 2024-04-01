# nn-inference-template - Linux Installation Guide

## Copy Model Files

At runtime, the **nn-inference-template** ```vst3``` and ```standalone``` attempt to load the neural models from the user's application data directory. Therefore, you must copy the models as follows:

| **source** | ```path/to/release/extras/nn-inference-template/*``` |
|------------|------------------------------------------------------|
| **target** | ```~/.config/nn-inference-template/*```              |

## Shared libs

While the **nn-inference-template** ```.vst3``` has the dynamic libraries bundled, the standalone is a pure binary and requires the dynamic libraries to be installed in a specific path. This is either in the following relative path to the binary: ```../lib```. The releases have this folder structure by default. Or you can install the dynamic libs in your system path. In this case, be careful not to overwrite existing libs that may be used by other applications.
