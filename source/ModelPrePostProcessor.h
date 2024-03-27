#ifndef NN_INFERENCE_TEMPLATE_MODELPREPOSTPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_MODELPREPOSTPROCESSOR_H

#include "ModelConfig.h"
#include <anira/anira.h>

class HybridNNPrePostProcessor : public anira::PrePostProcessor
{
public:
    virtual void preProcess(anira::RingBuffer& input, anira::AudioBufferF& output, [[maybe_unused]] anira::InferenceBackend currentInferenceBackend) override {
        for (size_t batch = 0; batch < config.m_batch_size; batch++) {
            size_t baseIdx = batch * config.m_model_input_size;
            popSamplesFromBuffer(input, output, config.m_model_output_size, config.m_model_input_size-config.m_model_output_size, baseIdx);
        }
    };
    
    anira::InferenceConfig config = hybridNNConfig;
};

#endif //NN_INFERENCE_TEMPLATE_MODELPREPOSTPROCESSOR_H