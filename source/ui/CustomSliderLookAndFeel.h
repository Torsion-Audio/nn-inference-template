//
// Created by valentin.ackva on 25.10.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_CUSTOMSLIDERLOOKANDFEEL_H
#define NN_INFERENCE_TEMPLATE_CUSTOMSLIDERLOOKANDFEEL_H

#include "JuceHeader.h"

class CustomSliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomSliderLookAndFeel()
    {

    }

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override
    {
            auto trackWidth = (float) height * 0.025f;

            juce::Point<float> startPoint ((float) width * 0.05f, (float) y + (float) height * 0.5f);
            juce::Point<float> endPoint ((float) width * 0.95f, startPoint.y);
            float distance = endPoint.x - startPoint.x;

            juce::Path backgroundTrack;
            backgroundTrack.startNewSubPath (startPoint);
            backgroundTrack.lineTo (endPoint);

            g.setColour (juce::Colour{ 0xff3E3846 });
            g.strokePath (backgroundTrack, { trackWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded });


            float thumbWidth = (float) width / 8.f;
            float thumbHeight = thumbWidth / 2.f;


            float thumbX = startPoint.getX() + sliderPos / (float) width * (distance - 20.f);
            juce::Point<float> thumbCenter = { thumbX, (float) y + (float) height * 0.5f };

            juce::Rectangle<float> thumbRec = { thumbCenter.getX() - thumbWidth / 2.f, thumbCenter.getY() - thumbHeight / 2, thumbWidth, thumbHeight };
            g.setColour (juce::Colour { 0xff9F0E5D });
            g.fillRoundedRectangle(thumbRec, thumbHeight / 2.f);

            float expansionRatio = 1.2f;

            thumbRec.setWidth(thumbRec.getWidth() * expansionRatio);
            thumbRec.setHeight(thumbRec.getHeight() * expansionRatio);
            thumbRec.setCentre(thumbCenter);

            juce::Image offscreenImage = juce::Image(juce::Image::ARGB, width, height, true);

            juce::Graphics offscreenGraphics(offscreenImage);

            offscreenGraphics.setColour(juce::Colour { 0xff9F0E5D });
            offscreenGraphics.fillRoundedRectangle(thumbRec, thumbHeight / 2.0f * expansionRatio);

            juce::ImageConvolutionKernel kernel(15);
            kernel.createGaussianBlur(3.0f);
            kernel.applyToImage(offscreenImage, offscreenImage, offscreenImage.getBounds());

            g.drawImageAt(offscreenImage, 0, 0);
        }
};

#endif //NN_INFERENCE_TEMPLATE_CUSTOMSLIDERLOOKANDFEEL_H
