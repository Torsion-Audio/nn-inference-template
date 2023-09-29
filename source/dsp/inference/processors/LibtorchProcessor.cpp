//
// Created by valentin.ackva on 29.09.2023.
//

#include "LibtorchProcessor.h"

LibtorchProcessor::LibtorchProcessor() {
    loadModel();
}

LibtorchProcessor::~LibtorchProcessor() {

}

void LibtorchProcessor::setModelInputSize(int size) {
    juce::ignoreUnused(size);
}

void LibtorchProcessor::process(juce::AudioBuffer<float> &buffer) {
    juce::ignoreUnused(buffer);
}
