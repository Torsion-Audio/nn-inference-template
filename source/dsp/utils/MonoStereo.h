//
// Created by valentin.ackva on 01.11.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_MONOSTEREO_H
#define NN_INFERENCE_TEMPLATE_MONOSTEREO_H

#include "JuceHeader.h"

class MonoStereo {
public:
    MonoStereo() {

    }

    void stereoToMono(juce::AudioBuffer<float> &targetMonoBlock, juce::AudioBuffer<float> &sourceBlock) {
        if (sourceBlock.getNumChannels() == 1) {
            targetMonoBlock.makeCopyOf(sourceBlock);
        } else {
            auto nSamples = sourceBlock.getNumSamples();

            auto monoWrite = targetMonoBlock.getWritePointer(0);
            auto lRead = sourceBlock.getReadPointer(0);
            auto rRead = sourceBlock.getReadPointer(1);

            juce::FloatVectorOperations::copy(monoWrite, lRead, nSamples);
            juce::FloatVectorOperations::add(monoWrite, rRead, nSamples);
            juce::FloatVectorOperations::multiply(monoWrite, 0.5f, nSamples);
        }
    }

    void monoToStereo(juce::AudioBuffer<float> &targetStereoBlock, juce::AudioBuffer<float> &sourceBlock) {
        if (targetStereoBlock.getNumChannels() == 1) {
            targetStereoBlock.makeCopyOf(sourceBlock);
        } else {
            auto nSamples = sourceBlock.getNumSamples();

            auto lWrite = targetStereoBlock.getWritePointer(0);
            auto rWrite = targetStereoBlock.getWritePointer(1);
            auto monoRead = sourceBlock.getReadPointer(0);

            juce::FloatVectorOperations::copy(lWrite, monoRead, nSamples);
            juce::FloatVectorOperations::copy(rWrite, monoRead, nSamples);
        }
    }
};


#endif //NN_INFERENCE_TEMPLATE_MONOSTEREO_H
