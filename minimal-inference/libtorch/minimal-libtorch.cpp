/* ==========================================================================

Minimal LibTorch example from https://pytorch.org/tutorials/advanced/cpp_export.html
Licence: modified BSD

========================================================================== */

#include <torch/script.h>
#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {

    std::string filepath = MODELS_PATH_PYTORCH;
    const std::string modelpath = filepath + "model_0/model_0.pt";

    // Load model
    torch::jit::Module module;
    try {
        module = torch::jit::load(modelpath);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        std::cout << e.what() << std::endl;
        return -1;
    }

    // Fill input tensor with data
    const int inputSize = 150;
    float inputData[inputSize];
    for (int i = 0; i < inputSize; i++) {
        inputData[i] = i * 0.001f;
    }

    // Create input tensor object from input data values and reshape
    at::Tensor frame = torch::from_blob(&inputData, {inputSize}).reshape({1, 1, inputSize});

    // Create IValue vector for input of interpreter
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(frame);

    // Execute inference
    at::Tensor output = module.forward(inputs).toTensor();

    // Extract the output tensor data
    int outputSize = 1;
    float outputData[outputSize];
    outputData[0] = output.item().toFloat();

    for (int i = 0; i < outputSize; i++) {
        std::cout << "Output: " << outputData[i] << '\n';
    }

    return 0;
}