#ifndef TEMPL_SEG_H
#define TEMPL_SEG_H

#include <bits/stdc++.h>
#include "templ/internal/bit.h"

namespace seg {
    using u32 = unsigned int;

    template <class S, auto op_, auto e> struct Seg {
        Seg() = default;
        explicit Seg(int n_) : Seg(std::vector<S>(n_, e())) {}
        template <class U>
        explicit Seg(const std::vector<U>& a)
            : n((int)a.size()), len((int)internal::bit_ceil((u32)(n))), d(2 * len, e()) {
            for (int i = 0; i < n; ++i)
                d[len + i] = S(a[i]);
            for (int i = len - 1; i >= 1; --i)
                pull(i);
        }

        S all() { return d[1]; }

        void set(int i, const S& val) {
            i += len;
            d[i] = val;
            for (i >>= 1; i >= 1; i >>= 1)
                pull(i);
        }

        S get(int i) { return d[i + len]; }

        S query(int l, int r) {
            if (l > r)
                return e();
            l += len;
            r += len;
            S lv = e(), rv = e();
            int sl = 0, sr = 0, csub = 1;
            for (; l <= r; l >>= 1, r >>= 1, csub <<= 1) {
                if (l & 1) {
                    lv = op(lv, d[l++], sl, csub);
                    sl |= csub;
                }
                if (!(r & 1)) {
                    rv = op(d[r--], rv, csub, sr);
                    sr |= csub;
                }
            }
            return op(lv, rv, sl, sr);
        }

        template <class Pred> int first_true(int l, int r, Pred&& cond) {
            if (l > r)
                return -1;
            l += len;
            if (cond(d[l]))
                return l - len;

            int ind = -1;
            int s = 1;
            for (; l > 1; l /= 2, s *= 2) {
                if (l % 2 == 0 && cond(d[l + 1])) {
                    ind = l + 1;
                    break;
                }
            }
            if (ind == -1)
                return -1;

            while (ind < len) {
                if (cond(d[2 * ind]))
                    ind = 2 * ind;
                else if (cond(d[2 * ind + 1]))
                    ind = 2 * ind + 1;
                else
                    return -1;
                s /= 2;
            }
            if (cond(d[ind]) && ind - len <= r)
                return ind - len;
            return -1;
        }
        template <class Pred> int last_true(int l, int r, Pred&& cond) {
            if (l > r)
                return -1;
            r += len;
            if (cond(d[r]))
                return r - len;

            int ind = -1;
            int s = 1;
            for (; r > 1; r /= 2, s *= 2) {
                if (r % 2 == 1 && cond(d[r - 1])) {
                    ind = r - 1;
                    break;
                }
            }
            if (ind == -1)
                return -1;

            while (ind < len) {
                if (cond(d[2 * ind + 1]))
                    ind = 2 * ind + 1;
                else if (cond(d[2 * ind]))
                    ind = 2 * ind;
                else
                    return -1;
                s /= 2;
            }
            if (cond(d[ind]) && ind - len >= l)
                return ind - len;
            return -1;
        }

      private:
        int n{}, len{};
        std::vector<S> d;
        int comp_sub(int i) const { return len >> internal::log(i); }
        S op(const S& lhs, const S& rhs, int sub1, int sub2) {
            if constexpr (std::is_invocable_v<decltype(op_), S, S, int, int>)
                return op_(lhs, rhs, sub1, sub2);
            else
                return op_(lhs, rhs);
        }
        void pull(int i) {
            int sub = comp_sub(i);
            d[i] = op(d[2 * i], d[2 * i + 1], sub >> 1, sub >> 1);
        }
    };
} // namespace seg

#endif
