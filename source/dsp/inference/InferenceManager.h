//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
#define NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H

#include <JuceHeader.h>

#include "InferenceThread.h"
#include "../utils/RingBuffer.h"

class InferenceManager {
public:
    InferenceManager();

    void prepare(const juce::dsp::ProcessSpec& spec);
    void processBlock(juce::AudioBuffer<float>& buffer);

    void parameterChanged(const juce::String &parameterID, float newValue);
    int getLatency() const;

private:
    void processOutput(juce::AudioBuffer<float>& buffer);
    void calculateLatency(int maxSamplesPerBuffer);

private:
    InferenceThread inferenceThread;
    RingBuffer receiveRingBuffer;

    int latencyInSamples = 0;
    int inferenceCounter = 0;
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
