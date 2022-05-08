#ifndef RANDOM_BOOL_CONCURRENT_HPP
#define RANDOM_BOOL_CONCURRENT_HPP

#include <atomic>
#include <random>
#include <mutex>
#include <climits>


template<typename PRNG>
class SimpleRandomBoolThreadlocalGen
{
public:
    bool operator()()
    {
        thread_local PRNG prng{ std::random_device{}() };
        std::uniform_int_distribution dist(0, 1);

        return dist(prng);
    }
};


template<typename PRNG>
class LastbitRandomBoolThreadlocalGen
{
public:
    bool operator()()
    {
        thread_local PRNG prng{ std::random_device{}() };

        return prng() & 1;
    }
};

template<typename PRNG>
class MaskRandomBoolLock
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        std::lock_guard guard(lock);
        if (pool == 1)
        {
            pool = prng();
            bool b = pool & leftmost_bit_mask;
            pool |= leftmost_bit_mask;

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
    std::mutex lock;
};


template<typename PRNG>
class AtomicMaskRandomBool
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        R old_val = current.load(std::memory_order::acquire);
        for (;;)
        {
            if ( old_val == 1 &&
                 current.compare_exchange_weak(old_val,
                                               next.load(std::memory_order::relaxed),
                                               std::memory_order::acq_rel,
                                               std::memory_order::acquire)
               )
            {
                R new_next = prng();
                next.store(new_next | leftmost_bit_mask, std::memory_order::release);

                return new_next & leftmost_bit_mask;
            }
            if (old_val == 1) continue;

            R new_val = old_val >> 1;
            if (current.compare_exchange_weak(old_val, new_val, std::memory_order::release, std::memory_order::relaxed))
            {
                return old_val & 1;
            }
        }
    }

private:
    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // could be wrong for 32bit generators

    PRNG prng{ std::random_device{}() };
    std::atomic<R> current = prng() | leftmost_bit_mask;
    std::atomic<R> next = prng() | leftmost_bit_mask;
};


template<typename PRNG>
class AtomicMaskRandomBoolV2
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        R old_val = current.load(std::memory_order::acquire);
        for (;;)
        {
            if (old_val == 1)
            {
                if (!current.compare_exchange_weak(old_val, prng() | leftmost_bit_mask, std::memory_order::acq_rel, std::memory_order::acquire))
                {
                    continue;
                }
            }

            R new_val = old_val >> 1;
            if (current.compare_exchange_weak(old_val, new_val, std::memory_order::release, std::memory_order::relaxed))
            {
                return old_val & 1;
            }
        }
    }

private:
    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // could be wrong for 32bit generators

    PRNG prng{ std::random_device{}() };
    std::atomic<R> current = prng() | leftmost_bit_mask;
};


template<typename PRNG>
class AtomicMaskRandomBoolThreadlocalGen
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        R old_val = current.load(std::memory_order::acquire);
        for (;;)
        {
            if (old_val == 1 &&
                current.compare_exchange_weak(old_val,
                                              next.load(std::memory_order::relaxed),
                                              std::memory_order::acq_rel,
                                              std::memory_order::acquire))
            {
                R new_next = prng();
                next.store(new_next | leftmost_bit_mask, std::memory_order::release);

                return new_next & leftmost_bit_mask;
            }
            if (old_val == 1) continue;

            R new_val = old_val >> 1;
            if (current.compare_exchange_weak(old_val, new_val, std::memory_order::release, std::memory_order::relaxed))
            {
                return old_val & 1;
            }
        }
    }

private:
    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // could be wrong for 32bit generators

    static thread_local PRNG prng;
    std::atomic<R> current = prng() | leftmost_bit_mask;
    std::atomic<R> next = prng() | leftmost_bit_mask;
};

template<typename PRNG>
thread_local PRNG AtomicMaskRandomBoolThreadlocalGen<PRNG>::prng{ std::random_device{}() };

#endif