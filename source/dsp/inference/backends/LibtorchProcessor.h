//
// Created by valentin.ackva on 29.09.2023.
//

#ifndef NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
#define NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H

#include <JuceHeader.h>

#include <torch/torch.h>
#include <torch/script.h>

class LibtorchProcessor {
public:
    LibtorchProcessor(int inputSize) : modelInputSize(inputSize) {
        loadModel();
    }

    ~LibtorchProcessor() = default;

    void process(juce::AudioBuffer<float>& buffer) {
        std::vector<int64_t> sizes = {modelInputSize};
        auto* channelData = buffer.getWritePointer(0);
        at::Tensor frame = torch::from_blob(channelData, sizes);
        frame = torch::reshape(frame, {1,1,modelInputSize});
        std::vector<torch::jit::IValue> inputs;
        inputs.push_back(frame);

//        at::Tensor output = module.forward(inputs).toTensor();

//        auto outputData = output.index({0,torch::indexing::Slice()});
        auto outputData = frame.index({0,0,torch::indexing::Slice()});
        auto outputDataPtr = outputData.data_ptr<float>();

        auto* write = buffer.getWritePointer(0);
        for (int sample = 0; sample < modelInputSize; ++sample) {
            write[sample] = outputDataPtr[sample];
        }
    }

private:
    void loadModel() {
//        std::stringstream modelStream;
//        modelStream.write(BinaryData::model_pt, BinaryData::model_ptSize);
//
//        try {
//            module = torch::jit::load(modelStream);
//        } catch (const c10::Error& e) {
//            std::cerr << "Error loading model\n";
//            std::cerr << e.what() << std::endl;
//            return;
//        }
    }
    torch::jit::script::Module module;
    int modelInputSize;
};


#endif //NN_INFERENCE_TEMPLATE_LIBTORCHPROCESSOR_H
