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
    inline static std::unique_ptr<int> bufferSize = nullptr;
    inline static std::unique_ptr<AudioPluginAudioProcessor> plugin = nullptr;
    inline static std::unique_ptr<juce::AudioBuffer<float>> buffer = nullptr;
    inline static std::unique_ptr<juce::MidiBuffer> midiBuffer = nullptr;

    void pushSamplesInBuffer() {
    for (int channel = 0; channel < plugin->getTotalNumInputChannels(); channel++) {
        for (int sample = 0; sample < plugin->getBlockSize(); sample++) {
            buffer->setSample(channel, sample, randomSample());
        }
    }

}

    ProcessBlockFixture() {std::cout << "Fixture constructor" << std::endl;}
    ~ProcessBlockFixture() {std::cout << "Fixture destructor" << std::endl;}

    void SetUp(const ::benchmark::State& state);

    void TearDown(const ::benchmark::State& state) {
        std::ignore = state;
    }
};

class SingletonSetup {
public:

    ProcessBlockFixture& fixture;

    SingletonSetup(ProcessBlockFixture& thisFixture, const ::benchmark::State& state) : fixture(thisFixture) {
        auto gui = juce::ScopedJuceInitialiser_GUI {};
        fixture.bufferSize = std::make_unique<int>(0);
        fixture.plugin = std::make_unique<AudioPluginAudioProcessor>();
        fixture.buffer = std::make_unique<juce::AudioBuffer<float>>(2, *fixture.bufferSize);
        fixture.midiBuffer = std::make_unique<juce::MidiBuffer>();
    }

    ~SingletonSetup() {
        fixture.midiBuffer.reset();
        fixture.buffer.reset();
        fixture.plugin.reset();
    }

    static void PerformSetup(ProcessBlockFixture& fixture, const ::benchmark::State& state) {
        static SingletonSetup setup(fixture, state);
        if (*fixture.bufferSize != (int) state.range(0)) {
            *fixture.bufferSize = (int) state.range(0);
            std::cout << "Sample Rate 44100 Hz | Buffer Size " << *fixture.bufferSize << " = " << (float) * fixture.bufferSize * 1000.f/44100.f << " ms" << std::endl;
            fixture.plugin->setPlayConfigDetails(2, 2, 44100, *fixture.bufferSize);
            fixture.plugin->prepareToPlay (44100, (int) *fixture.bufferSize);
            fixture.buffer->setSize(2, (int) *fixture.bufferSize);
        }
    }
};

void ProcessBlockFixture::SetUp(const ::benchmark::State& state) {
    SingletonSetup::PerformSetup(*this, state);
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_ONNX_BACKEND)(benchmark::State& state) {
    plugin->getInferenceThread().setBackend(ONNX);
    for (auto _ : state) {
        state.PauseTiming();
        pushSamplesInBuffer();
        state.ResumeTiming();
        plugin->processBlock(*buffer, *midiBuffer);
        while (plugin->getInferenceThread().isThreadRunning()){
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)(benchmark::State& state) {
    plugin->getInferenceThread().setBackend(LIBTORCH);
    for (auto _ : state) {
        state.PauseTiming();
        pushSamplesInBuffer();
        state.ResumeTiming();
        plugin->processBlock(*buffer, *midiBuffer);
        while (plugin->getInferenceThread().isThreadRunning()){
            std::this_thread::sleep_for(std::chrono::nanoseconds (10));
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_TFLITE_BACKEND)(benchmark::State& state) {
    plugin->getInferenceThread().setBackend(TFLITE);
    for (auto _ : state) {
        state.PauseTiming();
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


/* ============================================================ *
 * =================== REGISTER BANCHMARKs ==================== *
 * ============================================================ */

BENCHMARK(BM_PROCESSOR)->Unit(benchmark::kMillisecond);
BENCHMARK(BM_EDITOR)->Unit(benchmark::kMillisecond);

BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_ONNX_BACKEND)
->Unit(benchmark::kMillisecond)
->Iterations(1)->Repetitions(32)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
  })
->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })
->DisplayAggregatesOnly(true);

BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)
->Unit(benchmark::kMillisecond)
->Iterations(1)->Repetitions(32)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
  })
->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })
->DisplayAggregatesOnly(true);

BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_TFLITE_BACKEND)
->Unit(benchmark::kMillisecond)
->Iterations(1)->Repetitions(32)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
  })
->ComputeStatistics("max", [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
  })
->DisplayAggregatesOnly(true);
BENCHMARK_REGISTER_F(ThreadFixture, BM_THREAD)->Unit(benchmark::kMillisecond)->Iterations(32)->Repetitions(1);