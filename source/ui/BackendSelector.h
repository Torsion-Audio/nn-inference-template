#ifndef NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H
#define NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H

#include <JuceHeader.h>
#include "../PluginParameters.h"
#include "../dsp/inference/InferenceConfig.h"

class BackendSelector : public juce::Component {
public:
    BackendSelector(juce::AudioProcessorValueTreeState& state);

    void setBackend(int backendID);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseMove (const juce::MouseEvent &event) override;
    void mouseDown (const juce::MouseEvent &event) override;

    void backendChanged();

    int getCurrentBackendID();
    void getNextBackend();

    InferenceBackend stringToBackend(juce::String& backendStr);
    juce::String backendToString(InferenceBackend backend);

private:
    juce::AudioProcessorValueTreeState& apvts;

    std::unique_ptr<juce::Drawable> backendTFLite = juce::Drawable::createFromImageData (BinaryData::tflite_backend_png, BinaryData::tflite_backend_pngSize);
    std::unique_ptr<juce::Drawable> backendLibTorch = juce::Drawable::createFromImageData (BinaryData::libtorch_backend_png, BinaryData::libtorch_backend_pngSize);
    std::unique_ptr<juce::Drawable> backendONNX = juce::Drawable::createFromImageData (BinaryData::onnx_backend_png, BinaryData::onnx_backend_pngSize);
    std::unique_ptr<juce::Drawable> highlight = juce::Drawable::createFromImageData (BinaryData::highlight_png, BinaryData::highlight_pngSize);

    juce::Rectangle<int> tfliteBounds;
    juce::Rectangle<int> libtorchBounds;
    juce::Rectangle<int> onnxBounds;

    juce::Rectangle<int> tfliteHighlightBounds;
    juce::Rectangle<int> libtorchHighlightBounds;
    juce::Rectangle<int> onnxHighlightBounds;

    juce::StringArray backendList;
    InferenceBackend currentBackend;

    enum BackendHover {
        NONE,
        TFLITE_HOVER,
        LIBTORCH_HOVER,
        ONNX_HOVER
    };

    BackendHover currentHover = NONE;
};

#endif //NN_INFERENCE_TEMPLATE_BACKENDSELECTOR_H
