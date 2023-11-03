#ifndef NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H

#include <JuceHeader.h>
#include "../InferenceConfig.h"
#include <torch/script.h>

class LibtorchProcessor {
public:
    LibtorchProcessor();
    ~LibtorchProcessor();

    void prepareToPlay();
    void processBlock(NNInferenceTemplate::InputArray& input, NNInferenceTemplate::OutputArray& output);

private:
    std::string filepath = MODELS_PATH_PYTORCH;
    std::string modelname = MODEL_LIBTORCH;

    torch::jit::script::Module module;

    at::Tensor inputTensor;
    at::Tensor outputTensor;
    std::vector<torch::jit::IValue> inputs;
};


#endif //NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
