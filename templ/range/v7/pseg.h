#ifndef TEMPL_PSEG_H
#define TEMPL_PSEG_H

namespace seg {
  template <class Node>
  concept PSegNode = requires(Node n, const Node& cn, Pointer<Node> p, const typename Node::Index& k,
                              const typename Node::Output& val) {
    typename Node::Index;
    typename Node::Output;
    std::default_initializable<Node>;
    std::constructible_from<Node, Pointer<Node>, Pointer<Node>, typename Node::Index>;
    { n.ch } -> std::same_as<std::array<Pointer<Node>, 2>&>;
    { cn.ch } -> std::same_as<const std::array<Pointer<Node>, 2>&>;
    { Node::ID } -> std::convertible_to<typename Node::Output>;
    { Node::comb(val, val) } -> std::same_as<typename Node::Output>;
    { cn.value() } -> std::same_as<typename Node::Output>;
    { cn.needs_push() } -> std::same_as<bool>;
    { n.push(k) } -> std::same_as<void>;
  };

  template <PSegNode Node> struct PSeg {
    using P = Pointer<Node>;
    using Index = typename Node::Index;
    using Output = typename Node::Output;

    Index len{};
    std::vector<P> rts;
    PSeg() = default;
    explicit PSeg(Index len_, int nrts) : len((Index)std::bit_ceil<std::make_unsigned_t<Index>>(len_)), rts(nrts) {}
    template <std::ranges::sized_range R>
      requires std::constructible_from<Node, std::ranges::range_value_t<R>>
    explicit PSeg(const R& a, int nrts)
        : len((Index)(std::bit_ceil((std::make_unsigned_t<Index>)std::ranges::size(a)))), rts(nrts) {
      std::vector<P> vals(2 * len);
      {
        Index i = len;
        for (const auto& v : a)
          vals[i++] = P::make(v);
      }
      Index l = len / 2, r = (2 * len - 1) / 2;
      for (Index k = 2; l >= 1; l /= 2, r /= 2, k *= 2) {
        for (Index i = l; i <= r; ++i)
          vals[i] = P::make(vals[2 * i], vals[2 * i + 1], k);
      }
      rts[0] = vals[1];
    }

    void copy_rt(int ti, int tf) { rts[tf] = rts[ti]; }

    static void push(P n, int k) {
      if (!n || !n->needs_push())
        return;
      n->ch[0] = n->ch[0].clone();
      n->ch[1] = n->ch[1].clone();
      n->push(k);
    }

    template <class F, class... Args> static P upd(Index l, Index r, F&& f, P n, Index ll, Index rr, Args&&... args) {
      if (rr < l || r < ll)
        return n;
      if (l <= ll && rr <= r) {
        n = n.clone();
        std::invoke(f, *n, std::forward<Args>(args)..., rr - ll + 1);
        return n;
      }

      Index mid = ll + (rr - ll) / 2;
      push(n, rr - ll + 1);
      return P::make(upd(l, r, std::forward<F>(f), n ? n->ch[0] : P{}, ll, mid, std::forward<Args>(args)...),
                     upd(l, r, std::forward<F>(f), n ? n->ch[1] : P{}, mid + 1, rr, std::forward<Args>(args)...),
                     rr - ll + 1);
    }
    template <class F, class... Args> void upd(F&& f, int ti, int tf, Index l, Index r, Args&&... args) {
      rts[tf] = upd(l, r, std::forward<F>(f), rts[ti], 0, len - 1, std::forward<Args>(args)...);
    }

    static P copy_range(Index l, Index r, P n, P vals, Index ll, Index rr) {
      if (rr < l || r < ll)
        return n;
      if (l <= ll && rr <= r)
        return vals;

      Index mid = ll + (rr - ll) / 2;
      push(n, rr - ll + 1);
      push(vals, rr - ll + 1);
      return P::make(copy_range(l, r, n ? n->ch[0] : P{}, vals ? vals->ch[0] : P{}, ll, mid),
                     copy_range(l, r, n ? n->ch[1] : P{}, vals ? vals->ch[1] : P{}, mid + 1, rr), rr - ll + 1);
    }
    void copy_range(int ti, int tf, int ts, Index l, Index r) {
      rts[tf] = copy_range(l, r, rts[ti], rts[ts], 0, len - 1);
    }

    static Output query(Index l, Index r, P n, Index ll, Index rr) {
      if (!n || rr < l || r < ll)
        return Node::ID;
      if (l <= ll && rr <= r)
        return n->value();

      Index mid = ll + (rr - ll) / 2;
      push(n, rr - ll + 1);
      return Node::comb(query(l, r, n->ch[0], ll, mid), query(l, r, n->ch[1], mid + 1, rr));
    }
    Output query(int t, Index l, Index r) { return query(l, r, rts[t], 0, len - 1); }
  };
} // namespace seg
using seg::PSeg;

#endif
