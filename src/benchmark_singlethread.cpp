#include "random_bool.hpp"
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

BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<xorshift64s_concurrent>);

BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<xorshift64s_concurrent>);

BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<std::mt19937_64>);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<xoroshiro128p>);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<splitmix64>);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<splitmix64_concurrent>);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<xorshift64s>);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBool<xorshift64s_concurrent>);

BENCHMARK_MAIN();