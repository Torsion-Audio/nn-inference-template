#ifndef NN_INFERENCE_TEMPLATE_WINDOWINGPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_WINDOWINGPROCESSOR_H

#include <JuceHeader.h>
#include "../../utils/RingBuffer.h"

enum WindowingType {
    None,
    Hanning
};

class WindowingProcessor {
public:
    WindowingProcessor(int inputSize);
    ~WindowingProcessor() = default;

    void prepare(const juce::dsp::ProcessSpec& spec);

    void processPreBlock(RingBuffer& readRingBuffer, juce::AudioBuffer<float>& writeBuffer);
    void processPostBlock(juce::AudioBuffer<float>& readBuffer, juce::AudioBuffer<float>& writeBuffer);

    int getNumberOfNeededSamples();

    void setWindowingType(WindowingType newType);
    WindowingType getCurrentWindowingType();

private:
    int modelInputSize;
    int hanningChunkSize;

    WindowingType windowingType = Hanning;

    juce::AudioBuffer<float> prevBuffer;
    juce::AudioBuffer<float> prevCombinedBuffer;

    std::unique_ptr<juce::dsp::WindowingFunction<float>> hanningWindow;
};


#endif //NN_INFERENCE_TEMPLATE_WINDOWINGPROCESSOR_H
