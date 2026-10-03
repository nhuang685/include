#ifndef TEMPL_PSEG_H
#define TEMPL_PSEG_H

#include <bits/stdc++.h>
#include "templ/internal/bit.h"
#include "templ/internal/block.h"

namespace seg {
    template <class S, auto op_, auto e, class F, auto apply_, auto comp, auto id, auto isid, class I = int>
    struct PSeg {
        using UI = std::make_unsigned_t<I>;
        struct Node {
            std::array<Block<Node>, 2> ch;
            S d;
            F la;
        };
        using B = Block<Node>;
        I n, len;
        std::vector<B> rts;
        PSeg() = default;
        explicit PSeg(I n_, int nrts = 0) : n(n_), len(internal::bit_ceil((UI)n)), rts(nrts) {}
        template <class U> explicit PSeg(const std::vector<U>& a, int nrts = 0) : PSeg((int)a.size(), nrts) {
            auto dq = [&](auto&& self, int l, int r) -> B {
                if (l >= n)
                    return B{};
                if (l == r)
                    return make_one(S{a[l]});
                int mid = (l + r) / 2;
                B lhs = self(self, l, mid);
                B rhs = self(self, mid + 1, r);
                return pull(lhs, rhs, r - l + 1);
            };
            rts[0] = dq(dq, 0, len - 1);
        }
        int push() {
            rts.emplace_back();
            return (int)rts.size() - 1;
        }
        int push(int t) {
            rts.push_back(rts[t]);
            return (int)rts.size() - 1;
        }
        void copy(int to, int from) { rts[to] = rts[from]; }
        void set(int t, I i, S val) { rts[t] = set(i, val, rts[t], 0, len - 1); }
        void reset(int t, I l, I r) { rts[t] = reset(l, r, rts[t], 0, len - 1); }
        void copy_range(int to, int from, I l, I r) { rts[to] = copy_range(l, r, rts[to], rts[from], 0, len - 1); }
        void upd(int t, I l, I r, F f) { rts[t] = upd(l, r, f, rts[t], 0, len - 1); }
        S all(int t) { return rts[t] ? rts[t]->d : e(); }
        S query(int t, I l, I r) { return query(l, r, rts[t], 0, len - 1); }
        S get(int t, I i) { return query(t, i, i); }

        template <class... Args> static B make(Args&&... args) { return B::make(std::forward<Args>(args)...); }
        static B make_one(S d) { return make(std::array<B, 2>{}, d, id()); }
        static int sub_size(I l, I r, I li, I ri) { return std::max<I>(0, std::min(ri, r) - std::max(l, li) + 1); }
        static S op(const S& lhs, const S& rhs, I sub1, I sub2) {
            if constexpr (std::is_invocable_v<decltype(op_), S, S, I, I>)
                return op_(lhs, rhs, sub1, sub2);
            else
                return op_(lhs, rhs);
        }
        static void apply(const F& f, B node, I sub) {
            if constexpr (std::is_invocable_v<decltype(apply_), F, S, I>)
                node->d = apply_(f, node->d, sub);
            else
                node->d = apply_(f, node->d);
            node->la = comp(f, node->la);
        }
        static B pull(B lhs, B rhs, I sub) {
            return make(std::array<B, 2>{lhs, rhs}, op(lhs ? lhs->d : e(), rhs ? rhs->d : e(), sub >> 1, sub >> 1),
                        id());
        }
        static void push(B node, I sub) {
            if (!node || isid(node->la))
                return;
            node->ch[0] = B::clone_create(node->ch[0]);
            apply(node->la, node->ch[0], sub >> 1);
            node->ch[1] = B::clone_create(node->ch[1]);
            apply(node->la, node->ch[1], sub >> 1);
            node->la = id();
        }
        static B set(I i, S val, B node, I li, I ri) {
            if (i < li || ri < i)
                return node;
            if (li == ri)
                return make_one(val);
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            B lhs = set(i, val, node ? node->ch[0] : B{}, li, mid);
            B rhs = set(i, val, node ? node->ch[1] : B{}, mid + 1, ri);
            return pull(lhs, rhs, ri - li + 1);
        }
        static B reset(I l, I r, B node, I li, I ri) {
            if (!node || r < li || ri < l)
                return node;
            if (l <= li && ri <= r)
                return B{};
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            B lhs = reset(l, r, node ? node->ch[0] : B{}, li, mid);
            B rhs = reset(l, r, node ? node->ch[1] : B{}, mid + 1, ri);
            return pull(lhs, rhs, ri - li + 1);
        }
        static B copy_range(I l, I r, B to, B from, I li, I ri) {
            if (!to && !from)
                return to;
            if (r < li || ri < l)
                return to;
            if (l <= li && ri <= r)
                return from;
            I mid = (li + ri) / 2;
            push(to, ri - li + 1);
            push(from, ri - li + 1);
            B lhs = copy_range(l, r, to ? to->ch[0] : B{}, from ? from->ch[0] : B{}, li, mid);
            B rhs = copy_range(l, r, to ? to->ch[1] : B{}, from ? from->ch[1] : B{}, mid + 1, ri);
            return pull(lhs, rhs, ri - li + 1);
        }
        static B upd(I l, I r, F f, B node, I li, I ri) {
            if (r < li || ri < l)
                return node;
            if (l <= li && ri <= r) {
                node = B::clone_create(node);
                apply(f, node, ri - li + 1);
                return node;
            }
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            B lhs = upd(l, r, f, node ? node->ch[0] : B{}, li, mid);
            B rhs = upd(l, r, f, node ? node->ch[1] : B{}, mid + 1, ri);
            return pull(lhs, rhs, ri - li + 1);
        }
        static S query(I l, I r, B node, I li, I ri) {
            if (!node || r < li || ri < l)
                return e();
            if (l <= li && ri <= r)
                return node->d;
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            return op(query(l, r, node->ch[0], li, mid), query(l, r, node->ch[1], mid + 1, ri), sub_size(l, r, li, mid),
                      sub_size(l, r, mid + 1, ri));
        }
    };
} // namespace seg

#endif
