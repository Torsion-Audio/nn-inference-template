#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/RingBuffer.h"
#include "utils/InferenceConfig.h"
#include "processors/WindowingProcessor.h"
#include "backends/LibtorchProcessor.h"
#include "backends/OnnxRuntimeProcessor.h"

class InferenceThread : public juce::Thread {
public:
    InferenceThread();
    ~InferenceThread() override;

    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void inferenceThreadFinished(juce::AudioBuffer<float>& buffer) {juce::ignoreUnused(buffer);}
    };
    void addInferenceListener(Listener* listenerToAdd) {listeners.add(listenerToAdd);}
    void removeInferenceListener(Listener* listenerToRemove) {listeners.remove(listenerToRemove);}

    void prepare(const juce::dsp::ProcessSpec& spec);

    void sendAudio(juce::AudioBuffer<float>& buffer);
    std::function<int(juce::AudioBuffer<float> buffer, int numOfSamples)> onNewProcessedBuffer;

    void setBackend(InferenceBackend backend);
    void enableWindowing(bool enabled);

    int getLatency() const;

public:
    bool init = true;
    int init_samples = 0;

private:
    void run() override;
    void runAudioProcessing();

private:
    int maxModelCalcSize = MODEL_INPUT_SIZE;
    int modelInputSize = MODEL_INPUT_SIZE;
    std::atomic<float> processingTime;

    RingBuffer receiveRingBuffer;
    juce::AudioBuffer<float> modelInputBuffer;
    juce::AudioBuffer<float> processedBuffer;

    WindowingType windowingType = Hanning;
    WindowingProcessor windowingProcessor;

    OnnxRuntimeProcessor onnxProcessor;
    LibtorchProcessor torchProcessor;

    std::atomic<InferenceBackend> currentBackend {LIBTORCH};

    juce::ListenerList<Listener> listeners;
};


#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
