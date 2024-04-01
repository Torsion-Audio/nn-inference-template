# nn-inference-template - macOS Installation Guide

**Copy Model Files**

At runtime, the **nn-inference-template** ```vst3``` and ```standalone``` attempt to load the neural models from the user's application data directory. Therefore, you must copy the models as follows:

| **source** | ```path/to/release/extras/nn-inference-template/*```        |
|------------|-------------------------------------------------------------|
| **target** | ```~/Library/Application Support/nn-inference-template/*``` |
