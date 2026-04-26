#ifndef TEMPL_DSU_H
#define TEMPL_DSU_H

#include <bits/stdc++.h>

struct DSU {
  std::vector<int> vals;
  int cnt{};
  DSU() = default;
  explicit DSU(int n) : vals(n, -1), cnt(n) {}
  int find(int i) { return vals[i] < 0 ? i : (vals[i] = find(vals[i])); }
  bool unite(int u, int v) {
    u = find(u), v = find(v);
    if (u == v)
      return false;
    if (vals[u] > vals[v])
      std::swap(u, v);
    vals[u] += vals[v];
    vals[v] = u;
    --cnt;
    return true;
  }
  bool connected(int u, int v) { return find(u) == find(v); }
  int size(int u) { return -vals[find(u)]; }
  int count() const { return cnt; }
};

#endif
