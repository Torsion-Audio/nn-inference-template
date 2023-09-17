#include <benchmark/benchmark.h>
#include "../source/PluginProcessor.h"
#include "../source/PluginEditor.h"

static void BM_processor_client(benchmark::State& state) {
  // Perform setup here
    auto gui = juce::ScopedJuceInitialiser_GUI {};
  for (auto _ : state) {
    // This code gets timed
    AudioPluginAudioProcessor processor;
  }
}
// Register the function as a benchmark
BENCHMARK(BM_processor_client);

static void BM_editor_client(benchmark::State& state) {
  // Perform setup here
    auto gui = juce::ScopedJuceInitialiser_GUI {};
    AudioPluginAudioProcessor plugin;
  for (auto _ : state) {
    // This code gets timed
    auto editor = plugin.createEditor();
    plugin.editorBeingDeleted (editor);
    delete editor;
  }
}
// Register the function as a benchmark
BENCHMARK(BM_editor_client);