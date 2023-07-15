#ifndef RANDOM_BOOL_BIT_GENERATOR_HPP
#define RANDOM_BOOL_BIT_GENERATOR_HPP

#include <random>
#include <cstddef>
#include <cstdint>
#include <climits>


template<typename T>
inline constexpr size_t bitsizeof = CHAR_BIT * sizeof(T);

template<typename T>
inline constexpr T lsb_mask = T{ 1 };

template<typename T>
inline constexpr T msb_mask = T{ 1 } << (bitsizeof<T> - 1);


struct DummyRandomBool
{
    bool operator()(auto&) noexcept { return false; }
};

struct IntDistributionRandomBool
{
    bool operator()(auto& prng) noexcept
    {
        return static_cast<bool>(std::uniform_int_distribution{ 0, 1 }(prng));
    }
};

struct LSBRandomBool
{
    bool operator()(auto& prng) noexcept
    {
        return prng() & lsb_mask<uint64_t>;
    }
};

struct MSBRandomBool
{
    bool operator()(auto& prng) noexcept
    {
        return prng() & msb_mask<uint64_t>;
    }
};

struct PoolRandomBool63
{
    bool operator()(auto& prng) noexcept
    {
        if (bit_pool == lsb_mask<uint64_t>) [[unlikely]]
        {
            bit_pool = prng() | msb_mask<uint64_t>;
        }

        const bool bit = bit_pool & lsb_mask<uint64_t>;
        bit_pool >>= 1;

        return bit;
    }

private:
    uint64_t bit_pool = 1;
};

struct PoolRandomBool64
{
    bool operator()(auto& prng) noexcept
    {
        if (bit_pool == lsb_mask<uint64_t>) [[unlikely]]
        {
            const uint64_t next_pool = prng();
            const bool bit = next_pool & msb_mask<uint64_t>;
            bit_pool = next_pool | msb_mask<uint64_t>;

            return bit;
        }

        const bool bit = bit_pool & lsb_mask<uint64_t>;
        bit_pool >>= 1;

        return bit;
    }

private:
    uint64_t bit_pool = lsb_mask<uint64_t>;
};

#endif // !RANDOM_BOOL_BIT_GENERATOR_HPP
