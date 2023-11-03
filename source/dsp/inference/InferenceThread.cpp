#include "InferenceThread.h"

InferenceThread::InferenceThread() : juce::Thread("InferenceThread") {
    startThread(juce::Thread::Priority::highest);
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepareToPlay(const juce::dsp::ProcessSpec &spec) {
    for (size_t i = 0; i < processedModelInput.size(); i++) {
        processedModelInput[i] = 0.f;
    }
    for (size_t i = 0; i < rawModelOutputBuffer.size(); i++) {
        rawModelOutputBuffer[i] = 0.f;
    }

    rawModelInput.initialise(1, (int) spec.sampleRate * 6); // TODO how big does the ringbuffer need to be?
    processedModelOutput.initialise(1, (int) spec.sampleRate * 6); // TODO how big does the ringbuffer need to be?

    // TODO think about calculate maxInferencesPerBlock
    maxInferencesPerBlock = std::max((unsigned int) 1, (spec.maximumBlockSize / (BATCH_SIZE * MODEL_INPUT_SIZE)) + 1);

    onnxProcessor.prepareToPlay();
    torchProcessor.prepareToPlay();
    tfliteProcessor.prepareToPlay();
}

bool InferenceThread::isInferenceRunning() {
    return (currentlyProcessing.load() && processingShouldStart.load());
}

bool InferenceThread::startInference() {
    if (isInferenceRunning()) {
        return false;
    } else {
        processingShouldStart.store(true);
        return true;
    }
}


void InferenceThread::run() {
    while (!threadShouldExit()) {
        std::this_thread::sleep_for(std::chrono::microseconds(35));
        if (processingShouldStart.load()) {
            currentlyProcessing.store(true);
            processingShouldStart.store(false);

            auto start = std::chrono::high_resolution_clock::now();

            inference();

            auto stop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
            processingTime.store((float) duration.count());

            // std::cout << "Inference took " << duration.count() << "ms" << std::endl;


            currentlyProcessing.store(false);
        }
    }
}

void InferenceThread::inference() {

    size_t numInferences = (size_t) (rawModelInput.getAvailableSamples(0) / (MODEL_INPUT_SIZE * BATCH_SIZE));
    numInferences = std::min(numInferences, maxInferencesPerBlock);

    for (size_t i = 0; i < numInferences; i++) {

        // pre-processing
        for (size_t batch = 0; batch < BATCH_SIZE; batch++) {
            if (batch == 0) {
                for (size_t j = 1; j < MODEL_INPUT_SIZE_BACKEND; j++) {
                    processedModelInput[j-1] = processedModelInput[((BATCH_SIZE-1) * MODEL_INPUT_SIZE_BACKEND) + j];
                }
            } else {
                for (size_t j = 1; j < MODEL_INPUT_SIZE_BACKEND; j++) {
                    processedModelInput[(batch*MODEL_INPUT_SIZE_BACKEND) + (j-1)] = processedModelInput[((batch-1)*MODEL_INPUT_SIZE_BACKEND) + j];
                }
            }
            processedModelInput[(batch*MODEL_INPUT_SIZE_BACKEND) + (MODEL_INPUT_SIZE_BACKEND-1)] = rawModelInput.popSample(0);
        }

        // actual inference
        processModel();

        // post-processing
        for (size_t j = 0; j < BATCH_SIZE * MODEL_OUTPUT_SIZE_BACKEND; j++) {
            processedModelOutput.pushSample(rawModelOutputBuffer[j], 0);
        }
    }
}

void InferenceThread::processModel() {
        if (currentBackend == ONNX) {
            onnxProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
        } else if (currentBackend == LIBTORCH) {
            torchProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
        } else if (currentBackend == TFLITE) {
            tfliteProcessor.processBlock(processedModelInput, rawModelOutputBuffer);
        }
}

RingBuffer& InferenceThread::getModelInputBuffer() {
    return rawModelInput;
}

RingBuffer& InferenceThread::getModelOutputBuffer() {
    return processedModelOutput;
}

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}

