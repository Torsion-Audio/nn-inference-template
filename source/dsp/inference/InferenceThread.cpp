#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread") {
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepareToPlay(float * newModelInputBuffer, float * newModelOutputBuffer) {
    modelInputBuffer = newModelInputBuffer;
    modelOutputBuffer = newModelOutputBuffer;

    onnxProcessor.prepareToPlay(modelInputBuffer);
    for (int i = 0; i < 150; i++) {
        std::cout << modelInputBuffer[i] << std::endl;
    }
}

void InferenceThread::startInference() {
    startThread(juce::Thread::Priority::highest);
}

void InferenceThread::run() {
    auto start = std::chrono::high_resolution_clock::now();
    forwardBuffer();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    processingTime.store(duration.count());
}

void InferenceThread::forwardBuffer() {
    if (currentBackend == LIBTORCH) {
        // modelOutputBuffer = torchProcessor.process(modelInputBuffer);
    } else if (currentBackend == ONNX) {
       auto rawModelOutput = onnxProcessor.processBlock();
    }

    listeners.call(&Listener::inferenceThreadFinished);
}

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}