#ifndef NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
#define NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H

#include <JuceHeader.h>

#include "InferenceThread.h"
#include "../utils/RingBuffer.h"

class InferenceManager : private InferenceThread::Listener {
public:
    InferenceManager();
    ~InferenceManager() override;

    void prepareToPlay(const juce::dsp::ProcessSpec& spec);
    void processBlock(juce::AudioBuffer<float>& buffer);

    void parameterChanged(const juce::String &parameterID, float newValue);

    int getLatency() const;

private:
    void processOutput(juce::AudioBuffer<float>& buffer);
    void calculateLatency(int maxSamplesPerBuffer);
    void inferenceThreadFinished() override;

private:
    bool init = true;
    int init_samples = 0;

    InferenceThread inferenceThread;
    RingBuffer sendRingBuffer;
    juce::AudioBuffer<float> sendBuffer;
    RingBuffer receiveRingBuffer;
    juce::AudioBuffer<float> receiveBuffer;

    int numInferencedBufferAvailable;

    int latencyInSamples = 0;
    int inferenceCounter = 0;
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCEMANAGER_H
