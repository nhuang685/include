#ifndef TEMPL_SSEG_H
#define TEMPL_SSEG_H

#include <bits/stdc++.h>
#include "templ/internal/bit.h"
#include "templ/internal/block.h"

namespace seg {
    template <class S, auto op_, auto e, class F, auto apply_, auto comp, auto id, class I = int> struct SSeg {
      private:
        using UI = std::make_unsigned_t<I>;
        struct Node {
            std::array<Block<Node>, 2> ch;
            S d;
            F la;
        };
        using B = Block<Node>;

      public:
        SSeg() = default;
        explicit SSeg(I n_) : n(n_), len(internal::bit_ceil((UI)n)) {}
        template <class U> explicit SSeg(const std::vector<U>& a) : SSeg((int)a.size()) {
            auto dq = [&](auto&& self, B& node, int l, int r) -> void {
                if (!node)
                    node = B::make();
                if (l == r) {
                    node->d = a[l];
                    return;
                }
                int mid = (l + r) / 2;
                self(self, node->ch[0], l, mid);
                if (mid + 1 < n)
                    self(self, node->ch[1], mid + 1, r);
                pull(node, r - l + 1);
            };
            dq(dq, rt, 0, len - 1);
        }
        void set(I i, S val) { rt = set(i, val, rt, 0, len - 1); }
        void upd(I l, I r, F f) { rt = upd(l, r, f, rt, 0, len - 1); }
        S query(I l, I r) { return query(l, r, rt, 0, len - 1); }

      private:
        B rt;
        I n, len;
        static int sub_size(int l, int r, int li, int ri) { return std::max(0, std::min(ri, r) - std::max(l, li) + 1); }
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
        static void pull(B node, I sub) {
            node->d = op(node->ch[0] ? node->ch[0]->d : e(), node->ch[1] ? node->ch[1]->d : e(), sub >> 1, sub >> 1);
        }
        static void push(B node, I sub) {
            if (node->la == id())
                return;
            if (!node->ch[0])
                node->ch[0] = B::make();
            apply(node->la, node->ch[0], sub >> 1);
            if (!node->ch[1])
                node->ch[1] = B::make();
            apply(node->la, node->ch[1], sub >> 1);
            node->la = id();
        }
        static B set(I i, S val, B node, I li, I ri) {
            if (!node)
                node = B::make();
            if (li == ri) {
                node->d = val;
                return node;
            }
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            if (i < mid) {
                node->ch[0] = set(i, val, node->ch[0], li, mid);
            } else {
                node->ch[1] = set(i, val, node->ch[1], mid + 1, ri);
            }
            pull(node, ri - li + 1);
            return node;
        }
        static B upd(I l, I r, F f, B node, I li, I ri) {
            if (r < li || ri < l)
                return node;
            if (!node)
                node = B::make();
            if (l <= li && ri <= r) {
                apply(f, node, ri - li + 1);
                return node;
            }
            I mid = (li + ri) / 2;
            push(node, ri - li + 1);
            node->ch[0] = upd(l, r, f, node->ch[0], li, mid);
            node->ch[1] = upd(l, r, f, node->ch[1], mid + 1, ri);
            pull(node, ri - li + 1);
            return node;
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
