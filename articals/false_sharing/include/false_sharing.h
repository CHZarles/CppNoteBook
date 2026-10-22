#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

namespace false_sharing {

// 这里按常见的 64 字节 Cache line 展示布局差异。
constexpr std::size_t kCacheLineSize = 64;

// 两个原子变量连续排列，通常会落在同一条 Cache line 中。
struct AdjacentCounters {
    std::atomic<std::uint64_t> first{0};
    std::atomic<std::uint64_t> second{0};
};

// 每个计数器都从 Cache line 边界开始，避免两个计数器共享 Cache line。
struct alignas(kCacheLineSize) PaddedCounter {
    std::atomic<std::uint64_t> value{0};
};

struct PaddedCounters {
    PaddedCounter first;
    PaddedCounter second;
};

}  // namespace false_sharing
