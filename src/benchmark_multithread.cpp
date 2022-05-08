#include "random_bool.hpp"
#include "random_bool_concurrent.hpp"
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

constexpr const size_t NTHREADS = 8;

/* Simple method with std distributions */
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBoolThreadlocalGen<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBoolThreadlocalGen<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBoolThreadlocalGen<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBoolThreadlocalGen<xorshift64s>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, SimpleRandomBool<xorshift64s_concurrent>)->Threads(NTHREADS);

/* Use last bit */
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBoolThreadlocalGen<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBoolThreadlocalGen<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBoolThreadlocalGen<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBoolThreadlocalGen<xorshift64s>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, LastbitRandomBool<xorshift64s_concurrent>)->Threads(NTHREADS);

/* Use every bit with mask, locking */
BENCHMARK_TEMPLATE(randomBool, MaskRandomBoolLock<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBoolLock<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBoolLock<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, MaskRandomBoolLock<xorshift64s>)->Threads(NTHREADS);

/* Use every bit with mask, lockfree */
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolThreadlocalGen<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolThreadlocalGen<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolThreadlocalGen<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolThreadlocalGen<xorshift64s>)->Threads(NTHREADS);

BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBool<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBool<xorshift64s_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolV2<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, AtomicMaskRandomBoolV2<xorshift64s_concurrent>)->Threads(NTHREADS);

BENCHMARK_MAIN();