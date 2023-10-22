#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/RingBuffer.h"
#include "utils/InferenceConfig.h"
#include "backends/OnnxRuntimeProcessor.h"
// #include "backends/LibtorchProcessor.h"
// #include "processors/WindowingProcessor.h"

class InferenceThread : public juce::Thread {
public:
    InferenceThread();
    ~InferenceThread() override;

    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void inferenceThreadFinished() = 0;
    };
    void addInferenceListener(Listener* listenerToAdd) {listeners.add(listenerToAdd);}
    void removeInferenceListener(Listener* listenerToRemove) {listeners.remove(listenerToRemove);}

    void prepareToPlay();
    void setBackend(InferenceBackend backend);
    std::array<float, MODEL_INPUT_SIZE>& getModelInputBuffer();
    std::array<float, MODEL_INPUT_SIZE>& getModelOutputBuffer();

private:
    void run() override;
    void inference();
    void processModel();

private:
    std::atomic<float> processingTime;

    OnnxRuntimeProcessor onnxProcessor;
    // LibtorchProcessor torchProcessor;


    std::array<float, MODEL_INPUT_SIZE> rawModelInputBuffer;
    std::array<float, MODEL_OUTPUT_SIZE> rawModelOutputBuffer;
    std::array<float, MODEL_INPUT_SIZE> processedModelInput;
    std::array<float, MODEL_INPUT_SIZE> processedModelOutput;

    std::atomic<InferenceBackend> currentBackend {ONNX};

    juce::ListenerList<Listener> listeners;
};


#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
