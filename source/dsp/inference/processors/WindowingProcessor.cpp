//
// Created by valentin.ackva on 05.10.2023.
//

#include "WindowingProcessor.h"

WindowingProcessor::WindowingProcessor(int inputSize) : modelInputSize(inputSize),
                                                        hanningChunkSize(modelInputSize / 2)
{
    hanningWindow = std::make_unique<juce::dsp::WindowingFunction<float>>(modelInputSize,
            juce::dsp::WindowingFunction<float>::hann);
}

void WindowingProcessor::prepare(const juce::dsp::ProcessSpec &spec) {
    juce::ignoreUnused(spec);

    prevBuffer.setSize(1, hanningChunkSize);
    prevCombinedBuffer.setSize(1, modelInputSize);

    for (int sample = 0; sample < hanningChunkSize; ++sample) {
        prevBuffer.setSample(0, sample, 0.0f);
    }

    for (int sample = 0; sample < modelInputSize; ++sample) {
        prevCombinedBuffer.setSample(0, sample, 0.0f);
    }
}

void WindowingProcessor::processPreBlock(RingBuffer &readRingBuffer, juce::AudioBuffer<float> &writeBuffer) {
    if (windowingType == Hanning) {
        auto prevBufferWrite = prevBuffer.getWritePointer(0);
        auto prevBufferRead  = prevBuffer.getReadPointer(0);
        auto combinedBufferWrite = writeBuffer.getWritePointer(0);

        for (int i = 0; i < hanningChunkSize; ++i) {
            combinedBufferWrite[i] = prevBufferRead[i];
        }

        for (int i = 0; i < hanningChunkSize; ++i) {
            auto sample = readRingBuffer.popSample(0) * 0.5f;
            combinedBufferWrite[hanningChunkSize + i] = sample;
            prevBufferWrite[i] = sample;
        }

        hanningWindow->multiplyWithWindowingTable(combinedBufferWrite, modelInputSize);
    } else if (windowingType == None) {
        auto combinedBufferWrite = writeBuffer.getWritePointer(0);
        for (int sample = 0; sample < modelInputSize; ++sample) {
            combinedBufferWrite[sample] = readRingBuffer.popSample(0);
        }
    }
}

void WindowingProcessor::processPostBlock(juce::AudioBuffer<float> &readBuffer, juce::AudioBuffer<float> &writeBuffer) {
    if (windowingType == Hanning) {
        const int numSamples = writeBuffer.getNumSamples() / 2;
        auto bufferWrite = writeBuffer.getWritePointer(0);
        auto prevCombinedBufferRead  = prevCombinedBuffer.getReadPointer(0);
        auto combinedBufferRead  = readBuffer.getReadPointer(0);
        auto prevCombinedBufferWrite = prevCombinedBuffer.getWritePointer(0);

        for (int i = 0; i < numSamples; ++i) {
            auto prevSample = prevCombinedBufferRead[numSamples + i];
            auto currentSample = combinedBufferRead[i];
            bufferWrite[i] = prevSample + currentSample;
        }

        for (int i = 0; i < modelInputSize; ++i) {
            prevCombinedBufferWrite[i] = combinedBufferRead[i];
        }

    } else if (windowingType == None) {
        auto bufferWrite = writeBuffer.getWritePointer(0);
        auto combinedBufferRead  = readBuffer.getReadPointer(0);
        for (int sample = 0; sample < modelInputSize; ++sample) {
            bufferWrite[sample] = combinedBufferRead[sample];
        }
    }
}

int WindowingProcessor::getNumberOfNeededSamples() {
    return (windowingType == Hanning) ? hanningChunkSize : modelInputSize;
}

WindowingType WindowingProcessor::getCurrentWindowingType() {
    return windowingType;
}

void WindowingProcessor::setWindowingType(WindowingType newType) {
    windowingType = newType;
}
