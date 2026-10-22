#include "false_sharing.h"

#include <cstdint>
#include <iostream>
#include <thread>

namespace {

bool expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        return false;
    }
    return true;
}

template <typename First, typename Second>
bool increments_are_correct(First& first, Second& second) {
    constexpr std::uint64_t iterations = 100'000;
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

    return first.load(std::memory_order_relaxed) == iterations &&
           second.load(std::memory_order_relaxed) == iterations;
}

}  // namespace

int main() {
    using false_sharing::AdjacentCounters;
    using false_sharing::PaddedCounters;
    using false_sharing::kCacheLineSize;

    static_assert(offsetof(AdjacentCounters, second) < kCacheLineSize);
    static_assert(offsetof(PaddedCounters, second) >= kCacheLineSize);

    AdjacentCounters adjacent;
    PaddedCounters padded;
    const bool passed =
        expect(increments_are_correct(adjacent.first, adjacent.second),
               "相邻布局的计数结果错误") &&
        expect(increments_are_correct(padded.first.value, padded.second.value),
               "填充布局的计数结果错误");

    if (!passed) {
        return 1;
    }
    std::cout << "false-sharing tests passed\n";
}
