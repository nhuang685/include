#include <bits/stdc++.h>

template <class T> struct Fenwick {
  int len;
  std::vector<T> val;
  static T comb(T a, T b) { return a + b; }
  static T inv(T a) { return -a; }
  Fenwick() : len(0) {}
  explicit Fenwick(int len_) : len(len_), val(len + 1) {}
  void upd(int i, T a) {
    i++;
    for (; i <= len; i += (i & -i))
      val[i] = comb(val[i], a);
  }
  T sum(int r) const {
    r++;
    T ans{0};
    for (; r > 0; r -= (r & -r))
      ans = comb(ans, val[r]);
    return ans;
  }
  T query(int l, int r) const {
    if (l > r)
      return T{0};
    return comb(inv(sum(l - 1)), sum(r));
  }
};
