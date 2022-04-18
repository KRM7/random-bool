#include "rand_bool.hpp"
#include "prng.hpp"
#include "concurrent_prng.hpp"
#include <random>
#include "benchmark/benchmark.h"

template<typename BitGen>
void randomBool(benchmark::State& state)
{
    BitGen gen;
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(gen());
    }
}

BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<xorshift64s_concurrent>);

BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<xorshift64s_concurrent>);

BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMask<xorshift64s_concurrent>);

BENCHMARK_MAIN();