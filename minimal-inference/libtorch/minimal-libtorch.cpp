/* ==========================================================================

Minimal LibTorch example from https://pytorch.org/tutorials/advanced/cpp_export.html
Licence: modified BSD

========================================================================== */

#include <torch/script.h>
#include <torch/torch.h>
#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {

    std::cout << "Minimal LibTorch example:" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    const int batchSize = 2;
    const int modelInputSize = 150;
    const int modelOutputSize = 1;

    std::string filepath = MODELS_PATH_PYTORCH;
    const std::string modelpath = filepath + "model_0/model_0-minimal.pt";

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
    const int inputSize = batchSize * modelInputSize;
    float inputData[inputSize];
    for (int i = 0; i < inputSize; i++) {
        inputData[i] = i * 0.001f;
    }

    // Create input tensor object from input data values and reshape
    torch::Tensor inputTensor = torch::from_blob(&inputData, { batchSize, 1, modelInputSize });

    std::cout << "Input shape 0: " << inputTensor.sizes()[0] << '\n';
    std::cout << "Input shape 1: " << inputTensor.sizes()[1] << '\n';
    std::cout << "Input shape 2: " << inputTensor.sizes()[2] << '\n';

    // Create IValue vector for input of interpreter
    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(inputTensor);

    // Execute inference
    at::Tensor outputTensor = module.forward(inputs).toTensor();

    std::cout << "Output shape 0: " << outputTensor.sizes()[0] << '\n';
    std::cout << "Output shape 1: " << outputTensor.sizes()[1] << '\n';

    // Extract the output tensor data
    const int outputSize = batchSize * modelOutputSize;
    float outputData[outputSize];

    for (int i = 0; i < outputSize; i++) {
        outputData[i] = outputTensor[i].item().toFloat();
        std::cout << "Output data [" << i << "]: " << outputData[i] << std::endl;
    }

    return 0;
}