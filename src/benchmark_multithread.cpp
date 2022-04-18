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

constexpr const size_t NTHREADS = 8;

/* Simple method with std distributions */
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimpleThreadlocal<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimpleThreadlocal<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimpleThreadlocal<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimpleThreadlocal<xorshift64s>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolSimple<xorshift64s_concurrent>)->Threads(NTHREADS);

/* Use last bit */
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbitThreadlocal<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbitThreadlocal<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbitThreadlocal<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbitThreadlocal<xorshift64s>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolLastbit<xorshift64s_concurrent>)->Threads(NTHREADS);

/* Use every bit with mask, locking */
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskLock<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskLock<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskLock<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskLock<xorshift64s>)->Threads(NTHREADS);

/* Use every bit with mask, lockfree */
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicThreadlocal<std::mt19937_64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicThreadlocal<xoroshiro128p>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicThreadlocal<splitmix64>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicThreadlocal<xorshift64s>)->Threads(NTHREADS);

BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomic<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomic<xorshift64s_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicV2<splitmix64_concurrent>)->Threads(NTHREADS);
BENCHMARK_TEMPLATE(randomBool, RandomBoolMaskAtomicV2<xorshift64s_concurrent>)->Threads(NTHREADS);

BENCHMARK_MAIN();