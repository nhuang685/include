#ifndef TEMPL_VTREE_H
#define TEMPL_VTREE_H
#include <bits/stdc++.h>
#include "templ/graph/binlift.h"

class VT {
private:
  Tree t;
  int sz, cnt = 0;
  std::vector<int> ind;
  std::vector<std::vector<int>> vtadj;
  std::vector<int> p;

public:
  explicit VT(Tree t_) : t(std::move(t_)), sz(static_cast<int>(t.adj.size())), ind(sz) {}
  void init(std::vector<int> v) {
    v.push_back(0);
    std::sort(v.begin(), v.end(), [this](int a, int b) { return t.lt[a] < t.lt[b]; });
    v.erase(std::unique(v.begin(), v.end()), v.end());
    int vsz = static_cast<int>(v.size());
    for (int i = 0; i < vsz - 1; ++i) {
      v.push_back(t.lca(v[i], v[i + 1]));
    }
    std::sort(v.begin(), v.end(), [this](int a, int b) { return t.lt[a] < t.lt[b]; });
    v.erase(std::unique(v.begin(), v.end()), v.end());

    for (cnt = 0; cnt < static_cast<int>(v.size()); ++cnt) {
      ind[v[cnt]] = cnt;
    }
    vtadj.resize(cnt);
    p.resize(cnt);

    std::stack<int, std::vector<int>> s;
    for (int node : v) {
      if (!vtadj[ind[node]].empty()) {
        vtadj[ind[node]].clear();
      }
      if (node == 0) {
        s.push(0);
        continue;
      }
      int par = t.lca(s.top(), node);
      while (!s.empty() && t.dep[par] <= t.dep[s.top()]) {
        s.pop();
      }
      if (t.dep[par] < t.dep[node]) {
        vtadj[ind[par]].push_back(ind[node]);
        vtadj[ind[node]].push_back(ind[par]);
        p[ind[node]] = ind[par];
      }
      s.push(node);
    }
  }

  const auto& operator[](int node) { return ind[node]; }
  const auto& adj(int node) { return vtadj[ind[node]]; }
  const auto& adj() { return vtadj; }
  int par(int i) { return p[ind[i]]; }
};

#endif
