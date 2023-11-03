#ifndef NN_INFERENCE_TEMPLATE_TFLITEPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_TFLITEPROCESSOR_H

#include <JuceHeader.h>
#include "../InferenceConfig.h"
#include "tensorflow/lite/c_api.h"

class TFLiteProcessor {
public:
    TFLiteProcessor();
    ~TFLiteProcessor();

    void prepareToPlay();
    void processBlock(NNInferenceTemplate::InputArray& input, NNInferenceTemplate::OutputArray& output);

private:
    std::string filepath = MODELS_PATH_TENSORFLOW;
    std::string modelname = MODEL_TFLITE;
#ifdef _WIN32
    std::string modelpathStr = filepath + modelname;
    std::wstring modelpath = std::wstring(modelpathStr.begin(), modelpathStr.end());
#else
    std::string modelpath = filepath + modelname;
#endif

    TfLiteModel* model;
    TfLiteInterpreterOptions* options;
    TfLiteInterpreter* interpreter;

    TfLiteTensor* inputTensor;
    const TfLiteTensor* outputTensor;
};

#endif //NN_INFERENCE_TEMPLATE_TFLITEPROCESSOR_H
