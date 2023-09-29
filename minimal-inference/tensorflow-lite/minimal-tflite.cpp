/* ==========================================================================

Minimal TensorFlow Lite example from https://www.tensorflow.org/lite/guide/inference
Licence: Apache 2.0

========================================================================== */

#include <cstdio>
#include <iostream>
#include <vector>
#include "tensorflow/lite/c_api.h"

#define TFLITE_MINIMAL_CHECK(x)                              \
    if (!(x)) {                                                \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1);                                                 \
    }

int main(int argc, char* argv[]) {

    std::string filepath = MODELS_PATH;
    std::string modelpath = filepath + "/model.tflite";

    TfLiteModel* model = TfLiteModelCreateFromFile(modelpath.c_str());
    TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();

    // Create the interpreter.
    TfLiteInterpreter* interpreter = TfLiteInterpreterCreate(model, options);

    // Allocate tensors and populate the input tensor data.
    TfLiteInterpreterAllocateTensors(interpreter);

    TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);

    std::vector<float> input;

    for (int i = 0; i < 150; i++) {
        input.push_back(i*0.001f);
    }

    TfLiteTensorCopyFromBuffer(input_tensor, input.data(), input.size() * sizeof(float));

    // Execute inference.
    TfLiteInterpreterInvoke(interpreter);

    // Extract the output tensor data.
    const TfLiteTensor* output_tensor = TfLiteInterpreterGetOutputTensor(interpreter, 0);

    std::vector<float> output;
    output.resize(1);

    TfLiteTensorCopyToBuffer(output_tensor, output.data(), output.size() * sizeof(float));

    for (int i = 0; i < 1; i++) {
        std::cout << "Output: " << output[i] << std::endl;
    }

    // Dispose of the model and interpreter objects.
    TfLiteInterpreterDelete(interpreter);
    TfLiteInterpreterOptionsDelete(options);
    TfLiteModelDelete(model);
    
    return 0;
}
