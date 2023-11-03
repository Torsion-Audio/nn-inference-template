#include "InferenceManager.h"

InferenceManager::InferenceManager() {
}

InferenceManager::~InferenceManager() {
}

void InferenceManager::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == "paramBackEnd") {
        auto newBackEnd = (newValue == 0.0f) ? InferenceBackend::LIBTORCH : InferenceBackend::ONNX;
        inferenceThread.setBackend(newBackEnd);
    }
}

void InferenceManager::prepareToPlay(const juce::dsp::ProcessSpec &newSpec) {
    spec = const_cast<juce::dsp::ProcessSpec &>(newSpec);

    inferenceThread.prepareToPlay(spec);
    inferenceCounter = 0;

    init = true;
    bufferCount = 0;
    if ((int) spec.maximumBlockSize % (BATCH_SIZE * MODEL_INPUT_SIZE) != 0 && (int) spec.maximumBlockSize > (BATCH_SIZE * MODEL_INPUT_SIZE)) {
        initSamples = (BATCH_SIZE * MODEL_INPUT_SIZE) + (2 * (int) spec.maximumBlockSize) + MAX_INFERENCE_TIME + MODEL_LATENCY;
    } else {
        initSamples = (BATCH_SIZE * MODEL_INPUT_SIZE) + (int) spec.maximumBlockSize + MAX_INFERENCE_TIME + MODEL_LATENCY;
    }
}

void InferenceManager::processBlock(juce::AudioBuffer<float> &buffer) {
    if (init) {
        bufferCount += buffer.getNumSamples();
        if (bufferCount >= initSamples) init = false;
    }
    processInput(buffer);
    processOutput(buffer);
}

void InferenceManager::processInput(juce::AudioBuffer<float> &buffer) {
    auto& sendBuffer = inferenceThread.getSendBuffer();
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        sendBuffer.pushSample(buffer.getSample(0, sample), 0);
    }
}

void InferenceManager::processOutput(juce::AudioBuffer<float> &buffer) {
    buffer.clear();

    if (!init) {
        auto& receiveBuffer = inferenceThread.getReceiveBuffer();
        while (inferenceCounter > 0) {
            if (receiveBuffer.getAvailableSamples(0) >= 2 * buffer.getNumSamples()) {
                for (int i = 0; i < buffer.getNumSamples(); ++i) {
                    receiveBuffer.popSample(0);
                }
                inferenceCounter--;
            }
            else {
                break;
            }
        }
        if (receiveBuffer.getAvailableSamples(0) >= buffer.getNumSamples()) {
            for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
                buffer.setSample(0, sample, receiveBuffer.popSample(0));
            }
        }
        else {
            inferenceCounter++;
            std::cout << "##### missing samples" << std::endl;
        }
    }
}

int InferenceManager::getLatency() {
    int minimumLatency = initSamples - (int) spec.maximumBlockSize;
    if (minimumLatency % (int) spec.maximumBlockSize == 0) return minimumLatency;
    else return ((int) ((double) minimumLatency / (double) spec.maximumBlockSize) + 1) * (int) spec.maximumBlockSize;
}

InferenceThread &InferenceManager::getInferenceThread() {
    return inferenceThread;
}