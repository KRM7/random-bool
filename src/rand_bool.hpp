#ifndef RAND_BOOL_HPP
#define RAND_BOOL_HPP

#include <random>
#include <atomic>
#include <climits>


template<typename PRNG>
class RandomBoolSimple
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
class RandomBoolSimpleThreadlocal
{
public:
    bool operator()()
    {
        std::uniform_int_distribution dist(0, 1);

        return dist(prng);
    }

private:
    static thread_local PRNG prng;
};
template<typename PRNG>
thread_local PRNG RandomBoolSimpleThreadlocal<PRNG>::prng{ std::random_device{}() };


template<typename PRNG>
class RandomBoolLastbit
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
class RandomBoolLastbitThreadlocal
{
public:
    bool operator()()
    {
        return prng() & 1;
    }
private:
    static thread_local PRNG prng;
};
template<typename PRNG>
thread_local PRNG RandomBoolLastbitThreadlocal<PRNG>::prng{ std::random_device{}() };


template<typename PRNG>
class RandomBoolMaskLock
{
public:
    bool operator()()
    {
        std::lock_guard guard(lock);
        if (last == 1)
        {
            last = prng();
            bool b = last & leftmost_bit_mask;
            last |= leftmost_bit_mask;

            return b;
        }
        bool b = last & 1;
        last >>= 1;

        return b;
    }

private:
    PRNG prng{ std::random_device{}() };

    using R = typename PRNG::result_type;
    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1);  // wrong for mt 32bit

    R last = prng() | leftmost_bit_mask;
    std::mutex lock;
};


template<typename PRNG>
class RandomBoolMask
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
    PRNG prng{ std::random_device{}() };

    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // wrong for mt 32bit

    R pool = prng() | leftmost_bit_mask;
};


template<typename PRNG>
class RandomBoolMaskAtomic
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        for (;;)
        {
            R old_val = current.load(std::memory_order::acquire);
            if ( old_val == 1 && current.compare_exchange_weak(old_val, next.load(std::memory_order::acquire), std::memory_order::acq_rel) )
            {
                R new_next = prng();
                bool b = new_next & leftmost_bit_mask;
                next.store(new_next | leftmost_bit_mask, std::memory_order::release);

                return b;
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
    PRNG prng{ std::random_device{}() };

    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // wrong for mt 32bit

    std::atomic<R> current = prng() | leftmost_bit_mask;
    std::atomic<R> next = prng() | leftmost_bit_mask;
};

template<typename PRNG>
class RandomBoolMaskAtomicV2
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        for (;;)
        {
            R old_val = current.load(std::memory_order::acquire);
            if (old_val == 1)
            {
                if (!current.compare_exchange_weak(old_val, prng() | leftmost_bit_mask, std::memory_order::release))
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
    PRNG prng{ std::random_device{}() };

    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1); // wrong for mt 32bit

    std::atomic<R> current = prng() | leftmost_bit_mask;
};


template<typename PRNG>
class RandomBoolMaskAtomicThreadlocal
{
public:
    using R = typename PRNG::result_type;

    bool operator()()
    {
        for (;;)
        {
            R old_val = current.load();
            if (old_val == 1 && current.compare_exchange_weak(old_val, next.load()))
            {
                R new_next = prng();
                bool b = new_next & leftmost_bit_mask;
                next.store(new_next | leftmost_bit_mask);

                return b;
            }
            if (old_val == 1) continue;

            R new_val = old_val >> 1;
            if (current.compare_exchange_weak(old_val, new_val))
            {
                return old_val & 1;
            }
        }
    }

private:
    static thread_local PRNG prng;

    static constexpr const R leftmost_bit_mask = R{ 1 } << (sizeof(R) * CHAR_BIT - 1);

    std::atomic<R> current = prng() | leftmost_bit_mask;
    std::atomic<R> next = prng() | leftmost_bit_mask;
};
template<typename PRNG>
thread_local PRNG RandomBoolMaskAtomicThreadlocal<PRNG>::prng{ std::random_device{}() };

#endif