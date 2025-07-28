#ifndef NN_INFERENCE_TEMPLATE_MODELCONFIG_H
#define NN_INFERENCE_TEMPLATE_MODELCONFIG_H

#include <anira/anira.h>

#ifdef INSTALL_VERSION
#if __linux__
static std::string home = getenv("HOME");
static std::string path_prefix_pytorch = home + std::string("/.config/nn-inference-template/GuitarLSTM/pytorch-version/models/");
static std::string path_prefix_tflite = home + std::string("/.config/nn-inference-template/GuitarLSTM/tensorflow-version/models/");
#elif __APPLE__
static std::string home = getenv("HOME");
static std::string path_prefix_pytorch = home + std::string("/Library/Application Support/nn-inference-template/GuitarLSTM/pytorch-version/models/");
static std::string path_prefix_tflite = home + std::string("/Library/Application Support/nn-inference-template/GuitarLSTM/tensorflow-version/models/");
#elif _WIN32
#include <filesystem>
static std::string path_prefix_pytorch = std::filesystem::path {getenv("APPDATA")}.string() + std::string("/nn-inference-template/GuitarLSTM/pytorch-version/models/");
static std::string path_prefix_tflite = std::filesystem::path {getenv("APPDATA")}.string() + std::string("/nn-inference-template/GuitarLSTM/tensorflow-version/models/");
#endif
#else
static std::string path_prefix_pytorch = std::string(GUITARLSTM_MODELS_PATH_PYTORCH);
static std::string path_prefix_tflite = std::string(GUITARLSTM_MODELS_PATH_TENSORFLOW);
#endif


static std::vector<anira::ModelData> model_data_hybridnn_config = {
#ifdef USE_LIBTORCH
        {path_prefix_pytorch + std::string("model_0/GuitarLSTM-dynamic.pt"), anira::InferenceBackend::LIBTORCH},
#endif
#ifdef USE_ONNXRUNTIME
        {path_prefix_pytorch + std::string("model_0/GuitarLSTM-libtorch-dynamic.onnx"), anira::InferenceBackend::ONNX},
#endif
#ifdef USE_TFLITE
        {path_prefix_tflite + std::string("model_0/GuitarLSTM-256.tflite"), anira::InferenceBackend::TFLITE},
#endif
};

static std::vector<anira::TensorShape> tensor_shape_hybridnn_config = {
#ifdef USE_LIBTORCH
        {{{256, 1, 150}}, {{256, 1}}, anira::InferenceBackend::LIBTORCH},
#endif
#ifdef USE_ONNXRUNTIME
        {{{256, 1, 150}}, {{256, 1}}, anira::InferenceBackend::ONNX},
#endif
#ifdef USE_TFLITE
        {{{256, 150, 1}}, {{256, 1}}, anira::InferenceBackend::TFLITE},
#endif
};

static anira::ProcessingSpec processing_spec_hybridnn_config = {
        {1}, // preprocess_input_channels
        {1}, // postprocess_output_channels
        {256}, // preprocess_input_size
        {256} // postprocess_output_size
};

static anira::InferenceConfig hybridNNConfig (
        model_data_hybridnn_config,
        tensor_shape_hybridnn_config,
        processing_spec_hybridnn_config,
        10.66f,
        3
);

#endif //NN_INFERENCE_TEMPLATE_MODELCONFIG_H
