#ifndef TEMPL_CC_H
#define TEMPL_CC_H

#include <bits/stdc++.h>

template <class T> struct CC {
  std::vector<T> vals;
  void push(T a) { vals.push_back(a); }
  void init() {
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
  }
  int get(T a) const { return (int)std::distance(vals.begin(), std::lower_bound(vals.begin(), vals.end(), a)); }
  int exact(T a) const {
    int i = get(a);
    return i < size() && vals[i] == a ? i : -1;
  }
  int round_up(T a) const { return get(a); }
  int round_down(T a) const {
    return (int)std::distance(vals.begin(), std::upper_bound(vals.begin(), vals.end(), a)) - 1;
  }
  T value(int i) const { return vals[i]; }
  int size() const { return (int)vals.size(); }
};

#endif
