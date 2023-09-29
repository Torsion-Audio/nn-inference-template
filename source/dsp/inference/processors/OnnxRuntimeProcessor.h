//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H

#include <JuceHeader.h>
#include "../utils/InferenceConfig.h"
#include "onnxruntime_cxx_api.h"

class OnnxRuntimeProcessor {
public:
    OnnxRuntimeProcessor();
    ~OnnxRuntimeProcessor();

    void setModelInputSize(int size);
    void process(juce::AudioBuffer<float>& buffer);

private:
    void loadModel();

    Ort::Env env;
    Ort::RunOptions runOptions;
    Ort::Session session;

    int modelInputSize = MODEL_INPUT_SIZE;

    std::vector<float> onnxInputData;
    std::vector<float> onnxOutputData;
};


#endif //NN_INFERENCE_TEMPLATE_ONNXRUNTIMEPROCESSOR_H
