#ifndef TEMPL_DSU_H
#define TEMPL_DSU_H

#include <bits/stdc++.h>

struct DSU {
  std::vector<int> val;
  int cnt{};
  DSU() = default;
  explicit DSU(int n) : val(n, -1), cnt(n) {}
  int find(int i) { return val[i] < 0 ? i : (val[i] = find(val[i])); }
  bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v)
      return false;
    if (val[u] > val[v])
      std::swap(u, v);
    val[u] += val[v];
    vals[v] = u;
    --cnt;
    return true;
  }
  bool connected(int u, int v) { return find(u) == find(v); }
  int size(int u) { return -vals[find(u)]; }
  int count() const { return cnt; }
};

#endif
