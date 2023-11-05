#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread")
{
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepareToPlay(const juce::dsp::ProcessSpec &spec) {
    if (isThreadRunning()) stopThread(100);

    sendBuffer.initialise(1, (int) spec.sampleRate * 6); // TODO how big does the buffer need to be?
    receiveBuffer.initialise(1, (int) spec.sampleRate * 6); // TODO how big does the buffer need to be?

    std::fill(processedModelInput.begin(), processedModelInput.end(), 0.f);
    std::fill(rawModelOutputBuffer.begin(), rawModelOutputBuffer.end(), 0.f);

    onnxProcessor.prepareToPlay();
    torchProcessor.prepareToPlay();
    tfliteProcessor.prepareToPlay();

    // Start inference after first memory allocation
    if (!isThreadRunning()) startThread(juce::Thread::Priority::highest);
}

void InferenceThread::run() {
    while (!threadShouldExit()) {
        if (sendBuffer.getAvailableSamples(0) >= (BATCH_SIZE * MODEL_INPUT_SIZE)) {
            process();
        } else {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }
}

void InferenceThread::process() {
    preProcess();
    inference();
    postProcess();
}

void InferenceThread::preProcess() {
    for (size_t batch = 0; batch < BATCH_SIZE; batch++) {
        size_t baseIdx = batch * MODEL_INPUT_SIZE_BACKEND;
        size_t prevBaseIdx = (batch == 0 ? BATCH_SIZE - 1 : batch - 1) * MODEL_INPUT_SIZE_BACKEND;

        for (size_t j = 1; j < MODEL_INPUT_SIZE_BACKEND; j++) {
            processedModelInput[baseIdx + j - 1] = processedModelInput[prevBaseIdx + j];
        }

        processedModelInput[baseIdx + MODEL_INPUT_SIZE_BACKEND - 1] = sendBuffer.popSample(0);
    }
}

void InferenceThread::inference() {
    if (currentBackend == ONNX) {
        onnxProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
    } else if (currentBackend == LIBTORCH) {
        torchProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
    } else if (currentBackend == TFLITE) {
        tfliteProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
    }
}

void InferenceThread::postProcess() {
    for (size_t j = 0; j < BATCH_SIZE * MODEL_OUTPUT_SIZE_BACKEND; j++) {
        receiveBuffer.pushSample(rawModelOutputBuffer[j], 0);
    }
}

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}

ThreadSafeBuffer& InferenceThread::getSendBuffer() {
    return sendBuffer;
}

ThreadSafeBuffer& InferenceThread::getReceiveBuffer() {
    return receiveBuffer;
}