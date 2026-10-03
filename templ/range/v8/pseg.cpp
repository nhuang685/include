/**
 * @author n685
 * @date Wed Sep 16 21:14:21 2026
 */
#include <bits/stdc++.h>

#ifdef LOCAL
#include "dd/debug.h"
#else
#define dbg(...) 67
#define dbg_proj(...) 67
#define dbg_rproj(...) 67
void nline() {}
void bar() {}
void start_clock() {}
void end_clock() {}
#endif

using u32 = unsigned int;
using i64 = long long;
using u64 = unsigned long long;

#include <atcoder/modint.hpp>
using Mint = atcoder::modint998244353;
#include "templ/range/v8/pseg.h"

using S = Mint;
S op(S l, S r) { return l + r; }
S e() { return Mint{}; }
struct F {
    Mint b, c;
};
S apply(F f, S val, int sub) { return f.b * val + Mint{sub} * Mint{f.c}; }
F comp(F f, F g) { return F{f.b * g.b, f.b * g.c + f.c}; }
F id() { return F{Mint{1}, Mint{}}; }
bool isid(F f) { return f.b == Mint{1} && f.c == Mint{}; }
using Seg = seg::PSeg<S, op, e, F, apply, comp, id, isid>;

int main() {
#ifndef LOCAL
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

    Seg::B::vals.reserve(10000000);

    int n, q;
    std::cin >> n >> q;

    std::vector<Mint> a(n);
    for (Mint& v : a) {
        int val;
        std::cin >> val;
        v = Mint{val};
    }

    Seg seg(a, q + 1);
    for (int i = 1; i <= q; ++i) {
        int op, k;
        std::cin >> op >> k;
        ++k;
        if (op == 0) {
            int l, r;
            int b, c;
            std::cin >> l >> r >> b >> c;
            --r;
            seg.copy(i, k);
            seg.upd(i, l, r, F{Mint{b}, Mint{c}});
        } else if (op == 1) {
            int s, l, r;
            std::cin >> s >> l >> r;
            ++s;
            --r;
            seg.copy(i, k);
            seg.copy_range(i, s, l, r);
        } else {
            int l, r;
            std::cin >> l >> r;
            --r;
            std::cout << seg.query(k, l, r).val() << '\n';
        }
    }
}
