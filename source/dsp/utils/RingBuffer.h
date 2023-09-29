//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_RINGBUFFER_H
#define NN_INFERENCE_TEMPLATE_RINGBUFFER_H

#include <JuceHeader.h>

class RingBuffer
{
public:
    RingBuffer();

    void initialise(int numChannels, int numSamples);
    void reset();
    void pushSample(float sample, int channel);
    float popSample(int channel);
    int getAvailableSamples(int channel);

private:
    juce::AudioBuffer<float> buffer;
    std::vector<int> readPos, writePos;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingBuffer)
};

#endif //NN_INFERENCE_TEMPLATE_RINGBUFFER_H
