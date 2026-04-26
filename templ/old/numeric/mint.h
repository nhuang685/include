#ifndef TEMPL_MINT_H
#define TEMPL_MINT_H

// to be replaced

#include "bits/stdc++.h"

#include "templ/numeric/ex_eucl.h"

namespace modular {
  template <class Md, class V = long long>
    requires std::signed_integral<std::decay_t<decltype(Md::value)>>
  struct Mod {
    using T = std::decay_t<decltype(Md::value)>;
    T v = 0;

    static constexpr T normalize(std::integral auto val) {
      using U = decltype(Md::value + val);
      U uval = static_cast<U>(val);
      U umd = static_cast<U>(Md::value);
      if (uval <= -umd || umd <= uval)
        uval %= umd;
      if (val < 0)
        uval += umd;
      return static_cast<T>(uval);
    }
    constexpr Mod() : v(0) {}
    constexpr explicit Mod(std::integral auto _val) : v(normalize(_val)) {}

    static inline const Mod ZERO{0};
    static inline const Mod ONE{1};
    static inline const Mod TWO{2};

    // addition
    constexpr Mod& operator+=(Mod b) {
      v += b.v;
      if (v >= Md::value)
        v -= Md::value;
      return *this;
    }
    friend constexpr Mod operator+(Mod a, const Mod b) { return a += b; }
    constexpr Mod& operator++() { return *this += Mod{1}; }
    constexpr Mod operator++(int) {
      Mod res = *this;
      ++(*this);
      return res;
    }

    // subtraction
    constexpr Mod& operator-=(Mod b) {
      v -= b.v;
      if (v < 0)
        v += Md::value;
      return *this;
    }
    friend constexpr Mod operator-(Mod a, const Mod b) { return a -= b; }
    constexpr Mod& operator--() { return *this -= Mod{1}; }
    constexpr Mod operator--(int) {
      Mod res = *this;
      --*this;
      return res;
    }
    // negation
    constexpr Mod operator-() const { return Mod{-v}; }

    // multiplication
    constexpr Mod& operator*=(Mod b) {
      v = static_cast<T>(static_cast<V>(v) * b.v % Md::value);
      return *this;
    }
    friend constexpr Mod operator*(Mod a, Mod b) { return a *= b; }
    constexpr Mod pow(std::integral auto b) const {
      Mod res{1}, a = *this;
      while (b > 0) {
        if (b % 2 == 1)
          res *= a;
        a *= a;
        b /= 2;
      }
      return res;
    }

    // inverse
    constexpr Mod inv() const {
      return Mod{inverse(v, Md::value)};
      // return binpow(Md::value - 2);
    }

    // sqrt
    /*
    std::optional<Mod> sqrt() {
      static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
      Mod c{0};
      while ((c * c - *this).binpow((Md::value - 1) / 2) == Mod{1})
        c = Mod{rng()};
      if (c == Mod{0})
        return std::nullopt;
      std::pair<Mod, Mod> res(Mod{1}, Mod{0}), a(c, Mod{1});
      T b = (Md::value + 1) / 2;
      auto mul = [&c, this](const std::pair<Mod, Mod>& u, const std::pair<Mod, Mod>& v) -> std::pair<Mod, Mod> {
        return {u.first * v.first + u.second * v.second * (c * c - *this), u.second * v.first + u.first * v.second};
      };
      while (b > 0) {
        if (b % 2 == 1)
          res = mul(res, a);
        a = mul(a, a);
        b /= 2;
      }
      return res.first;
      // return std::min(res.first, -res.first);
    }
    */

    // comparison
    constexpr bool operator==(const Mod& b) const = default;
    constexpr std::strong_ordering operator<=>(const Mod& b) const = default;

    // io
    friend std::istream& operator>>(std::istream& in, Mod& a) {
      long long vv;
      in >> vv;
      a = Mod{vv};
      return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const Mod& a) {
      out << a.v;
      return out;
    }

    // conversion
    constexpr T val() const { return v; }
  };
} // namespace modular

#if defined(LOCAL) && __cplusplus >= 202302L
template <class Md, class V>
  requires std::formattable<typename modular::Mod<Md, V>::T, char>
struct std::formatter<modular::Mod<Md, V>, char> {
  using T = typename modular::Mod<Md, V>::T;
  std::formatter<T> underlying;
  constexpr formatter() {
    if constexpr (requires { underlying.set_debug_format(); })
      underlying.set_debug_format();
  }
  template <class ParseContext> constexpr auto parse(ParseContext& ctx) { return underlying.parse(ctx); }
  template <class FormatContext> auto format(const modular::Mod<Md, V>& val, FormatContext& ctx) const {
    return underlying.format(val.val(), ctx);
  }
};
#endif

#endif
