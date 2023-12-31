#include "TFLiteProcessor.h"

#ifdef _WIN32
#include <comdef.h>
#endif

TFLiteProcessor::TFLiteProcessor()
{
#ifdef _WIN32
    _bstr_t modelPathChar (modelpath.c_str());
    model = TfLiteModelCreateFromFile(modelPathChar);
#else
    model = TfLiteModelCreateFromFile(modelpath.c_str());
#endif

    options = TfLiteInterpreterOptionsCreate();
    interpreter = TfLiteInterpreterCreate(model, options);
}

TFLiteProcessor::~TFLiteProcessor()
{
    TfLiteInterpreterDelete(interpreter);
    TfLiteInterpreterOptionsDelete(options);
    TfLiteModelDelete(model);
}

void TFLiteProcessor::prepareToPlay() {
    TfLiteInterpreterAllocateTensors(interpreter);
    inputTensor = TfLiteInterpreterGetInputTensor(interpreter, 0);
    outputTensor = TfLiteInterpreterGetOutputTensor(interpreter, 0);
    NNInferenceTemplate::InputArray input;
    NNInferenceTemplate::OutputArray output;
    processBlock(input, output);
}

void TFLiteProcessor::processBlock(NNInferenceTemplate::InputArray& input, NNInferenceTemplate::OutputArray& output) {
    TfLiteTensorCopyFromBuffer(inputTensor, input.data(), input.size() * sizeof(float));
    TfLiteInterpreterInvoke(interpreter);
    TfLiteTensorCopyToBuffer(outputTensor, output.data(), output.size() * sizeof(float));
}