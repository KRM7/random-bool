#ifndef PRNG_HPP
#define PRNG_HPP

#include <limits>
#include <cstdint>
#include <array>
#include <bit>

class splitmix64 final
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr splitmix64(state_type seed) noexcept
        : state(seed)
    {}

    constexpr result_type operator()() noexcept
    {
        state += 0x9e3779b97f4a7c15;
        result_type z = state;
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

    static constexpr result_type min() noexcept
    {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() noexcept
    {
        return std::numeric_limits<result_type>::max();
    }

private:
    state_type state;
};

class xoroshiro128p final
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr xoroshiro128p(uint64_t seed) noexcept
    {
        splitmix64 seed_seq_gen(seed);
        state[0] = seed_seq_gen();
        state[1] = seed_seq_gen();
    }

    explicit constexpr xoroshiro128p(const std::array<state_type, 2>& state) noexcept
        : state(state)
    {}

    constexpr result_type operator()() noexcept
    {
        const state_type s0 = state[0];
        state_type s1 = state[1];
        const result_type result = s0 + s1;

        s1 ^= s0;
        state[0] = std::rotl<state_type>(s0, 24U) ^ s1 ^ (s1 << 16);
        state[1] = std::rotl<state_type>(s1, 37U);

        return result;
    }

    static constexpr result_type min() noexcept
    {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() noexcept
    {
        return std::numeric_limits<result_type>::max();
    }

private:
    std::array<state_type, 2> state;
};

class xorshift64s final
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr xorshift64s(uint64_t seed) noexcept
        : state(seed)
    {}

    constexpr result_type operator()() noexcept
    {
        state ^= state >> 12;
        state ^= state << 25;
        state ^= state >> 27;
        return state * 0x2545F4914F6CDD1DULL;
    }

    static constexpr result_type min() noexcept
    {
        return std::numeric_limits<result_type>::min();
    }
    static constexpr result_type max() noexcept
    {
        return std::numeric_limits<result_type>::max();
    }

private:
    state_type state;
};

#endif