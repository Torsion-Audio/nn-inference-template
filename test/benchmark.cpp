#include <benchmark/benchmark.h>
#include "../source/PluginProcessor.h"
#include "../source/PluginEditor.h"
#include "utils/TestThread.h"

/* ============================================================ *
 * ===================== Helper functions ===================== *
 * ============================================================ */

static float randomSample () {
    return -1.f + (float) (std::rand()) / ((float) (RAND_MAX/2.f));
}

/* ============================================================ *
 * =================== PROCESSOR AND EDITOR =================== *
 * ============================================================ */

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

/* ============================================================ *
 * ==================== INFERENCE ENGINES ===================== *
 * ============================================================ */

class ProcessBlockFixture : public benchmark::Fixture 
{
public:
    std::unique_ptr<AudioPluginAudioProcessor> plugin;
    std::unique_ptr<juce::AudioBuffer<float>> buffer;
    std::unique_ptr<juce::MidiBuffer> midiBuffer;

    void pushSamplesInBuffer() {
    for (int channel = 0; channel < plugin->getTotalNumInputChannels(); channel++) {
        for (int sample = 0; sample < plugin->getBlockSize(); sample++) {
            buffer->setSample(channel, sample, randomSample());
        }
    }

}

    ProcessBlockFixture() {
    }

    void SetUp(const ::benchmark::State& state) {
        plugin = std::make_unique<AudioPluginAudioProcessor>();
        buffer = std::make_unique<juce::AudioBuffer<float>>(2, (int) state.range(0));
        midiBuffer = std::make_unique<juce::MidiBuffer>();
        plugin->setPlayConfigDetails(2, 2, 44100, (int) state.range(0));
        plugin->prepareToPlay (44100, (int) state.range(0));
    }

    void TearDown(const ::benchmark::State& state) {
        std::cout << "Buffer size: " << state.range(0) << " samples | " << plugin->getBlockSize() * 1000.f/44100.f << " ms" << std::endl;
        plugin.reset();
        buffer.reset();
        midiBuffer.reset();
    }
};

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_ONNX_BACKEND)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().setBackend(ONNX);
        pushSamplesInBuffer();
        state.ResumeTiming();
        plugin->processBlock(*buffer, *midiBuffer);
        while (plugin->getInferenceThread().isThreadRunning()){
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().setBackend(LIBTORCH);
        pushSamplesInBuffer();
        state.ResumeTiming();
        plugin->processBlock(*buffer, *midiBuffer);
        while (plugin->getInferenceThread().isThreadRunning()){
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_TFLITE_BACKEND)(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        plugin->getInferenceThread().setBackend(TFLITE);
        pushSamplesInBuffer();
        state.ResumeTiming();
        plugin->processBlock(*buffer, *midiBuffer);
        while (plugin->getInferenceThread().isThreadRunning()){
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}

/* ============================================================ *
 * ======================= JUCE THREAD ======================== *
 * ============================================================ */

class ThreadFixture : public benchmark::Fixture
{
public:
    std::unique_ptr<TestThread> thread;

    ThreadFixture() {
        auto gui = juce::ScopedJuceInitialiser_GUI {};
    }

    void SetUp(const ::benchmark::State& state) {
        thread = std::make_unique<TestThread>();
        std::ignore = state;
    }

    void TearDown(const ::benchmark::State& state) {
        thread.reset();
        std::ignore = state;
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
BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_ONNX_BACKEND)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_TFLITE_BACKEND)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1)->RangeMultiplier(2)->Range(128, 8<<10);
BENCHMARK_REGISTER_F(ThreadFixture, BM_THREAD)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1);