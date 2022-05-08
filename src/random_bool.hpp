#ifndef RAND_BOOL_HPP
#define RAND_BOOL_HPP

#include <random>
#include <climits>

template<typename PRNG>
class SimpleRandomBool
{
public:
    bool operator()()
    {
        std::uniform_int_distribution dist(0, 1);

        return dist(prng);
    }

private:
    PRNG prng{ std::random_device{}() };
};


template<typename PRNG>
class LastbitRandomBool
{
public:
    bool operator()()
    {
        return prng() & 1;
    }

private:
    PRNG prng{ std::random_device{}() };
};

template<typename PRNG>
class MaskRandomBool
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        if (pool == 1)
        {
            R new_pool = prng();
            bool b = new_pool & leftmost_bit_mask;
            pool = new_pool | leftmost_bit_mask;

            return b;
        }

        bool b = pool & 1;
        pool >>= 1;

        return b;
    }

private:
    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // could be wrong for 32bit generators

    PRNG prng{ std::random_device{}() };
    R pool = prng() | leftmost_bit_mask;
};

#endif