//
// Created by valentin.ackva on 25.10.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H
#define NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H

#include <JuceHeader.h>
#include "CustomSliderLookAndFeel.h"

class DryWetSlider : public juce::Component {
public:
    DryWetSlider() {
        slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        slider.setLookAndFeel(&customSliderLookAndFeel);
        addAndMakeVisible(slider);
    }

    ~DryWetSlider() override {
        slider.setLookAndFeel(nullptr);
    }

    void resized() override {
        slider.setBounds(getWidth() / 4, 0, getWidth() / 2, getHeight());
    }

    void paint(juce::Graphics& g) override {
    }

private:
    juce::Slider slider;
    CustomSliderLookAndFeel customSliderLookAndFeel;
};

#endif //NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H
