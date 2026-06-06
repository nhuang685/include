#ifndef TEMPL_FENWICK_H
#define TEMPL_FENWICK_H

#include <bits/stdc++.h>

template <class T> struct Fenwick {
  int len = 0;
  std::vector<T> vals;
  Fenwick() = default;
  explicit Fenwick(int len_) : len(len_), vals(len + 1) {}
  void upd(int i, T val) {
    for (++i; i <= len; i += (i & -i))
      vals[i] += val;
  }
  T sum(int r) const {
    T ans{0};
    for (++r; r > 0; r -= (r & -r))
      ans += vals[r];
    return ans;
  }
  T query(int l, int r) const {
    if (l > r)
      return T{0};
    return sum(r) - sum(l - 1);
  }
  template <class Compare> int lower_bound(int val, Compare comp) const {
    const int lg = std::bit_width<unsigned int>(len - 1);
    int pos = 0;
    T tot{0};
    for (int i = lg - 1; i >= 0; --i) {
      if ((pos | (1 << i)) <= len && comp(tot + vals[pos | (1 << i)], val)) {
        pos |= 1 << i;
        tot += vals[pos | (1 << i)];
      }
    }
    return pos;
  }
  int lower_bound(int val) const { return lower_bound(val, std::less{}); }
  template <class Compare> int upper_bound(int val, Compare comp) const {
    const int lg = std::bit_width<unsigned int>(len);
    int pos = 0;
    T tot{0};
    for (int i = lg - 1; i >= 0; --i) {
      if ((pos | (1 << i)) <= len && !comp(val, tot + vals[pos | (1 << i)])) {
        pos |= 1 << i;
        tot += vals[pos | (1 << i)];
      }
    }
    return pos;
  }
  int upper_bound(int val) const { return upper_bound(val, std::less{}); }
};

#endif
