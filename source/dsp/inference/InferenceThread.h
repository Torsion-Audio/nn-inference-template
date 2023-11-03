#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/RingBuffer.h"
#include "InferenceConfig.h"
#include "backends/OnnxRuntimeProcessor.h"
#include "backends/LibtorchProcessor.h"
#include "backends/TFLiteProcessor.h"
// #include "processors/WindowingProcessor.h"

class InferenceThread : private juce::Thread {
public:
    InferenceThread();
    ~InferenceThread() override;

    void prepareToPlay(const juce::dsp::ProcessSpec &spec);
    void setBackend(InferenceBackend backend);

    RingBuffer& getModelInputBuffer();
    RingBuffer& getModelOutputBuffer();

    bool isInferenceRunning();
    bool startInference();

public:
    std::atomic<bool> currentlyProcessing {false};
    std::atomic<bool> processingShouldStart {false};

private:
    void run() override;
    void inference();
    void processModel();

private:
    size_t maxInferencesPerBlock = 0;
    std::atomic<float> processingTime;

    OnnxRuntimeProcessor onnxProcessor;
    LibtorchProcessor torchProcessor;
    TFLiteProcessor tfliteProcessor;

    RingBuffer rawModelInput;
    RingBuffer processedModelOutput;
    std::array<float, BATCH_SIZE * MODEL_OUTPUT_SIZE_BACKEND> rawModelOutputBuffer;
    std::array<float, BATCH_SIZE * MODEL_INPUT_SIZE_BACKEND> processedModelInput;

    std::atomic<InferenceBackend> currentBackend {ONNX};

    juce::ListenerList<Listener> listeners;
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
