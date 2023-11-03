#include "ThreadSafeBuffer.h"

ThreadSafeBuffer::ThreadSafeBuffer() = default;

void ThreadSafeBuffer::initialise(int numChannels, int numSamples) {
    buffer.clear();
    for (int i = 0; i < numChannels; ++i) {
        buffer.push_back(moodycamel::ReaderWriterQueue<float>((size_t) numSamples));
    }
}

void ThreadSafeBuffer::reset() {
    for (size_t i = 0; i < buffer.size(); ++i) {
        while (buffer[i].size_approx() != 0) {
            buffer[i].pop();
        }
    }
}

void ThreadSafeBuffer::pushSample(float sample, size_t channel) {
    if (!buffer[channel].try_enqueue(sample)) {
        std::cout << "ThreadSafeBuffer::pushSample: could not enqueue sample" << std::endl;
    }
}

float ThreadSafeBuffer::popSample(size_t channel) {
    float sample;
    if(!buffer[channel].try_dequeue(sample)) {
        std::cout << "ThreadSafeBuffer::popSample: could not dequeue sample" << std::endl;
        return 0.0f;
    } else {
        return sample;
    }
}

int ThreadSafeBuffer::getAvailableSamples(size_t channel) {
    return (int) buffer[channel].size_approx();
}