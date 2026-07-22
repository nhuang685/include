#ifndef TEMPL_MAXFLOW_H
#define TEMPL_MAXFLOW_H

#include <bits/stdc++.h>

template <class T> struct Flow {
    struct Edge {
        T cap;
        int v;
        int rev = -1;
        Edge(T _cap, int _v) : cap(_cap), v(_v) {}
    };
    int sz, s = -1, t = -1;
    std::vector<std::vector<Edge>> adj;
    explicit Flow(int _sz) : sz(_sz), adj(_sz) {}
    void unite(int u, int v, T c) {
        adj[u].emplace_back(c, v);
        adj[v].emplace_back(0, u);
        adj[u].back().rev = static_cast<int>(adj[v].size()) - 1;
        adj[v].back().rev = static_cast<int>(adj[u].size()) - 1;
    }

    std::vector<int> l, h;
    void bfs() {
        l.assign(sz, static_cast<int>(1e9));
        l[s] = 0;
        std::queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            for (Edge& e : adj[node]) {
                if (e.cap > 0 && l[e.v] > l[node] + 1) {
                    l[e.v] = l[node] + 1;
                    q.push(e.v);
                }
            }
        }
    }
    T dfs(int node, T f) {
        if (node == t) {
            return f;
        }
        for (; h[node] < static_cast<int>(adj[node].size()); ++h[node]) {
            Edge& e = adj[node][h[node]];
            if (e.cap > 0 && l[e.v] == l[node] + 1) {
                T val = dfs(e.v, std::min(f, e.cap));
                if (val) {
                    e.cap -= val;
                    adj[e.v][e.rev].cap += val;
                    return val;
                }
            }
        }
        return 0;
    }
    T max_flow(int _s, int _t) {
        s = _s;
        t = _t;
        bfs();
        T ans = 0;
        while (l[t] != static_cast<int>(1e9)) {
            h.assign(sz, 0);
            // ans += dfs(s, std::numeric_limits<T>::max());
            while (T res = dfs(s, std::numeric_limits<T>::max())) {
                ans += res;
            }
            bfs();
        }
        return ans;
    }
};

#endif
