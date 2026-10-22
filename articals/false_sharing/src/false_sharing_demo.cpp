#include "false_sharing.h"

#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

int main() {
    using false_sharing::AdjacentCounters;
    using false_sharing::PaddedCounters;
    using false_sharing::kCacheLineSize;

    static_assert(offsetof(AdjacentCounters, second) < kCacheLineSize);
    static_assert(offsetof(PaddedCounters, second) >= kCacheLineSize);

    AdjacentCounters adjacent;
    PaddedCounters padded;

    // 两个线程分别写自己的计数器，并返回这次运行的耗时。
    auto run = [](std::atomic<std::uint64_t>& first,
                  std::atomic<std::uint64_t>& second) -> std::uint64_t {
        constexpr std::uint64_t iterations = 20'000'000;
        const auto begin = std::chrono::steady_clock::now();
        std::thread t1([&] {
            for (std::uint64_t i = 0; i < iterations; ++i) {
                first.fetch_add(1, std::memory_order_relaxed);
            }
        });
        std::thread t2([&] {
            for (std::uint64_t i = 0; i < iterations; ++i) {
                second.fetch_add(1, std::memory_order_relaxed);
            }
        });
        t1.join();
        t2.join();
        const auto end = std::chrono::steady_clock::now();
        return static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
    };

    const auto adjacent_ns = run(adjacent.first, adjacent.second);
    const auto padded_ns = run(padded.first.value, padded.second.value);
    const double ratio = static_cast<double>(adjacent_ns) / padded_ns;

    std::cout << "相邻布局: first=" << adjacent.first
              << ", second=" << adjacent.second << '\n'
              << "填充布局: first=" << padded.first.value
              << ", second=" << padded.second.value << '\n'
              << "相邻布局耗时: " << adjacent_ns / 1'000'000.0 << " ms\n"
              << "填充布局耗时: " << padded_ns / 1'000'000.0 << " ms\n"
              << "相邻/填充耗时比: " << ratio << "x\n"
              << "相邻布局的字段偏移: " << offsetof(AdjacentCounters, second)
              << " bytes\n"
              << "填充布局的字段偏移: " << offsetof(PaddedCounters, second)
              << " bytes\n";
}
