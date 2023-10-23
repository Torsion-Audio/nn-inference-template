#include "LibtorchProcessor.h"

LibtorchProcessor::LibtorchProcessor() {
    try {
        module = torch::jit::load(filepath + modelname);
    }
    catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
        std::cout << e.what() << std::endl;
    }
}

LibtorchProcessor::~LibtorchProcessor() {
}

void LibtorchProcessor::prepareToPlay() {
    inputs.push_back(torch::zeros(MODEL_INPUT_SHAPE_LIBTORCH));
}

void LibtorchProcessor::processBlock(std::array<float, MODEL_INPUT_SIZE_BACKEND>& input, std::array<float, MODEL_OUTPUT_SIZE_BACKEND>& output) {

    // Create input tensor object from input data values and shape
    frame = torch::from_blob(input.data(), MODEL_INPUT_SIZE_BACKEND).reshape(MODEL_INPUT_SHAPE_LIBTORCH);
    inputs[0] = frame;

    // Run inference
    at::Tensor outputTensor = module.forward(inputs).toTensor();

    // Extract the output tensor data
    for (size_t i = 0; i < MODEL_OUTPUT_SIZE_BACKEND; i++) {
        output[i] = outputTensor[(int64_t) i].item<float>();
    }
}