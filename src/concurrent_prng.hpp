#ifndef CONC_PRNG_HPP
#define CONC_PRNG_HPP

#include <cstdint>
#include <limits>
#include <array>
#include <bit>
#include <atomic>

class xorshift64s_concurrent final
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr xorshift64s_concurrent(uint64_t seed) noexcept
        : state(seed)
    {}

    result_type operator()() noexcept
    {
        state_type old_state = state.load(std::memory_order::relaxed);
        for (;;)
        {
            state_type new_state = old_state;
            new_state ^= new_state >> 12;
            new_state ^= new_state << 25;
            new_state ^= new_state >> 27;

            if (state.compare_exchange_weak(old_state, new_state, std::memory_order::release, std::memory_order::relaxed))
            {
                return new_state * 0x2545F4914F6CDD1DULL;
            }
        }
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
    std::atomic<state_type> state;
};

class splitmix64_concurrent final
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr splitmix64_concurrent(state_type seed) noexcept
        : state(seed)
    {}

    result_type operator()() noexcept
    {
        result_type z = state.fetch_add(0x9e3779b97f4a7c15, std::memory_order::acquire) + 0x9e3779b97f4a7c15;
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
    std::atomic<state_type> state;
};

#endif