#include "RingBuffer.h"

RingBuffer::RingBuffer() = default;

void RingBuffer::initialise(int numChannels, int numSamples) {
    buffer.clear();

    for (int i = 0; i < numChannels; ++i) {
        buffer.push_back(moodycamel::ReaderWriterQueue<float>((size_t) numSamples));
    }
}

void RingBuffer::reset() {
    for (size_t i = 0; i < buffer.size(); ++i) {
        for (size_t j = 0; j < buffer[i].size_approx(); ++j) {
            buffer[i].pop();
        }
    }
}

void RingBuffer::pushSample(float sample, size_t channel) {
    if (!buffer[channel].try_enqueue(sample)) {
        std::cout << "RingBuffer::pushSample: could not enqueue sample" << std::endl;
    }
}

float RingBuffer::popSample(size_t channel) {
    float sample;
    if(!buffer[channel].try_dequeue(sample)) {
        std::cout << "RingBuffer::popSample: could not dequeue sample" << std::endl;
        return 0.0f;
    } else {
        return sample;
    }
}

int RingBuffer::getAvailableSamples(size_t channel) {
    return buffer[channel].size_approx();
}