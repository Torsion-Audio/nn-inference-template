//
// Created by valentin.ackva on 29.09.2023.
//

#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread") {
    setModelInputSize(modelInputSize);
    loadInternalModel();
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepare(const juce::dsp::ProcessSpec &spec) {
    receiveRingBuffer.initialise(1, (int) spec.sampleRate * 6);

    currentSpec = spec;
    init = true;
    init_samples = 0;

    chunkSize = modelInputSize / 2;

    tmpIn.setSize(1, chunkSize);
    tmpOut.setSize(1, chunkSize);

    prevBuffer.setSize(1, chunkSize, false, true, true);
    combinedBuffer.setSize(1, 2 * chunkSize, false, true, true);
    prevCombinedBuffer.setSize(1, 2 * chunkSize, false, true, true);

    hanningWindow = std::make_unique<juce::dsp::WindowingFunction<float>>(2 * chunkSize,
                                                                          juce::dsp::WindowingFunction<float>::hann);
}

void InferenceThread::sendAudio(juce::AudioBuffer<float> &buffer) {
    auto readPointer = buffer.getReadPointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        receiveRingBuffer.pushSample(readPointer[sample], 0);
        if (init) init_samples++;
    }

    const int availableSamples = receiveRingBuffer.getAvailableSamples(0);
    bool enoughSamples = (windowing) ? availableSamples >= modelInputSize / 2 : availableSamples >= modelInputSize;

    if (!isThreadRunning() && enoughSamples && !loadingModel) {

        if (windowing) {
            // Get the samples from ring buffer
            auto tmpInWrite = tmpIn.getWritePointer(0);
            for (int sample = 0; sample < modelInputSize / 2; ++sample) {
                tmpInWrite[sample] = receiveRingBuffer.popSample(0);
            }

            const int numSamples = tmpIn.getNumSamples();
            auto bufferRead  = tmpIn.getReadPointer(0);
            auto prevBufferWrite = prevBuffer.getWritePointer(0);
            auto prevBufferRead  = prevBuffer.getReadPointer(0);
            auto combinedBufferWrite = combinedBuffer.getWritePointer(0);

            for (int i = 0; i < chunkSize; ++i) {
                combinedBufferWrite[i] = prevBufferRead[i];
            }

            for (int i = 0; i < numSamples; ++i) {
                combinedBufferWrite[chunkSize + i] = bufferRead[i];
                prevBufferWrite[i] = bufferRead[i];
            }

            hanningWindow->multiplyWithWindowingTable(combinedBufferWrite, 2 * chunkSize);
        } else {
            auto combinedBufferWrite = combinedBuffer.getWritePointer(0);
            for (int sample = 0; sample < modelInputSize; ++sample) {
                combinedBufferWrite[sample] = receiveRingBuffer.popSample(0);
            }
        }

        std::cout << "Inference time: " << processingTime.load() << "ms\n";

        startThread(juce::Thread::Priority::highest);
    }

    if (init && init_samples >= modelInputSize + maxModelCalcSize) init = false;

}

void InferenceThread::run() {
    auto start = std::chrono::high_resolution_clock::now();

    if (currentBackend == LIBTORCH) {
        libtorchProcessor.process(combinedBuffer);
    } else if (currentBackend == ONNX) {
        onnxRuntimeProcessor.process(combinedBuffer);
    }


    if (windowing) {
        const int numSamples = processedBuffer.getNumSamples();
        auto bufferWrite = processedBuffer.getWritePointer(0);
        auto prevCombinedBufferRead  = prevCombinedBuffer.getReadPointer(0);
        auto combinedBufferRead  = combinedBuffer.getReadPointer(0);
        auto prevCombinedBufferWrite = prevCombinedBuffer.getWritePointer(0);

        for (int i = 0; i < numSamples; ++i) {
            auto prevSample = prevCombinedBufferRead[numSamples + i];
            auto currentSample = combinedBufferRead[i];
            bufferWrite[i] = prevSample + currentSample;
        }

        for (int i = 0; i < 2 * chunkSize; ++i) {
            prevCombinedBufferWrite[i] = combinedBufferRead[i];
        }
    } else {
        auto bufferWrite = processedBuffer.getWritePointer(0);
        auto combinedBufferRead  = combinedBuffer.getReadPointer(0);
        for (int sample = 0; sample < modelInputSize; ++sample) {
            bufferWrite[sample] = combinedBufferRead[sample];
        }
    }

    onNewProcessedBuffer(processedBuffer);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);


    processingTime.store(static_cast<float>(duration.count()));
}

void InferenceThread::setModelInputSize(int newModelInputSize) {
    modelInputSize = newModelInputSize;
    processedBuffer.setSize(1, newModelInputSize / 2);

    onnxRuntimeProcessor.setModelInputSize(newModelInputSize);
    libtorchProcessor.setModelInputSize(newModelInputSize);
}

void InferenceThread::loadInternalModel() {
    loadingModel = true;

    if (isThreadRunning()) {
        stopThread(10);

        while (!isThreadRunning()) {
            juce::Time::waitForMillisecondCounter(juce::Time::getMillisecondCounter() + 1);
        }
    }

//    onnxRuntimeProcessor.loadInternalModel();
//    libtorchProcessor.loadInternalModel();

    if (!startUp){
        prepare(currentSpec);
    }

    loadingModel = false;
    startUp = false;
}

int InferenceThread::getLatency() const{
    return modelInputSize + maxModelCalcSize;
}
