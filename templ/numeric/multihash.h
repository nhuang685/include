#ifndef TEMPL_MULTIHASH_H
#define TEMPL_MULTIHASH_H

#include "bits/stdc++.h"
#include "templ/numeric/ex_eucl.h"

namespace multihash {
  template <int NUM> struct MultiHash {
    static constexpr int MAX = (int)1e9 + 7;
    // static inline const std::array<int, NUM> mods = []() {
    //   std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    //   std::uniform_int_distribution rnd(10000, MAX);
    //   std::array<int, NUM> ans{};
    //   for (int& v : ans) {
    //     v = rnd(rng);
    //   }
    //   return ans;
    // }();
    static inline std::array<int, NUM> mods;
    static void gen_mods() {
      std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
      std::uniform_int_distribution rnd(10000, MAX);
      for (int& v : mods)
        v = rnd(rng);
    }
    template <class T> static constexpr T normalize(std::integral auto val, T mod) {
      using U = decltype(mod + val);
      U uval = static_cast<U>(val);
      U umd = static_cast<U>(mod);
      if (uval <= -umd || umd <= uval) {
        uval %= umd;
      }
      if (val < 0) {
        uval += umd;
      }
      return static_cast<T>(uval);
    }

    std::array<int, NUM> vals{};
    MultiHash() = default;
    explicit MultiHash(std::integral auto val) {
      for (int i = 0; i < NUM; ++i)
        vals[i] = normalize(val, mods[i]);
    }
    static MultiHash raw(int val) {
      MultiHash hash;
      hash.vals.fill(val);
      return hash;
    }
    MultiHash& operator+=(const MultiHash& rhs) {
      for (int i = 0; i < NUM; ++i) {
        vals[i] += rhs.vals[i];
        if (vals[i] >= mods[i])
          vals[i] -= mods[i];
      }
      return *this;
    }
    friend MultiHash operator+(MultiHash lhs, const MultiHash& rhs) { return lhs += rhs; }
    MultiHash& operator-=(const MultiHash& rhs) {
      for (int i = 0; i < NUM; ++i) {
        vals[i] -= rhs.vals[i];
        if (vals[i] < 0)
          vals[i] += mods[i];
      }
      return *this;
    }
    MultiHash operator-() const {
      MultiHash ans;
      for (int i = 0; i < NUM; ++i)
        ans.vals[i] = normalize(-vals[i], mods[i]);
      return ans;
    }
    friend MultiHash operator-(MultiHash lhs, const MultiHash& rhs) { return lhs -= rhs; }
    MultiHash& operator*=(const MultiHash& rhs) {
      for (int i = 0; i < NUM; ++i)
        vals[i] = (int)((long long)vals[i] * rhs.vals[i] % mods[i]);
      return *this;
    }
    friend MultiHash operator*(MultiHash lhs, const MultiHash& rhs) { return lhs *= rhs; }

    MultiHash binpow(std::integral auto b) const {
      MultiHash res{1}, a = *this;
      while (b > 0) {
        if (b % 2 == 1)
          res *= a;
        a *= a;
        b /= 2;
      }
      return res;
    }
    MultiHash inv() const {
      MultiHash ans;
      for (int i = 0; i < NUM; ++i)
        ans.vals[i] = normalize(inverse(vals[i], mods[i]));
      return ans;
    }
    bool operator==(const MultiHash& rhs) const = default;
    auto operator<=>(const MultiHash& rhs) const = default;
  };

} // namespace multihash

#endif
