#include <benchmark/benchmark.h>
#include "../source/PluginProcessor.h"
#include "../source/PluginEditor.h"
#include "utils/TestThread.h"

// TODO Make sure that benchmarks also work when HOST_BUFFER_SIZE % MODEL_INPUT_SIZE != 0

/* ============================================================ *
 * ========================= Configs ========================== *
 * ============================================================ */

#define PERCENTILE 0.999

/* ============================================================ *
 * ===================== Helper functions ===================== *
 * ============================================================ */

static float randomSample () {
    return -1.f + (float) (std::rand()) / ((float) (RAND_MAX/2.f));
}

static double calculatePercentile(const std::vector<double>& v, double percentile) {
    // Make sure the data is not empty
    if (v.empty()) {
        throw std::invalid_argument("Input vector is empty.");
    }

    // Sort the data in ascending order
    std::vector<double> sortedData = v;
    std::sort(sortedData.begin(), sortedData.end());

    // Calculate the index for the 99th percentile
    size_t n = sortedData.size();
    size_t percentileIndex = (size_t) (percentile * (n - 1));

    // Check if the index is an integer
    if (percentileIndex == static_cast<size_t>(percentileIndex)) {
        // The index is an integer, return the value at that index
        return sortedData[static_cast<size_t>(percentileIndex)];
    } else {
        // Interpolate between the two nearest values
        size_t lowerIndex = static_cast<size_t>(percentileIndex);
        size_t upperIndex = lowerIndex + 1;
        double fraction = percentileIndex - lowerIndex;
        return (1.0 - fraction) * sortedData[lowerIndex] + fraction * sortedData[upperIndex];
    }
}

const auto calculateMin = [](const std::vector<double>& v) -> double {
    return *(std::min_element(std::begin(v), std::end(v)));
};

const auto calculateMax = [](const std::vector<double>& v) -> double {
    return *(std::max_element(std::begin(v), std::end(v)));
};

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

    ProcessBlockFixture() {}
    ~ProcessBlockFixture() {}

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
        std::ignore = state;
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
    plugin->getInferenceManager().getInferenceThread().setBackend(ONNX);
    for (auto _ : state) {
        state.PauseTiming();
        pushSamplesInBuffer();
        state.ResumeTiming();

        bool init = plugin->getInferenceManager().isInitializing();
        int prevNumReceivedSamples = plugin->getInferenceManager().getNumReceivedSamples();
        plugin->processBlock(*buffer, *midiBuffer);

        if (init) {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples + plugin->getBlockSize()){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
        }
        else {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)(benchmark::State& state) {
    plugin->getInferenceManager().getInferenceThread().setBackend(LIBTORCH);
    for (auto _ : state) {
        state.PauseTiming();
        pushSamplesInBuffer();
        state.ResumeTiming();

        bool init = plugin->getInferenceManager().isInitializing();
        int prevNumReceivedSamples = plugin->getInferenceManager().getNumReceivedSamples();
        plugin->processBlock(*buffer, *midiBuffer);

        if (init) {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples + plugin->getBlockSize()){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
        }
        else {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
        }
    }
}

BENCHMARK_DEFINE_F(ProcessBlockFixture, BM_TFLITE_BACKEND)(benchmark::State& state) {
    plugin->getInferenceManager().getInferenceThread().setBackend(TFLITE);
    for (auto _ : state) {
        state.PauseTiming();
        pushSamplesInBuffer();
        state.ResumeTiming();

        bool init = plugin->getInferenceManager().isInitializing();
        int prevNumReceivedSamples = plugin->getInferenceManager().getNumReceivedSamples();
        plugin->processBlock(*buffer, *midiBuffer);

        if (init) {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples + plugin->getBlockSize()){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
        }
        else {
            while (plugin->getInferenceManager().getNumReceivedSamples() < prevNumReceivedSamples){
                std::this_thread::sleep_for(std::chrono::nanoseconds (10));
            }
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
->Iterations(1)->Repetitions(4000)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", calculateMin)
->ComputeStatistics("max", calculateMax)
->ComputeStatistics("percentile", [](const std::vector<double>& v) -> double {
    return calculatePercentile(v, PERCENTILE);
  })
->DisplayAggregatesOnly(true);

BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_LIBTORCH_BACKEND)
->Unit(benchmark::kMillisecond)
->Iterations(1)->Repetitions(4000)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", calculateMin)
->ComputeStatistics("max", calculateMax)
->ComputeStatistics("percentile", [](const std::vector<double>& v) -> double {
    return calculatePercentile(v, PERCENTILE);
  })
->DisplayAggregatesOnly(true);

BENCHMARK_REGISTER_F(ProcessBlockFixture, BM_TFLITE_BACKEND)
->Unit(benchmark::kMillisecond)
->Iterations(1)->Repetitions(4000)
->RangeMultiplier(2)->Range(128, 8<<10)
->ComputeStatistics("min", calculateMin)
->ComputeStatistics("max", calculateMax)
->ComputeStatistics("percentile", [](const std::vector<double>& v) -> double {
    return calculatePercentile(v, PERCENTILE);
  })
->DisplayAggregatesOnly(true);

// BENCHMARK_REGISTER_F(ThreadFixture, BM_THREAD)
// ->Iterations(1)->Repetitions(1000)
// ->RangeMultiplier(2)->Range(128, 8<<10)
// ->ComputeStatistics("min", calculateMin)
// ->ComputeStatistics("max", calculateMax)
// ->ComputeStatistics("99_percentile", calculatePercentile)
// ->DisplayAggregatesOnly(true);