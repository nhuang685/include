#ifndef TEMPL_LDOUBLE_H
#define TEMPL_LDOUBLE_H

#include "bits/stdc++.h"

using db = long double;
// [[maybe_unused]] const db PI = std::acos(static_cast<db>(-1.0));
[[maybe_unused]] constexpr db PI = std::numbers::pi_v<db>;
constexpr db EPS = 1e-9;
template <class T, class U> constexpr bool eq(const T& a, const U& b) {
  if constexpr (std::is_floating_point_v<typename std::common_type_t<T, U>>) {
    return std::abs(a - b) < EPS;
  } else {
    return a == b;
  }
}

#endif // TEMPL_LDOUBLE_H
