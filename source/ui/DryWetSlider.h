#ifndef NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H
#define NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H

#include <JuceHeader.h>
#include "CustomSliderLookAndFeel.h"
#include "PluginParameters.h"

class DryWetSlider : public juce::Component {
public:
    DryWetSlider(juce::AudioProcessorValueTreeState& state);
    ~DryWetSlider() override;

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& apvts;

    juce::Slider slider;
    CustomSliderLookAndFeel customSliderLookAndFeel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachment;
};

#endif //NN_INFERENCE_TEMPLATE_DRYWETSLIDER_H
