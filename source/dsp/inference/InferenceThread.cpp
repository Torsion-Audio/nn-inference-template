#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread") {
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepareToPlay() {
    for (size_t i = 0; i < MODEL_INPUT_SIZE; i++) {
        processedModelInput[i] = 0.f;
        rawModelInputBuffer[i] = 0.f;
        processedModelOutput[i] = 0.f;
    }
    for (size_t i = 0; i < MODEL_OUTPUT_SIZE; i++) {
        rawModelOutputBuffer[i] = 0.f;
    }

    onnxProcessor.prepareToPlay();
}

void InferenceThread::run() {
    auto start = std::chrono::high_resolution_clock::now();
    inference();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    processingTime.store(duration.count());

    listeners.call(&Listener::inferenceThreadFinished);
}

void InferenceThread::inference() {

    for (size_t i = 0; i < MODEL_INPUT_SIZE; i++) {
        // pre-processing
        for (size_t j = 1; j < MODEL_INPUT_SIZE; j++) {
            processedModelInput[j-1] = processedModelInput[j];
        }
        processedModelInput[MODEL_INPUT_SIZE-1] = rawModelInputBuffer[i];

        // actual inference
        processModel();

        // post-processing
        processedModelOutput[i] = rawModelOutputBuffer[0];
    }
}

void InferenceThread::processModel() {
        if (currentBackend == LIBTORCH) {
        // processedModelOutputBuffer = torchProcessor.process(rawModelInputBuffer);
        } else if (currentBackend == ONNX) {
            onnxProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
        }
}

std::array<float, MODEL_INPUT_SIZE>& InferenceThread::getModelInputBuffer() {
    return rawModelInputBuffer;
}

std::array<float, MODEL_INPUT_SIZE>& InferenceThread::getModelOutputBuffer() {
    return processedModelOutput;
}

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}