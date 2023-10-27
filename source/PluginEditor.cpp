#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), apvts(p.getValueTreeState()), dryWetSlider(p.getValueTreeState()), backendSelector(p.getValueTreeState())
{
    juce::ignoreUnused (processorRef);

    for (auto & parameterID : PluginParameters::getPluginParameterList()) {
        apvts.addParameterListener(parameterID, this);
    }

    double ratio = static_cast<double>(pluginEditorWidth) / static_cast<double>(pluginEditorHeight);
    setResizeLimits(pluginEditorWidth / 2, pluginEditorHeight / 2, pluginEditorWidth, pluginEditorHeight);
    getConstrainer()->setFixedAspectRatio(ratio);
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
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto scaledHeight = [&] (int factor) {
        return static_cast<int>((float) getHeight() / (float) pluginEditorHeight * factor);
    };

    backendSelector.setBounds(getBounds().removeFromTop(scaledHeight(500)));

    auto sliderComponentBound = getBounds().removeFromTop(scaledHeight(710)).removeFromBottom(scaledHeight(60));
    dryWetSlider.setBounds(sliderComponentBound);
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String &parameterID, float newValue) {
    if (parameterID == PluginParameters::BACKEND_TYPE_ID.getParamID()) {
        backendSelector.setBackend(static_cast<int>(newValue));
    }
}
