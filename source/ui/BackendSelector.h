//
// Created by valentin.ackva on 25.10.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H
#define NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H

#include <JuceHeader.h>
#include "config.h"
#include "../PluginParameters.h"
#include "../dsp/inference/InferenceConfig.h"

class BackendSelector : public juce::Component {
public:
    BackendSelector() {
        backendList = PluginParameters::backendTypes;
        currentBackend = getInferenceBackend(PluginParameters::defaultBackend);
    }

    ~BackendSelector() override {

    }

    void resized() override {

    }

    void getNextBackend() {
        switch (currentBackend) {
            case InferenceBackend::TFLITE:
                currentBackend = InferenceBackend::LIBTORCH;
                break;
            case InferenceBackend::LIBTORCH:
                currentBackend = InferenceBackend::ONNX;
                break;
            case InferenceBackend::ONNX:
                currentBackend = InferenceBackend::TFLITE;
                break;
        }
    }


    InferenceBackend getInferenceBackend(juce::String& backendStr) {
        if (backendStr == "TFLITE") return InferenceBackend::TFLITE;
        if (backendStr == "LIBTORCH") return InferenceBackend::LIBTORCH;
        if (backendStr == "ONNXRUNTIME") return InferenceBackend::ONNX;
        throw std::invalid_argument("Invalid backend string");
    }

    void backendChanged(int backendID) {
        juce::String type = backendList.getReference(backendID);
        currentBackend = getInferenceBackend(type);
        repaint();
    }

    void mouseMove (const juce::MouseEvent &event) override {
        auto pos = event.getPosition();

        std::cout << pos.getX() << " " << pos.getY() << std::endl;
    }

    void mouseDown (const juce::MouseEvent &event) override {
        getNextBackend();
        repaint();
    }

    void paint(juce::Graphics& g) override {
        auto currentBound = getBounds();

        switch (currentBackend) {
            case InferenceBackend::TFLITE:
                backendTFLite->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
                break;
            case InferenceBackend::LIBTORCH:
                backendLibTorch->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
                break;
            case InferenceBackend::ONNX:
                backendONNX->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::stretchToFit, 1.0f);
                break;
        }
    }

private:
    std::unique_ptr<juce::Drawable> backendTFLite = juce::Drawable::createFromImageData (BinaryData::tflite_backend_png, BinaryData::tflite_backend_pngSize);
    std::unique_ptr<juce::Drawable> backendLibTorch = juce::Drawable::createFromImageData (BinaryData::libtorch_backend_png, BinaryData::libtorch_backend_pngSize);
    std::unique_ptr<juce::Drawable> backendONNX = juce::Drawable::createFromImageData (BinaryData::onnx_backend_png, BinaryData::onnx_backend_pngSize);

    juce::StringArray backendList;
    InferenceBackend currentBackend;
};

#endif //NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H
