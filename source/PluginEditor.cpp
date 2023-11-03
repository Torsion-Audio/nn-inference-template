#include "PluginProcessor.h"
#include "PluginEditor.h"

// TODO: Not fully implemented yet
#define WINDOW_RESIZEABLE false

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), backendSelector(p.getValueTreeState()), dryWetSlider(p.getValueTreeState()), apvts(p.getValueTreeState())
{
    juce::ignoreUnused (processorRef);

    for (auto & parameterID : PluginParameters::getPluginParameterList()) {
        apvts.addParameterListener(parameterID, this);
    }

    if (WINDOW_RESIZEABLE) {
        double ratio = static_cast<double>(pluginEditorWidth) / static_cast<double>(pluginEditorHeight);
        setResizeLimits(pluginEditorWidth / 2, pluginEditorHeight / 2, pluginEditorWidth, pluginEditorHeight);
        getConstrainer()->setFixedAspectRatio(ratio);
    }
    setSize(pluginEditorWidth, pluginEditorHeight);

    addAndMakeVisible(backendSelector);
    addAndMakeVisible(dryWetSlider);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto currentBound = getBounds();
    background->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::centred, 1.0f);
    texture->drawWithin(g, currentBound.toFloat(), juce::RectanglePlacement::centred, .25f);

    auto currentBackend = backendSelector.getBackend();

    switch (currentBackend) {
        case TFLITE:
            tfliteFont->drawWithin(g, fontBounds.toFloat(), juce::RectanglePlacement::doNotResize, 1.0f);
            break;
        case LIBTORCH:
            libtorchFont->drawWithin(g, fontBounds.toFloat(), juce::RectanglePlacement::doNotResize, 1.0f);
            break;
        case ONNX:
            onnxFont->drawWithin(g, fontBounds.toFloat(), juce::RectanglePlacement::doNotResize, 1.0f);
            break;
        default:
            break;
    }

}

void AudioPluginAudioProcessorEditor::resized()
{
    auto scaledHeight = [&] (int factor) {
        return static_cast<int>((float) getHeight() / (float) pluginEditorHeight * factor);
    };

    fontBounds = getBounds().removeFromTop(610).removeFromBottom(75);

    backendSelector.setBounds(getBounds().removeFromTop(scaledHeight(500)));

    auto sliderComponentBound = getBounds().removeFromTop(scaledHeight(710)).removeFromBottom(scaledHeight(60));
    dryWetSlider.setBounds(sliderComponentBound);

}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == PluginParameters::BACKEND_TYPE_ID.getParamID()) {
        backendSelector.setBackend(static_cast<int>(newValue));
        repaint();
    }
}
