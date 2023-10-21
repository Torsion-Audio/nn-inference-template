#include "InferenceManager.h"

InferenceManager::InferenceManager() {
    inferenceThread.addInferenceListener(this);
}

InferenceManager::~InferenceManager() {
    inferenceThread.removeInferenceListener(this);
}

void InferenceManager::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == "paramBackEnd") {
        auto newBackEnd = (newValue == 0.0f) ? InferenceBackend::LIBTORCH : InferenceBackend::ONNX;
        inferenceThread.setBackend(newBackEnd);
    }
}

void InferenceManager::prepareToPlay(const juce::dsp::ProcessSpec &spec) {
    numInferencedBufferAvailable = 0;

    sendRingBuffer.initialise(1, (int) spec.sampleRate * 6);
    receiveRingBuffer.initialise(1, (int) spec.sampleRate * 6); // TODO how big does the ringbuffer need to be?
    sendBuffer.setSize(1, MODEL_INPUT_SIZE);
    sendBuffer.clear();
    receiveBuffer.setSize(1, MODEL_INPUT_SIZE);
    receiveBuffer.clear();

    inferenceThread.prepareToPlay((float *) sendBuffer.getReadPointer(0), (float *) receiveBuffer.getWritePointer(0));
    numInferencedBufferAvailable = 0;
    inferenceCounter = 0;

    calculateLatency((int) spec.maximumBlockSize);
}

void InferenceManager::processBlock(juce::AudioBuffer<float> &buffer) {
    if (init) {
        init_samples += buffer.getNumSamples();
        if (init && init_samples >= MODEL_INPUT_SIZE + MAX_INFERENCE_TIME + MODEL_LATENCY) init = false;
    }
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        sendRingBuffer.pushSample(buffer.getSample(0, sample), 0);
    }
    if (sendRingBuffer.getAvailableSamples(0) >= MODEL_INPUT_SIZE) {
        if (!inferenceThread.isThreadRunning()) {
            while (numInferencedBufferAvailable > 0) {
                for (int sample = 0; sample < MODEL_INPUT_SIZE; ++sample) {
                    receiveRingBuffer.pushSample(receiveBuffer.getSample(0, sample), 0);
                }
                numInferencedBufferAvailable--;
            }
            for (int sample = 0; sample < MODEL_INPUT_SIZE; ++sample) {
                sendBuffer.setSample(0, sample, sendRingBuffer.popSample(0));
            }
            inferenceThread.startInference();
        }
    }
    processOutput(buffer);
}

void InferenceManager::processOutput(juce::AudioBuffer<float> &buffer) {
    buffer.clear();

    if (!init) {
        while (inferenceCounter > 0) {
            if (receiveRingBuffer.getAvailableSamples(0) >= 2 * buffer.getNumSamples()) {
                for (int i = 0; i < buffer.getNumSamples(); ++i) {
                    receiveRingBuffer.popSample(0);
                }
                inferenceCounter--;
            }
            else {
                break;
            }
        }

        if (receiveRingBuffer.getAvailableSamples(0) >= buffer.getNumSamples()) {
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
                buffer.setSample(0, sample, receiveRingBuffer.popSample(0));
            }
        }
        else {
            inferenceCounter++;
            std::cout << "missing samples" << std::endl;
        }
    }
}

void InferenceManager::calculateLatency(int maxSamplesPerBuffer) {
    latencyInSamples = MODEL_INPUT_SIZE + MAX_INFERENCE_TIME + MODEL_LATENCY - maxSamplesPerBuffer;
}

int InferenceManager::getLatency() const {
    return latencyInSamples;
}

void InferenceManager::inferenceThreadFinished() {
    numInferencedBufferAvailable++;
}
