#pragma once

#include "PluginProcessor.h"
#include "ui/BackendSelector.h"
#include "ui/DryWetSlider.h"
#include "PluginParameters.h"

//==============================================================================
class AudioPluginAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::AudioProcessorValueTreeState::Listener
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void parameterChanged (const juce::String& parameterID, float newValue) override {

    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    const int pluginEditorWidth = 520;
    const int pluginEditorHeight = 750;


    std::unique_ptr<juce::Drawable> background = juce::Drawable::createFromImageData (BinaryData::background_png, BinaryData::background_pngSize);
    std::unique_ptr<juce::Drawable> texture = juce::Drawable::createFromImageData (BinaryData::texture_overlay_png, BinaryData::texture_overlay_pngSize);

    BackendSelector backendSelector;
    DryWetSlider dryWetSlider;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
