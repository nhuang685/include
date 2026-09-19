#ifndef TEMPL_BIT_H
#define TEMPL_BIT_H

#include <bit>

namespace internal {
    using u32 = unsigned int;
    using u64 = unsigned long long;

#if __cplusplus >= 202002L
    using std::bit_width;
    using std::countl_zero;
    using std::countr_zero;
    using std::has_single_bit;
    using std::popcount;
#else
    int countl_zero(u32 val) { return __builtin_clz(val); }
    int countl_zero(u64 val) { return __builtin_clzll(val); }
    int countr_zero(u32 val) { return __builtin_ctz(val); }
    int countr_zero(u64 val) { return __builtin_ctzll(val); }
    int bit_width(u32 val) { return 32 - countl_zero(val); }
    int bit_width(u64 val) { return 64 - countl_zero(val); }
    int popcount(u32 val) { return __builtin_popcount(val); }
    int popcount(u64 val) { return __builtin_popcountll(val); }
    bool has_single_bit(u32 val) { return popcount(val) == 1; }
    bool has_single_bit(u64 val) { return popcount(val) == 1; }
#endif
    int log(u32 val) { return bit_width(val) - 1; }
    int log(u64 val) { return bit_width(val) - 1; }

#if __cplusplus >= 202002L
    using std::bit_ceil;
    using std::bit_floor;
#else
    u32 bit_ceil(u32 val) {
        if (val < 2)
            return 1;
        return 2 << log(val - 1);
    }
    u64 bit_ceil(u64 val) {
        if (val < 2)
            return 1;
        return 2ULL << log(val - 1);
    }
    u32 bit_floor(u32 val) { return 1 << log(val); }
    u64 bit_floor(u64 val) { return 1ULL << log(val); }
#endif
}; // namespace internal

#endif // TEMPL_BIT_H
