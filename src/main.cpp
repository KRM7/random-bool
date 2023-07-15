#include "prng.hpp"
#include "bit_gen.hpp"
#include <benchmark/benchmark.h>
#include <random>

#define BENCHMARK_PRNG_SEED 0x8bd88b12a5e9edda


template<typename BitGenerator, typename PRNG>
static inline void randomBool(benchmark::State& state)
{
    PRNG prng{ BENCHMARK_PRNG_SEED };
    BitGenerator bitgen{};

    benchmark::DoNotOptimize(prng);
    benchmark::DoNotOptimize(bitgen);

    for (auto _ : state)
    {
        bool bit = bitgen(prng);
        benchmark::DoNotOptimize(bit);
    }
}

BENCHMARK_TEMPLATE(randomBool, DummyRandomBool, std::mt19937_64);

BENCHMARK_TEMPLATE(randomBool, IntDistributionRandomBool, std::mt19937_64);
BENCHMARK_TEMPLATE(randomBool, IntDistributionRandomBool, xoroshiro128p);

BENCHMARK_TEMPLATE(randomBool, LSBRandomBool, xoroshiro128p);
BENCHMARK_TEMPLATE(randomBool, MSBRandomBool, xoroshiro128p);

BENCHMARK_TEMPLATE(randomBool, PoolRandomBool63, xoroshiro128p);
BENCHMARK_TEMPLATE(randomBool, PoolRandomBool64, xoroshiro128p);


// -------------------------------------------------------------------------------------

template<typename BitGenerator, typename PRNG>
static inline void concurrentRandomBool(benchmark::State& state)
{
    thread_local PRNG prng{ BENCHMARK_PRNG_SEED };
    thread_local BitGenerator bitgen{};

    benchmark::DoNotOptimize(prng);
    benchmark::DoNotOptimize(bitgen);

    for (auto _ : state)
    {
        bool bit = bitgen(prng);
        benchmark::DoNotOptimize(bit);
    }
}

BENCHMARK_TEMPLATE(concurrentRandomBool, PoolRandomBool63, xoroshiro128p)->ThreadRange(1, 8);
