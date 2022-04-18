#include <random>
#include <atomic>
#include <climits>
#include <thread>
#include <iostream>
#include <bitset>

inline thread_local std::mt19937 prng{ std::random_device{}() };

class MaskRandomBool
{
public:
    using R = std::mt19937::result_type;

    bool operator()()
    {
        if (last == 1)
        {
            std::cout << "Regenerate pool on" << std::this_thread::get_id() << "\n";
            last = prng();
            bool b = last & mask;
            last |= mask;

            return b;
        }
        bool b = last & 1;
        last >>= 1;

        return b;
    }
private:
    R last = 1;
    static constexpr const R mask = R{ 1 } << (4 * CHAR_BIT - 1);
};

class MaskRandomBoolLocking
{
public:
    using R = std::mt19937::result_type;

    bool operator()()
    {
        std::lock_guard guard(lock);
        if (last == 1)
        {
            std::cout << "Regenerate pool on " << std::this_thread::get_id() << "\n";
            last = prng();
            bool b = last & mask;
            last |= mask;

            return b;
        }
        bool b = last & 1;
        last >>= 1;

        return b;
    }

private:
    static constexpr const R mask = R{ 1 } << (4* CHAR_BIT - 1);

    R last = prng() | mask;
    std::mutex lock;
};

class MaskRandomBoolLockFree
{
public:
    using R = std::mt19937::result_type;

    bool operator()()
    {
        for (;;)
        {
            R old_val = current.load();
            if (old_val == 1 && current.compare_exchange_weak(old_val, next.load()))
            {
                std::cout << "New pool on " << std::this_thread::get_id() << "\n";
                R new_next = prng();
                bool b = new_next & 1;
                next.store(new_next | mask);

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
    static constexpr const R mask = R{ 1 } << (4 * CHAR_BIT - 1);

    std::atomic<R> current = prng() | mask;
    std::atomic<R> next = prng() | mask;
};

class MaskRandomBoolLockFreeOrder
{
public:
    using R = std::mt19937::result_type;

    bool operator()()
    {
        for (;;)
        {
            R old_val = current.load(std::memory_order_acquire);
            if (old_val == 1 && current.compare_exchange_weak(old_val, next.load(std::memory_order_acquire), std::memory_order::acq_rel))
            {
                std::cout << "New 2pool on " << std::this_thread::get_id() << "\n";
                R new_next = prng();
                bool b = new_next & 1;
                next.store(new_next | mask, std::memory_order_release);

                return b;
            }
            if (old_val == 1) continue;

            R new_val = old_val >> 1;
            if (current.compare_exchange_weak(old_val, new_val, std::memory_order::acq_rel))
            {
                return old_val & 1;
            }
        }
    }

private:
    static constexpr const R mask = R{ 1 } << (4 * CHAR_BIT - 1);

    std::atomic<R> current = prng() | mask;
    std::atomic<R> next = prng() | mask;
};

inline MaskRandomBoolLocking locking;
inline MaskRandomBoolLockFree lockfree;
inline MaskRandomBoolLockFreeOrder lockfreeorder;

void generateBooleansLocking()
{
    for (size_t i = 0; i < 10; i++)
    {
        locking();
    }
}

void generateBooleansLockfree()
{
    for (size_t i = 0; i < 10; i++)
    {
        lockfree();
    }
}

void generateBooleansLockfreeOrder()
{
    for (size_t i = 0; i < 10; i++)
    {
        lockfreeorder();
    }
}


int main()
{
    std::cout << sizeof(std::mt19937::result_type) * CHAR_BIT << "\n";

    for (size_t i = 0; i < 10; i++)
    {
        std::bitset<sizeof(std::mt19937::result_type) * CHAR_BIT> bits(prng());
        std::cout << bits << "\n";
    }

    {
        std::vector<std::jthread> threads;
        for (size_t i = 0; i < 10; i++)
        {
            threads.emplace_back(generateBooleansLocking);
        }
        std::cout << "________________________________________________________________\n";
    }

    {
        std::vector<std::jthread> threads;
        for (size_t i = 0; i < 10; i++)
        {
            threads.emplace_back(generateBooleansLockfree);
        }
        std::cout << "________________________________________________________________\n";
    }

    {
        std::vector<std::jthread> threads;
        for (size_t i = 0; i < 10; i++)
        {
            threads.emplace_back(generateBooleansLockfreeOrder);
        }
        std::cout << "________________________________________________________________\n";
    }
}