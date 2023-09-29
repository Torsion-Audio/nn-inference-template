//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
#define NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H

#include <JuceHeader.h>

#include "../utils/RingBuffer.h"
#include "utils/InferenceConfig.h"

#include "processors/LibtorchProcessor.h"
#include "processors/OnnxRuntimeProcessor.h"

class InferenceThread : public juce::Thread {
public:
    InferenceThread();
    ~InferenceThread() override;

    class Listener {
    public:
        virtual ~Listener() = default;
        virtual void inferenceThreadFinished(juce::AudioBuffer<float>& buffer) {juce::ignoreUnused(buffer);}
    };
    void addProcessorListener(Listener* listenerToAdd) {listeners.add(listenerToAdd);}
    void removeProcessorListener(Listener* listenerToRemove) {listeners.remove(listenerToRemove);}

    void prepare(const juce::dsp::ProcessSpec& spec);
    void sendAudio(juce::AudioBuffer<float>& buffer);
    int getLatency() const;

    std::function<int(juce::AudioBuffer<float> buffer)> onNewProcessedBuffer;

    bool init = true;
    int init_samples = 0;

    void setBackend(InferenceBackend backend) {
        juce::ignoreUnused(backend);
        //currentBackend.store(backend);
    }

private:
    void run() override;
    void setModelInputSize(int newModelInputSize);
    void loadInternalModel();
    void returnProcessedBuffer(juce::AudioBuffer<float>& buffer) {
        listeners.call([&buffer](Listener& l) { l.inferenceThreadFinished(buffer); });
    }

private:
    bool startUp = true;

    bool windowing = true;

    juce::dsp::ProcessSpec currentSpec;

    juce::AudioBuffer<float> tmpIn;
    juce::AudioBuffer<float> tmpOut;
    juce::AudioBuffer<float> processedBuffer;

    int maxModelCalcSize = 32768;
    int modelInputSize = 32768;
    RingBuffer receiveRingBuffer;

    std::atomic<float> processingTime;

    bool loadingModel = false;
    int chunkSize = 0;

    OnnxRuntimeProcessor onnxRuntimeProcessor;
    LibtorchProcessor libtorchProcessor;

    InferenceBackend currentBackend = TFLite;

    juce::ListenerList<Listener> listeners;

    juce::AudioBuffer<float> prevBuffer;
    juce::AudioBuffer<float> combinedBuffer;
    juce::AudioBuffer<float> prevCombinedBuffer;

    std::unique_ptr<juce::dsp::WindowingFunction<float>> hanningWindow;
};


#endif //NN_INFERENCE_TEMPLATE_INFERENCETHREAD_H
