#ifndef TEMPL_INF_H
#define TEMPL_INF_H

#include "bits/stdc++.h"

template <class T> constexpr T INF = T{};
template <> constexpr double INF<double> = std::numeric_limits<double>::infinity();
template <> constexpr long double INF<long double> = std::numeric_limits<long double>::infinity();
template <> constexpr int INF<int> = 0x3f3f3f3f; // 1061109567
template <> constexpr long long INF<long long> = 0x3f3f3f3f3f3f3f3fLL; // 4557430888798830399

#endif // TEMPL_INF_H
