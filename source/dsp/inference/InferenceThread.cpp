#include "InferenceThread.h"

InferenceThread::InferenceThread(RingBuffer& sendBuffer, RingBuffer& returnBuffer) : juce::Thread("InferenceThread"),
                                                                                     sendRingBufferRef(sendBuffer),
                                                                                     receiveRingBufferRef(returnBuffer)
{
}

InferenceThread::~InferenceThread() {
    stopThread(100);
}

void InferenceThread::prepareToPlay(const juce::dsp::ProcessSpec &spec) {
    std::fill(processedModelInput.begin(), processedModelInput.end(), 0.f);
    std::fill(rawModelOutputBuffer.begin(), rawModelOutputBuffer.end(), 0.f);

    // TODO think about calculate maxInferencesPerBlock
    maxInferencesPerBlock = std::max((unsigned int) 1, (spec.maximumBlockSize / (BATCH_SIZE * MODEL_INPUT_SIZE)) + 1);

    onnxProcessor.prepareToPlay();
    torchProcessor.prepareToPlay();
    tfliteProcessor.prepareToPlay();

    // Start inference after first allocation
    if (!isThreadRunning()) startThread(juce::Thread::Priority::highest);
}

void InferenceThread::run() {
    while (!threadShouldExit()) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));

        if (sendRingBufferRef.getAvailableSamples(0) >= (BATCH_SIZE * MODEL_INPUT_SIZE)) {
            initInference();
        }
    }
}

void InferenceThread::initInference() {
    auto start = std::chrono::high_resolution_clock::now();

    inference();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = (float) std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    processingTime.store(duration);

    // std::cout << "Inference took " << duration.count() << "ms" << std::endl;
}

void InferenceThread::inference() {

    size_t numInferences = (size_t) (sendRingBufferRef.getAvailableSamples(0) / (MODEL_INPUT_SIZE * BATCH_SIZE));
    numInferences = std::min(numInferences, maxInferencesPerBlock);

    for (size_t i = 0; i < numInferences; i++) {

        // pre-processing
        for (size_t batch = 0; batch < BATCH_SIZE; batch++) {
            size_t baseIdx = batch * MODEL_INPUT_SIZE_BACKEND;
            size_t prevBaseIdx = (batch == 0 ? BATCH_SIZE - 1 : batch - 1) * MODEL_INPUT_SIZE_BACKEND;

            for (size_t j = 1; j < MODEL_INPUT_SIZE_BACKEND; j++) {
                processedModelInput[baseIdx + j - 1] = processedModelInput[prevBaseIdx + j];
            }

            processedModelInput[baseIdx + MODEL_INPUT_SIZE_BACKEND - 1] = sendRingBufferRef.popSample(0);
        }

        // actual inference
        processModel();

        // post-processing
        for (size_t j = 0; j < BATCH_SIZE * MODEL_OUTPUT_SIZE_BACKEND; j++) {
            receiveRingBufferRef.pushSample(rawModelOutputBuffer[j], 0);
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

void InferenceThread::setBackend(InferenceBackend backend) {
    currentBackend.store(backend);
}

