//
// Created by valentin.ackva on 29.09.2023.
//

#include "OnnxRuntimeProcessor.h"

OnnxRuntimeProcessor::OnnxRuntimeProcessor() : session(nullptr) {
    loadModel();
}

OnnxRuntimeProcessor::~OnnxRuntimeProcessor() {
    session.release();
}

void OnnxRuntimeProcessor::setModelInputSize(int size) {
    modelInputSize = size;
    onnxInputData.resize(size, 0.0f);
    onnxOutputData.resize(size, 0.0f);
}

void OnnxRuntimeProcessor::loadModel() {
    Ort::SessionOptions sessionOptions;

//    session = Ort::Session(env,
//                           BinaryData::model_onnx,
//                           BinaryData::model_onnxSize,
//                           sessionOptions);
}

void OnnxRuntimeProcessor::process(juce::AudioBuffer<float> &buffer) {

    auto readInput = buffer.getReadPointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        onnxInputData[sample] = readInput[sample];
    }

    Ort::AllocatorWithDefaultOptions ort_alloc;

    Ort::AllocatedStringPtr inputName = session.GetInputNameAllocated(0, ort_alloc);
    Ort::AllocatedStringPtr outputName = session.GetOutputNameAllocated(0, ort_alloc);
    const std::array<const char *, 1> inputNames = {(char*) inputName.get()};
    const std::array<const char *, 1> outputNames = {(char*) outputName.get()};

    //process
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtDeviceAllocator, OrtMemTypeCPU);

    // define shape
    std::array<int64_t, 3> inputShape = {1, 1, modelInputSize};
    std::array<int64_t, 3> outputShape = {1, 1, modelInputSize};

    auto inputTensor = std::make_shared<Ort::Value>(Ort::Value::CreateTensor<float>(memory_info,
                                                                                    onnxInputData.data(),
                                                                                    modelInputSize,
                                                                                    inputShape.data(),
                                                                                    inputShape.size()));

    auto outputTensor = std::make_shared<Ort::Value>(Ort::Value::CreateTensor<float>(memory_info,
                                                                                     onnxOutputData.data(),
                                                                                     modelInputSize,
                                                                                     outputShape.data(),
                                                                                     outputShape.size()));

    // run inference
    try {
        session.Run(runOptions, inputNames.data(), inputTensor.get(), 1, outputNames.data(), outputTensor.get(), 1);
    } catch (Ort::Exception &e) {
        std::cout << e.what() << std::endl;
    }

    auto writeOutput = buffer.getWritePointer(0);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        writeOutput[sample] = onnxOutputData[sample];
    }
}


