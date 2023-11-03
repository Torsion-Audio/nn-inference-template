#ifndef NN_INFERENCE_TEMPLATE_RINGBUFFER_H
#define NN_INFERENCE_TEMPLATE_RINGBUFFER_H

#include <JuceHeader.h>
#include <readerwriterqueue.h>

class RingBuffer
{
public:
    RingBuffer();

    void initialise(int numChannels, int numSamples);
    void reset();
    void pushSample(float sample, size_t channel);
    float popSample(size_t channel);
    int getAvailableSamples(size_t channel);

private:
    std::vector<moodycamel::ReaderWriterQueue<float>> buffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RingBuffer)
};

#endif //NN_INFERENCE_TEMPLATE_RINGBUFFER_H
