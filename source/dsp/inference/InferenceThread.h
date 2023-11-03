#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/ThreadSafeBuffer.h"
#include "InferenceConfig.h"
#include "backends/OnnxRuntimeProcessor.h"
#include "backends/LibtorchProcessor.h"
#include "backends/TFLiteProcessor.h"

class InferenceThread : private juce::Thread {
public:
    InferenceThread();
    ~InferenceThread() override;

    void prepareToPlay(const juce::dsp::ProcessSpec &spec);
    void setBackend(InferenceBackend backend);

    ThreadSafeBuffer& getSendBuffer();
    ThreadSafeBuffer& getReceiveBuffer();

private:
    void run() override;
    void inference();
    void processModel();

private:
    std::atomic<float> processingTime;

    OnnxRuntimeProcessor onnxProcessor;
    LibtorchProcessor torchProcessor;
    TFLiteProcessor tfliteProcessor;

    ThreadSafeBuffer sendBuffer;
    ThreadSafeBuffer receiveBuffer;

    NNInferenceTemplate::OutputArray rawModelOutputBuffer;
    NNInferenceTemplate::InputArray processedModelInput;

    std::atomic<InferenceBackend> currentBackend {ONNX};
};

#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
