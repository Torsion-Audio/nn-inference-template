/* ==========================================================================

Minimal LibTorch example from https://pytorch.org/tutorials/advanced/cpp_export.html
Licence: modified BSD

========================================================================== */

#include <torch/script.h>

#include <iostream>
#include <memory>

int main(int argc, const char* argv[]) {

    std::string filepath = MODELS_PATH;
    const std::string modelpath = filepath + "/model.pt";

    torch::jit::script::Module module;
    try {
        module = torch::jit::load(modelpath);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        std::cout << e.what() << std::endl;
        return -1;
    }

    const int inputSize = 150;
    std::array<float, inputSize> inputData;
    for (int i = 0; i < inputSize; i++) {
        inputData[i] = i * 0.1f;
    }

    at::Tensor frame = torch::from_blob(inputData.data(), {inputSize}).reshape({1, 1, inputSize});

    std::vector<torch::jit::IValue> inputs;
    inputs.push_back(frame);

    auto output = module.forward(inputs).toTensor();

    std::cout << "Output: " << output.item().toFloat() << '\n';

    return 0;
}