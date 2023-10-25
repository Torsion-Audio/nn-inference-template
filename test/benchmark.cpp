#include <benchmark/benchmark.h>
#include "../source/PluginProcessor.h"
#include "../source/PluginEditor.h"

static void BM_PROCESSOR(benchmark::State& state) {
    auto gui = juce::ScopedJuceInitialiser_GUI {};
    for (auto _ : state) {
        AudioPluginAudioProcessor processor;
    }
}

static void BM_EDITOR(benchmark::State& state) {
    auto gui = juce::ScopedJuceInitialiser_GUI {};
    AudioPluginAudioProcessor plugin;
    for (auto _ : state) {
        auto editor = plugin.createEditor();
        plugin.editorBeingDeleted (editor);
        delete editor;
    }
}

class InferenceFixture : public benchmark::Fixture 
{
public:
    std::unique_ptr<AudioPluginAudioProcessor> plugin;
    std::unique_ptr<juce::AudioBuffer<float>> buffer;
    std::unique_ptr<juce::MidiBuffer> midiBuffer;

    InferenceFixture() {
    }

    void SetUp(const ::benchmark::State& state) {
        plugin = std::make_unique<AudioPluginAudioProcessor>();
        buffer = std::make_unique<juce::AudioBuffer<float>>();
        midiBuffer = std::make_unique<juce::MidiBuffer>();
        buffer->setSize (2, 512);
        buffer->clear();
        midiBuffer->clear();
        plugin->prepareToPlay (44100, 1024);
        plugin->processBlock (*buffer, *midiBuffer);
    }

    void TearDown(const ::benchmark::State& state) {
        plugin.reset();
        buffer.reset();
        midiBuffer.reset();
    }
};

BENCHMARK_DEFINE_F(InferenceFixture, BM_ONNX_INFERENCE)(benchmark::State& st) 
{
    for (auto _ : st) 
    {
        plugin->getInferenceThread().testInference(ONNX);
    }
}

BENCHMARK_DEFINE_F(InferenceFixture, BM_LIBTORCH_INFERENCE)(benchmark::State& st) 
{
    for (auto _ : st) 
    {
        plugin->getInferenceThread().testInference(LIBTORCH);
    }
}

// Register the function as a benchmark
BENCHMARK(BM_PROCESSOR)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_EDITOR)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(InferenceFixture, BM_ONNX_INFERENCE)->Unit(benchmark::kMillisecond)->Iterations(10)->Repetitions(1);
BENCHMARK_REGISTER_F(InferenceFixture, BM_LIBTORCH_INFERENCE)->Unit(benchmark::kMillisecond)->Iterations(10)->Repetitions(1);