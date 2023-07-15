```shell
clang-cl version 15.0.1 -std:c++20 -O2 -flto -DNDEBUG
```

```shell
random_bool --benchmark_enable_random_interleaving=true --benchmark_repetitions=5
```

```text
-----------------------------------------------------------------------------------------------------------------
Benchmark                                                                       Time             CPU   Iterations
-----------------------------------------------------------------------------------------------------------------
// sequential
randomBool<DummyRandomBool, std::mt19937_64>_median                         0.457 ns        0.469 ns            5
randomBool<IntDistributionRandomBool, std::mt19937_64>_median                3.91 ns         3.92 ns            5
randomBool<IntDistributionRandomBool, xoroshiro128p>_median                  3.24 ns         3.21 ns            5
randomBool<LSBRandomBool, xoroshiro128p>_median                              3.32 ns         3.30 ns            5
randomBool<MSBRandomBool, xoroshiro128p>_median                              3.25 ns         3.21 ns            5
randomBool<PoolRandomBool63, xoroshiro128p>_median                           2.03 ns         2.04 ns            5
randomBool<PoolRandomBool64, xoroshiro128p>_median                           2.52 ns         2.51 ns            5

// concurrent
concurrentRandomBool<PoolRandomBool63, xoroshiro128p>/threads:1_median       2.40 ns         2.40 ns            5
concurrentRandomBool<PoolRandomBool63, xoroshiro128p>/threads:2_median       1.39 ns         2.79 ns            5
concurrentRandomBool<PoolRandomBool63, xoroshiro128p>/threads:4_median      0.965 ns         3.77 ns            5
concurrentRandomBool<PoolRandomBool63, xoroshiro128p>/threads:8_median      0.812 ns         3.81 ns            5
```