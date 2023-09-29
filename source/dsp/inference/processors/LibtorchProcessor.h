//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H

#include <JuceHeader.h>

#include <torch/torch.h>
#include <torch/script.h>

class LibtorchProcessor {
public:
    LibtorchProcessor();

    ~LibtorchProcessor();

    void setModelInputSize(int size);

    void process(juce::AudioBuffer<float>& buffer);

private:
    void loadModel() {

    }
};


#endif //NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
