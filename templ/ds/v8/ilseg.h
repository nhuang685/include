#ifndef TEMPL_ILSEG_H
#define TEMPL_ILSEG_H

#include "bits/stdc++.h"
#include "templ/internal/bit.h"

namespace seg {
  using u32 = unsigned int;

  template <class T> struct LSeg {
    using S = typename T::S;
    using F = typename T::F;

    LSeg() = default;
    explicit LSeg(int n_) : LSeg(std::vector<S>(n_, T::e())) {}
    template <class U> explicit LSeg(const std::vector<U>& a)
        : n((int)a.size()), len((int)internal::bit_ceil((u32)n)), h(internal::countr_zero((u32)len)),
          d(2 * len, T::e()), la(len, T::id()) {
      for (int i = 0; i < n; ++i)
        d[len + i] = S(a[i]);
      for (int i = len - 1; i >= 1; --i)
        pull(i);
    }

    S all() const { return d[1]; }

    void set(int i, const S& val) {
      i += len;
      for (int s = h; s >= 1; --s)
        push(i >> s);
      d[i] = val;
      for (int s = 1; s <= h; ++s)
        pull(i >> s);
    }

    S get(int i) {
      i += len;
      for (int s = h; s >= 1; --s)
        push(i >> s);
      return d[i];
    }

    void upd(int l, int r, const F& f) {
      if (l > r)
        return;
      l += len;
      r += len;
      push_l(l);
      push_r(r);
      {
        int l2 = l, r2 = r;
        for (; l <= r; l >>= 1, r >>= 1) {
          if (l & 1)
            apply(f, l++);
          if (!(r & 1))
            apply(f, r--);
        }
        l = l2;
        r = r2;
      }
      for (int s = 1; s <= h; ++s) {
        if (((l >> s) << s) != l)
          pull(l >> s);
        if ((((r + 1) >> s) << s) != r + 1)
          pull(r >> s);
      }
    }

    S query(int l, int r) {
      if (l > r)
        return T::e();
      l += len;
      r += len;
      push_l(l);
      push_r(r);
      S lv = T::e(), rv = T::e();
      int sl = 0, sr = 0, csub = 1;
      for (; l <= r; l >>= 1, r >>= 1, csub <<= 1) {
        if (l & 1) {
          lv = T::comb(lv, d[l++], sl, csub);
          sl |= csub;
        }
        if (!(r & 1)) {
          rv = T::comb(d[r--], rv, csub, sr);
          sr |= csub;
        }
      }
      return T::comb(lv, rv, sl, sr);
    }

    template <class Pred> int first_true(int l, int r, Pred&& cond) {
      if (l > r)
        return -1;
      l += len;
      push_l(l);
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
        push(ind);
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
      push_r(r);
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
        push(ind);
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

    std::vector<S> debug() const {
      auto cp = *this;
      std::vector<S> ans(n, T::e());
      for (int i = 0; i < n; ++i)
        ans[i] = cp.get(i);
      return ans;
    }

  private:
    int n{}, len{}, h{};
    std::vector<S> d;
    std::vector<F> la;
    int comp_sub(int i) const { return len >> internal::log(i); }
    void apply(const F& f, int i) {
      d[i] = T::apply(f, d[i], comp_sub(i));
      if (i < len)
        la[i] = T::comp(f, la[i]);
    }
    void pull(int i) {
      int sub = comp_sub(i);
      d[i] = T::comb(d[2 * i], d[2 * i + 1], sub >> 1, sub >> 1);
    }
    void push(int i) {
      apply(la[i], 2 * i);
      apply(la[i], 2 * i + 1);
      la[i] = T::id();
    }
    void push_l(int l) {
      for (int s = h; s >= 1; --s) {
        if (((l >> s) << s) != l)
          push(l >> s);
      }
    }
    void push_r(int r) {
      for (int s = h; s >= 1; --s) {
        if ((((r + 1) >> s) << s) != r + 1)
          push(r >> s);
      }
    }
  };
} // namespace seg

#endif // TEMPL_ILSEG_H
