/* ==========================================================================

Minimal TensorFlow Lite example from https://www.tensorflow.org/lite/guide/inference
Licence: Apache 2.0

========================================================================== */

#include <cstdio>
#include <iostream>
#include "tensorflow/lite/c_api.h"

#define TFLITE_MINIMAL_CHECK(x)                              \
    if (!(x)) {                                                \
        fprintf(stderr, "Error at %s:%d\n", __FILE__, __LINE__); \
        exit(1);                                                 \
    }

int main(int argc, char* argv[]) {

    std::string filepath = MODELS_PATH_TENSORFLOW;
    std::string modelpath = filepath + "model_0/model_0.tflite";

    // Load model
    TfLiteModel* model = TfLiteModelCreateFromFile(modelpath.c_str());

    // Create the interpreter
    TfLiteInterpreterOptions* options = TfLiteInterpreterOptionsCreate();
    TfLiteInterpreter* interpreter = TfLiteInterpreterCreate(model, options);

    // Allocate memory for all tensors
    TfLiteInterpreterAllocateTensors(interpreter);

    // Get input tensor
    TfLiteTensor* input_tensor = TfLiteInterpreterGetInputTensor(interpreter, 0);

    // Fill input tensor with data
    const int inputSize = 150;
    float inputData[inputSize];
    for (int i = 0; i < inputSize; i++) {
        inputData[i] = i * 0.001f;
    }
    TfLiteTensorCopyFromBuffer(input_tensor, &inputData, inputSize * sizeof(float));

    // Execute inference.
    TfLiteInterpreterInvoke(interpreter);

    // Get output tensor
    const TfLiteTensor* output_tensor = TfLiteInterpreterGetOutputTensor(interpreter, 0);

    // Extract the output tensor data
    const int outputSize = 1;
    float outputData[outputSize];
    TfLiteTensorCopyToBuffer(output_tensor, &outputData, outputSize * sizeof(float));

    for (int i = 0; i < 1; i++) {
        std::cout << "Output: " << outputData[i] << std::endl;
    }

    // Dispose of the model and interpreter objects.
    TfLiteInterpreterDelete(interpreter);
    TfLiteInterpreterOptionsDelete(options);
    TfLiteModelDelete(model);
    
    return 0;
}
