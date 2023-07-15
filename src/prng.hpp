#ifndef RANDOM_BOOL_PRNG_HPP
#define RANDOM_BOOL_PRNG_HPP

#include <array>
#include <bit>
#include <limits>
#include <cstdint>

class splitmix64
{
public:
    using result_type = uint64_t;
    using state_type = uint64_t;

    explicit constexpr splitmix64(uint64_t seed) noexcept :
        state_(seed)
    {}

    constexpr result_type operator()() noexcept
    {
        state_ += 0x9e3779b97f4a7c15;
        result_type z = state_;
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
    state_type state_;
};

class xoroshiro128p
{
public:
    using result_type = uint64_t;
    using state_type = std::array<uint64_t, 2>;

    explicit constexpr xoroshiro128p(uint64_t seed) noexcept
    {
        splitmix64 seed_seq_gen(seed);
        state_ = { seed_seq_gen(), seed_seq_gen() };
    }

    constexpr result_type operator()() noexcept
    {
        const auto result = state_[0] + state_[1];
        const auto xstate = state_[0] ^ state_[1];

        state_[0] = std::rotl(state_[0], 24) ^ xstate ^ (xstate << 16);
        state_[1] = std::rotl(xstate, 37);

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
    state_type state_;
};

#endif // !RANDOM_BOOL_PRNG_HPP
