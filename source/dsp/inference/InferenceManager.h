#ifndef NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
#define NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H

#include <JuceHeader.h>

#include "InferenceThread.h"
#include "../utils/ThreadSafeBuffer.h"

class InferenceManager {
public:
    InferenceManager();
    ~InferenceManager();

    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processBlock(juce::AudioBuffer<float>& buffer);

    void parameterChanged(const juce::String &parameterID, float newValue);

    int getLatency();

    // Required for unit test
    int getNumReceivedSamples();
    bool isInitializing();
    InferenceThread &getInferenceThread();

private:
    void processInput(juce::AudioBuffer<float>& buffer);
    void processOutput(juce::AudioBuffer<float>& buffer);

private:
    InferenceThread inferenceThread;
    juce::dsp::ProcessSpec spec;

    bool init = true;
    int bufferCount = 0;
    int initSamples = 0;
    int inferenceCounter = 0;
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
