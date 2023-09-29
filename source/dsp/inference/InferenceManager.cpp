//
// Created by valentin.ackva on 29.09.2023.
//

#include "InferenceManager.h"

InferenceManager::InferenceManager()
{
    inferenceThread.onNewProcessedBuffer = [this] (juce::AudioBuffer<float> buffer) {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            receiveRingBuffer.pushSample(buffer.getSample(0, sample), 0);
        }
        return 0;
    };
}

void InferenceManager::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == "paramBackEnd") {
        auto newBackEnd = (newValue == 0.0f) ? InferenceBackend::LIBTORCH : InferenceBackend::ONNX;
        inferenceThread.setBackend(newBackEnd);
    }
}

void InferenceManager::prepare(const juce::dsp::ProcessSpec &spec) {
    auto samplesPerBlock = (int) spec.maximumBlockSize;

    inferenceThread.prepare(spec);

    receiveRingBuffer.initialise(1, (int) spec.sampleRate * 6);
    inferenceCounter = 0;

    calculateLatency(samplesPerBlock);
}

void InferenceManager::processBlock(juce::AudioBuffer<float> &buffer) {
    inferenceThread.sendAudio(buffer);
    processOutput(buffer);
}

void InferenceManager::processOutput(juce::AudioBuffer<float> &buffer) {
    auto numSamples = buffer.getNumSamples();
    auto availableSamples = receiveRingBuffer.getAvailableSamples(0);
    if (!inferenceThread.init) {
        if (availableSamples >= numSamples) {
            if (inferenceCounter > 0) {
                if (availableSamples >= 2 * numSamples) {
                    for (int i = 0; i < numSamples; ++i) {
                        receiveRingBuffer.popSample(0);
                    }
                    inferenceCounter--;
                }
            }
            for (int sample = 0; sample < numSamples; ++sample) {
                buffer.setSample(0, sample, receiveRingBuffer.popSample(0));
            }
        } else {
            inferenceCounter++;
            std::cout << "missing samples" << std::endl;
            for (int sample = 0; sample < numSamples; ++sample) {
                buffer.setSample(0, sample, 0.0f);
            }
        }
    }
}

void InferenceManager::calculateLatency(int maxSamplesPerBuffer) {
    float latency = (float) (inferenceThread.getLatency()) / (float) maxSamplesPerBuffer;
    if (latency == static_cast<float>(static_cast<int>(latency))) latencyInSamples = static_cast<int>(latency) * maxSamplesPerBuffer - maxSamplesPerBuffer;
    else latencyInSamples = static_cast<int>((latency + 1.f)) * maxSamplesPerBuffer - maxSamplesPerBuffer;
}

int InferenceManager::getLatency() const {
    return latencyInSamples;
}