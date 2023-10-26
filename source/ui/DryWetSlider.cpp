#include "DryWetSlider.h"

DryWetSlider::DryWetSlider(juce::AudioProcessorValueTreeState &apvts) {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    slider.setLookAndFeel(&customSliderLookAndFeel);
    addAndMakeVisible(slider);

    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, PluginParameters::DRY_WET_ID.getParamID(), slider);
}

DryWetSlider::~DryWetSlider() {
    sliderAttachment.reset();
    slider.setLookAndFeel(nullptr);
}

void DryWetSlider::resized() {
    slider.setBounds(getWidth() / 4, 0, getWidth() / 2, getHeight());
}
