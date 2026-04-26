#ifndef TEMPL_DSU_RB_H
#define TEMPL_DSU_RB_H

#include <bits/stdc++.h>
#include "templ/utility/inf.h"

struct DSU {
  struct RB {
    int i1, v1, i2, v2;
  };
  std::vector<int> vals;
  std::vector<RB> rb;
  int cnt{};
  DSU() = default;
  explicit DSU(int n) : vals(n, -1), cnt(n) {}
  int find(int i) { return vals[i] < 0 ? i : find(vals[i]); }
  bool unite(int u, int v) {
    u = find(u), v = find(v);
    rb.emplace_back(u, vals[u], v, vals[v]);
    if (u == v)
      return false;
    if (vals[u] > vals[v])
      std::swap(u, v);
    vals[u] += vals[v];
    vals[v] = u;
    --cnt;
    return true;
  }
  void roll() {
    auto [i1, v1, i2, v2] = rb.back();
    rb.pop_back();
    vals[i1] = v1;
    vals[i2] = v2;
  }
  void roll(int m) {
    if (rb.empty())
      return;
    for (int i = 0; i < m; ++i) {
      roll();
      if (rb.empty())
        return;
    }
  }
  void reset() { roll(INF<int>); }
  bool connected(int u, int v) { return find(u) == find(v); }
  int size(int u) { return -vals[find(u)]; }
  int count() const { return cnt; }
};

#endif // TEMPL_DSU_RB_H
