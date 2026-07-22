#ifndef TEMPL_MINT_H
#define TEMPL_MINT_H

#include "templ/numeric/ex_eucl.h"

namespace modular {
    using u32 = unsigned int;
    using i64 = long long;
    using u64 = unsigned long long;

    template <u32 MOD> constexpr u32 mul(u32 a, u32 b) { return (u64)a * b % MOD; }
    template <u64 MOD> constexpr u64 mul(u64 a, u64 b) {
        return (a * b - (u64)(1.0L * a * b / MOD - 0.5L) * MOD) % MOD;
    }

    template <std::unsigned_integral S, S MOD> struct SMint {
        S v = 0;
        constexpr SMint() = default;
        template <std::unsigned_integral T> constexpr explicit SMint(T v_) : v(v_ % MOD) {}
        template <std::signed_integral T> constexpr explicit SMint(T v_) {
            using V = std::make_signed_t<S>;
            V val = v_ % (V)MOD;
            if (val < 0)
                val += (V)MOD;
            v = val;
        }
        static constexpr SMint raw(S v) {
            SMint res;
            res.v = v;
            return res;
        }

        constexpr SMint& operator+=(const SMint& rhs) {
            v += rhs.v;
            if (v >= MOD)
                v -= MOD;
            return *this;
        }
        constexpr SMint& operator-=(const SMint& rhs) {
            v -= rhs.v;
            if (v >= MOD)
                v += MOD;
            return *this;
        }
        constexpr SMint& operator*=(const SMint& rhs) {
            v = mul<MOD>(v, rhs.v);
            return *this;
        }
        friend constexpr SMint operator+(SMint lhs, const SMint& rhs) { return lhs += rhs; }
        friend constexpr SMint operator-(SMint lhs, const SMint& rhs) { return lhs -= rhs; }
        friend constexpr SMint operator*(SMint lhs, const SMint& rhs) { return lhs *= rhs; }

        constexpr SMint pow(std::integral auto b) const {
            if (b < 0)
                return inv().pow(-b);
            SMint res{1}, a = *this;
            while (b > 0) {
                if (b % 2 == 1)
                    res *= a;
                a *= a;
                b /= 2;
            }
            return res;
        }

        constexpr SMint inv() const {
            return SMint{inverse<i64>(v, MOD)};
            // return pow(MOD - 2);
        };

        constexpr SMint& operator++() { return *this += SMint{1}; }
        constexpr SMint operator++(int) {
            SMint res = *this;
            ++*this;
            return res;
        }
        constexpr SMint& operator--() { return *this -= SMint{1}; }
        constexpr SMint operator--(int) {
            SMint res = *this;
            --*this;
            return res;
        }
        constexpr SMint operator-() const { return SMint{} - *this; }

        constexpr bool operator==(const SMint& b) const = default;
        constexpr std::strong_ordering operator<=>(const SMint& b) const = default;

        friend std::istream& operator>>(std::istream& in, SMint& a) {
            i64 vv;
            in >> vv;
            a = SMint{vv};
            return in;
        }
        friend std::ostream& operator<<(std::ostream& out, const SMint& a) {
            out << a.v;
            return out;
        }

        constexpr S val() const { return v; }
    };

    using Mint1e9_7 = SMint<u32, 1'000'000'007>;
    using Mint998244353 = SMint<u32, 998'244'353>;
} // namespace modular

#if defined(LOCAL) && __cplusplus >= 202302L
template <std::unsigned_integral S, S MOD>
    requires std::formattable<S, char>
struct std::formatter<modular::SMint<S, MOD>, char> {
    std::formatter<S> underlying;
    constexpr formatter() {
        if constexpr (requires { underlying.set_debug_format(); })
            underlying.set_debug_format();
    }
    template <class ParseContext> constexpr auto parse(ParseContext& ctx) { return underlying.parse(ctx); }
    template <class FormatContext> auto format(const modular::SMint<S, MOD>& val, FormatContext& ctx) const {
        return underlying.format(val.val(), ctx);
    }
};
#endif

#endif // TEMPL_MINT_H
