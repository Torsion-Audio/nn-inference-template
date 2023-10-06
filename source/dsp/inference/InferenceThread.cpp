//
// Created by valentin.ackva on 29.09.2023.
//

#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread"),
                                     windowingProcessor(MODEL_INPUT_SIZE),
                                     torchProcessor(MODEL_INPUT_SIZE),
                                     onnxProcessor(MODEL_INPUT_SIZE)
{

}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepare(const juce::dsp::ProcessSpec &spec) {
    receiveRingBuffer.initialise(1, (int) spec.sampleRate * 6);

    init = true;
    init_samples = 0;

    processedBuffer.setSize(1, modelInputSize);
    modelInputBuffer.setSize(1, modelInputSize);

    windowingProcessor.prepare(spec);
}

void InferenceThread::sendAudio(juce::AudioBuffer<float> &buffer) {

    auto readPointer = buffer.getReadPointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        receiveRingBuffer.pushSample(readPointer[sample], 0);
        if (init) init_samples++;
    }

    // Only change type when audio is not processing
    if (windowingType != windowingProcessor.getCurrentWindowingType()) {
        windowingProcessor.setWindowingType(windowingType);
    }

    const int availableSamples = receiveRingBuffer.getAvailableSamples(0);
    const int numberOfSamplesNeeded = windowingProcessor.getNumberOfNeededSamples();
    bool enoughSamples = availableSamples >= numberOfSamplesNeeded;

    if (!isThreadRunning() && enoughSamples) {
        windowingProcessor.processPreBlock(receiveRingBuffer, modelInputBuffer);
        startThread(juce::Thread::Priority::highest);
    }

    if (init && init_samples >= modelInputSize + maxModelCalcSize) init = false;
}

void InferenceThread::run() {
    auto start = std::chrono::high_resolution_clock::now();

    runAudioProcessing();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    processingTime.store(duration.count());
}

void InferenceThread::runAudioProcessing() {
    if (currentBackend == LIBTORCH) {
        torchProcessor.process(modelInputBuffer);
    } else if (currentBackend == ONNX) {
        onnxProcessor.process(modelInputBuffer);
    }

    windowingProcessor.processPostBlock(modelInputBuffer, processedBuffer);
    int numSamples = windowingProcessor.getNumberOfNeededSamples();

    onNewProcessedBuffer(processedBuffer, numSamples);
}

int InferenceThread::getLatency() const{
    return modelInputSize + maxModelCalcSize;
}

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}

void InferenceThread::enableWindowing(bool enabled) {
    windowingType = enabled ? Hanning : None;
}
