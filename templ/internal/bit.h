#ifndef TEMPL_BIT_H
#define TEMPL_BIT_H

#include <bit>

namespace internal {
  using u32 = unsigned int;

#if __cplusplus >= 202002L
  using std::bit_width;
  using std::countl_zero;
  using std::countr_zero;
  using std::popcount;
#else
  int countl_zero(u32 val) { return __builtin_clz(val); }
  int countr_zero(u32 val) { return __builtin_ctz(val); }
  int bit_width(u32 val) { return 32 - countl_zero(val); }
  int popcount(u32 val) { return __builtin_popcount(val); }
#endif
  int log(u32 val) { return bit_width(val) - 1; }
  bool is_power_2(u32 val) { return popcount(val) == 1; }

#if __cplusplus >= 202002L
  using std::bit_ceil;
  using std::bit_floor;
#else
  u32 bit_ceil(u32 val) {
    if (val < 2)
      return 1;
    return 2 << log(val - 1);
  }
  u32 bit_floor(u32 val) { return 1 << log(val); }
#endif
}; // namespace internal

#endif // TEMPL_BIT_H
