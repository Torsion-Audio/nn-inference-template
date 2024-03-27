# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

juce_add_binary_data(BinaryData SOURCES
        graphics/background.png
        graphics/texture_overlay.png

        graphics/tflite_backend.png
        graphics/libtorch_backend.png
        graphics/onnx_backend.png

        graphics/tflite_font.svg
        graphics/libtorch_font.svg
        graphics/onnx_font.svg

        graphics/highlight.png
)

# enable position independent code because otherwise the static binary library cannot be linked into a shared library
set_target_properties(BinaryData PROPERTIES POSITION_INDEPENDENT_CODE ON)