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

#include "templ/numeric/mint_998244353.h"
#include "templ/range/v8/sseg.h"

using S = Mint;
S op(S l, S r) { return l + r; }
S e() { return Mint{}; }
struct F {
    Mint b, c;
    bool operator==(const F& rhs) const = default;
};
S apply(F f, S val, int sub) { return f.b * val + Mint{sub} * Mint{f.c}; }
F comp(F f, F g) { return F{f.b * g.b, f.b * g.c + f.c}; }
F id() { return F{Mint{1}, Mint{}}; }
using Seg = seg::SSeg<S, op, e, F, apply, comp, id>;

int main() {
#ifndef LOCAL
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
#endif

    int n, q;
    std::cin >> n >> q;

    Seg seg(n);
    while (q--) {
        int op, l, r;
        std::cin >> op >> l >> r;
        --r;
        if (op == 0) {
            Mint b, c;
            std::cin >> b >> c;
            dbg(b, c);
            seg.upd(l, r, F{b, c});
        } else
            std::cout << seg.query(l, r) << '\n';
    }
}
