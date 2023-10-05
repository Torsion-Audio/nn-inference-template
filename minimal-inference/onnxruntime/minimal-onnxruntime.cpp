/* ==========================================================================

Minimal OnnxRuntime example from https://onnxruntime.ai
Licence: MIT

========================================================================== */

#include "onnxruntime_cxx_api.h"
#include <iostream>

int main(int argc, char* argv[]) {

    // Load the model and create InferenceSession
    Ort::Env env;

    std::string filepath = MODELS_PATH;
    std::string modelpath = filepath + "/model-tensorflow2.onnx";

#ifdef _WIN32
    std::wstring modelWideStr = std::wstring(modelpath.begin(), modelpath.end());
    const wchar_t* modelWideCStr = modelWideStr.c_str();
    Ort::Session session(env, modelWideCStr, Ort::SessionOptions{nullptr });
#else
    Ort::Session session(env, modelpath.c_str(), Ort::SessionOptions{ nullptr });
#endif

    std::vector<float> input;
    for (int i = 0; i < 150; i++) {
        input.push_back(i*0.001f);
    }

    std::vector<float> output;
    output.resize(1);

    Ort::AllocatorWithDefaultOptions ort_alloc;

    Ort::AllocatedStringPtr inputName = session.GetInputNameAllocated(0, ort_alloc);
    Ort::AllocatedStringPtr outputName = session.GetOutputNameAllocated(0, ort_alloc);
    const std::array<const char *, 1> inputNames = {(char*) inputName.get()};
    const std::array<const char *, 1> outputNames = {(char*) outputName.get()};

    //process
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    // define shape
    std::array<int64_t, 3> inputShape = {1, 150, 1};

    const Ort::Value inputTensor = Ort::Value::CreateTensor<float>  (memory_info,
                                                                    input.data(),
                                                                    input.size(),
                                                                    inputShape.data(),
                                                                    inputShape.size());

    try {
        auto outputTensor = session.Run(Ort::RunOptions{nullptr}, 
        inputNames.data(), 
        &inputTensor, 
        inputNames.size(), 
        outputNames.data(), 
        outputNames.size());
        output[0] = outputTensor[0].GetTensorMutableData<float>()[0];
        std::cout << "Output: " << output[0] << std::endl;
    } catch (Ort::Exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}