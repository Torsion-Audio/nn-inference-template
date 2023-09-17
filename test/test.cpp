#include <benchmark/benchmark.h>
#include <gtest/gtest.h>

TEST(Benchmark, Client){
    benchmark::RunSpecifiedBenchmarks();
    std::cout << "--------------------------------------------------------------" << std::endl;
}