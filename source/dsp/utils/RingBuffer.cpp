#include "RingBuffer.h"

RingBuffer::RingBuffer() = default;

void RingBuffer::initialise(int numChannels, int numSamples) {
    readPos.resize((size_t) numChannels);
    writePos.resize((size_t) numChannels);

    for (size_t i = 0; i < readPos.size(); i++) {
        readPos[i] = 0;
        writePos[i] = 0;
    }

    buffer.setSize(numChannels, numSamples);
}

void RingBuffer::reset() {
    buffer.clear();

    for (size_t i = 0; i < readPos.size(); i++) {
        readPos[i] = 0;
        writePos[i] = 0;
    }
}

void RingBuffer::pushSample(float sample, int channel) {
    buffer.setSample(channel, writePos[(size_t) channel], sample);

    ++writePos[(size_t) channel];

    if (writePos[(size_t) channel] >= buffer.getNumSamples()) {
        writePos[(size_t) channel] = 0;
    }
}

float RingBuffer::popSample(int channel) {
    auto sample = buffer.getSample(channel, readPos[(size_t) channel]);

    ++readPos[(size_t) channel];

    if (readPos[(size_t) channel] >= buffer.getNumSamples()) {
        readPos[(size_t) channel] = 0;
    }
    return sample;
}

int RingBuffer::getAvailableSamples(int channel) {
    int returnValue;

    if (readPos[(size_t) channel] <= writePos[(size_t) channel]) {
        returnValue = writePos[(size_t) channel] - readPos[(size_t) channel];
    } else {
        returnValue = writePos[(size_t) channel] + buffer.getNumSamples() - readPos[(size_t) channel];
    }

    return returnValue;
}