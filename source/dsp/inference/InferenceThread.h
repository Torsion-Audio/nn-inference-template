#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/RingBuffer.h"
#include "InferenceConfig.h"
#include "backends/OnnxRuntimeProcessor.h"
#include "backends/LibtorchProcessor.h"
#include "backends/TFLiteProcessor.h"

class InferenceThread : private juce::Thread {
public:
    InferenceThread(RingBuffer& sendBuffer, RingBuffer& returnBuffer);
    ~InferenceThread() override;

    void prepareToPlay(const juce::dsp::ProcessSpec &spec);
    void setBackend(InferenceBackend backend);

private:
    void run() override;
    void initInference();
    void inference();
    void processModel();

private:
    size_t maxInferencesPerBlock = 0;
    std::atomic<float> processingTime;

    OnnxRuntimeProcessor onnxProcessor;
    LibtorchProcessor torchProcessor;
    TFLiteProcessor tfliteProcessor;

    RingBuffer& sendRingBufferRef;
    RingBuffer& receiveRingBufferRef;

    std::array<float, BATCH_SIZE * MODEL_OUTPUT_SIZE_BACKEND> rawModelOutputBuffer;
    std::array<float, BATCH_SIZE * MODEL_INPUT_SIZE_BACKEND> processedModelInput;

    std::atomic<InferenceBackend> currentBackend {ONNX};

    juce::ListenerList<Listener> listeners;
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
