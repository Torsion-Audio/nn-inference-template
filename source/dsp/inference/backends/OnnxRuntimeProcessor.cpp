#include "OnnxRuntimeProcessor.h"

OnnxRuntimeProcessor::OnnxRuntimeProcessor() : memory_info(Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU)), session(env, modelpath.c_str(), Ort::SessionOptions{ nullptr }) {
}

OnnxRuntimeProcessor::~OnnxRuntimeProcessor() {
}

void OnnxRuntimeProcessor::prepareToPlay(float * newModelInputBuffer) {
    modelInputBuffer = newModelInputBuffer;

    inputSize = MODEL_INPUT_SIZE;
    // Define the shape of input tensor
    inputShape = {1, MODEL_INPUT_SIZE, 1};
}

float* OnnxRuntimeProcessor::processBlock() {

    // Create input tensor object from input data values and shape
    const Ort::Value inputTensor = Ort::Value::CreateTensor<float> (memory_info,
                                                                    modelInputBuffer,
                                                                    MODEL_INPUT_SIZE,
                                                                    inputShape.data(),
                                                                    inputShape.size());

    // Get input and output names from model
    Ort::AllocatedStringPtr inputName = session.GetInputNameAllocated(0, ort_alloc);
    Ort::AllocatedStringPtr outputName = session.GetOutputNameAllocated(0, ort_alloc);
    inputNames = {(char*) inputName.get()};
    outputNames = {(char*) outputName.get()};

    try {
        // Run inference
        outputTensors = session.Run(Ort::RunOptions{nullptr}, inputNames.data(), &inputTensor, inputNames.size(), outputNames.data(), outputNames.size());
    }
    catch (Ort::Exception &e) {
        std::cout << e.what() << std::endl;
    }
    
    float* outputData[MODEL_OUTPUT_SIZE];

    // Extract the output tensor data
    outputData[0] = outputTensors[0].GetTensorMutableData<float>();

    for (size_t i = 0; i < MODEL_OUTPUT_SIZE; i++) {
        std::cout << "Output of TensorFlow model: " << *outputData[i] << std::endl;
    }
    return nullptr;
}
