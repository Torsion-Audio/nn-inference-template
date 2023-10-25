#include <benchmark/benchmark.h>
#include "../source/PluginProcessor.h"
#include "../source/PluginEditor.h"
#include "TestThread.h"

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

    InferenceFixture() {
    }

    void SetUp(const ::benchmark::State& state) {
        plugin = std::make_unique<AudioPluginAudioProcessor>();
        plugin->prepareToPlay (44100, (int) state.range(0));
    }

    void TearDown(const ::benchmark::State& state) {
        plugin.reset();
        std::cout << "Buffer size: " << state.range(0) << " samples | " << state.range(0) * 1000.f/44100.f << " ms" << std::endl;
    }
};

BENCHMARK_DEFINE_F(InferenceFixture, BM_ONNX_INFERENCE)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().testPushSamples((int) state.range(0));
        state.ResumeTiming();
        plugin->getInferenceThread().testInference(ONNX);
    }
}

BENCHMARK_DEFINE_F(InferenceFixture, BM_LIBTORCH_INFERENCE)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().testPushSamples((int) state.range(0));
        state.ResumeTiming();
        plugin->getInferenceThread().testInference(LIBTORCH);
    }
}

BENCHMARK_DEFINE_F(InferenceFixture, BM_TFLITE_INFERENCE)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().testPushSamples((int) state.range(0));
        state.ResumeTiming();
        plugin->getInferenceThread().testInference(TFLITE);
    }
}


// --- Benchmarking threads ---

class ThreadFixture : public benchmark::Fixture
{
public:
    std::unique_ptr<TestThread> thread;

    ThreadFixture() { }

    void SetUp(const ::benchmark::State& state) {
        thread = std::make_unique<TestThread>();
    }

    void TearDown(const ::benchmark::State& state) {
        thread.reset();
    }
};


BENCHMARK_DEFINE_F(ThreadFixture, BM_THREAD)(benchmark::State& state) {
    for (auto _ : state) {
        thread->startThread(juce::Thread::Priority::highest);

        while (thread->isThreadRunning()) {
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}



// Register the function as a benchmark
BENCHMARK(BM_PROCESSOR)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_EDITOR)->Unit(benchmark::kMillisecond);
BENCHMARK_REGISTER_F(InferenceFixture, BM_ONNX_INFERENCE)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(InferenceFixture, BM_LIBTORCH_INFERENCE)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(InferenceFixture, BM_TFLITE_INFERENCE)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(ThreadFixture, BM_THREAD)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1);