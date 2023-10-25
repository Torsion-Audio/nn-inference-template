#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

TEST(Benchmark, All){
    benchmark::RunSpecifiedBenchmarks();
    std::cout << "--------------------------------------------------------------" << std::endl;
}