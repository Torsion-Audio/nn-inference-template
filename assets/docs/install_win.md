# nn-inference-template - Windows Installation Guide

**Copy Model Files**

At runtime, the **nn-inference-template** ```.vst3``` and ```.exe``` attempt to load the neural models from the user's application data directory. Therefore, you must copy the models as follows:

| **source** | ```path\to\release\extras\nn-inference-template\*```   |
|------------|--------------------------------------------------------|
| **target** | ```%APPDATA%\nn-inference-template\*```                |

**Shared libs**

At runtime, the **nn-inference-template** ```.vst3``` and ```.exe``` will try to load the shared libraries located in ```lib\```. Windows will automatically search for these libs next to your executable. 
If you are running the **nn-inference-template** as an  ```.exe```,  place the libs next to it.
If you are using it as a ```.vst3``` , the plugin will search for the libs next to the executable of your DAW.

> You can also place the libs in your system directory ```C:\Windows\System32```, but be cautious not to overwrite existing shared libraries.

