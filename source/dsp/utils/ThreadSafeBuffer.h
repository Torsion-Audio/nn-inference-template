#ifndef NN_INFERENCE_TEMPLATE_THREADSAFEBUFFER_H
#define NN_INFERENCE_TEMPLATE_THREADSAFEBUFFER_H

#include <JuceHeader.h>
#include <readerwriterqueue.h>

class ThreadSafeBuffer
{
public:
    ThreadSafeBuffer();

    void initialise(int numChannels, int numSamples);
    void reset();

    void pushSample(float sample, size_t channel);
    float popSample(size_t channel);

    int getAvailableSamples(size_t channel);

private:
    std::vector<moodycamel::ReaderWriterQueue<float>> buffer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ThreadSafeBuffer)
};

#endif //NN_INFERENCE_TEMPLATE_THREADSAFEBUFFER_H
