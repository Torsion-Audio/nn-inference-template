#ifndef NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H

#include <JuceHeader.h>
#include "../InferenceConfig.h"
#include "onnxruntime_cxx_api.h"

class OnnxRuntimeProcessor {
public:
    OnnxRuntimeProcessor();
    ~OnnxRuntimeProcessor();

    void prepareToPlay();
    void processBlock(std::array<float, MODEL_INPUT_SIZE_BACKEND>& input, std::array<float, MODEL_OUTPUT_SIZE_BACKEND>& output);

private:
    std::string filepath = MODELS_PATH_ONNX;
    std::string modelname = MODEL_ONNX;
#ifdef _WIN32
    std::string modelpathStr = filepath + modelname;
    std::wstring modelpath = std::wstring(modelpathStr.begin(), modelpathStr.end());
#else
    std::string modelpath = filepath + modelname;
#endif

    Ort::Env env;
    Ort::MemoryInfo memory_info;
    Ort::AllocatorWithDefaultOptions ort_alloc;
    Ort::Session session;

    std::array<int64_t, 3> inputShape;
    std::array<const char *, 1> inputNames;

    std::array<const char *, 1> outputNames;
    // Define output tensor vector
    std::vector<Ort::Value> outputTensors;
};

#endif //NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H
