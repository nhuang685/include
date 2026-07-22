#ifndef TEMPL_BINLIFT_H
#define TEMPL_BINLIFT_H

#include <bits/stdc++.h>

struct Tree {
    int n;
    const int lg;
    const std::vector<std::vector<int>>& adj;
    std::vector<int> par, dep, sub;
    std::vector<std::vector<int>> lift;
    std::vector<int> lt, rt;
    void init(int node, int& cnt) {
        lt[node] = cnt++;
        for (int i : adj[node]) {
            if (i == par[node])
                continue;
            par[i] = node;
            dep[i] = dep[node] + 1;
            init(i, cnt);
            sub[node] += sub[i];
        }
        rt[node] = cnt - 1;
    }
    explicit Tree(const std::vector<std::vector<int>>& adj_)
        : n(static_cast<int>(adj_.size())), lg(std::bit_width<unsigned int>(n - 1)), adj(adj_), par(n, -1), dep(n),
          sub(n, 1), lift(lg, std::vector<int>(n, -1)), lt(n), rt(n) {
        int cnt = 0;
        for (int i = 0; i < n; ++i) {
            if (par[i] == -1) {
                init(i, cnt);
            }
        }
        if (lg == 0) {
            return;
        }
        lift[0] = par;
        for (int i = 1; i < lg; ++i) {
            for (int j = 0; j < n; ++j) {
                if (lift[i - 1][j] != -1) {
                    lift[i][j] = lift[i - 1][lift[i - 1][j]];
                }
            }
        }
    }
    int up(int u, int dist) const {
        if (dist < 0 || dep[u] < dist)
            return -1;
        for (int i = 0; i < lg; ++i) {
            if (((1 << i) & dist) != 0) {
                u = lift[i][u];
            }
        }
        return u;
    }
    // is v an ancestor of u?
    bool ancestor(int u, int v) const { return lt[u] <= lt[v] && lt[v] <= rt[u]; }
    int lca(int u, int v) const {
        if (dep[u] < dep[v])
            std::swap(u, v);
        if (ancestor(v, u))
            return v;
        for (int i = lg - 1; i >= 0; --i) {
            if (dep[u] >= (1 << i) && !ancestor(lift[i][u], v)) {
                u = lift[i][u];
            }
        }
        return par[u];
    }
    int dist(int u, int v) const { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }
};

#endif
