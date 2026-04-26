#include <bits/stdc++.h>

namespace seg {
  template <class Node>
  concept SegNode = requires(Node n) {
    typename Node::Output;
    Node();
    requires std::same_as<std::decay_t<decltype(Node::ID)>, typename Node::Output>;
    { Node::comb(Node::ID, Node::ID) } -> std::same_as<typename Node::Output>;
    { std::as_const(n).value() } -> std::same_as<typename Node::Output>;
    { n.pull(n, n) } -> std::same_as<void>;
  };

  template <SegNode Node> struct Seg {
    using Output = typename Node::Output;
    int len{};
    std::vector<Node> vals;
    Seg() = default;
    explicit Seg(int len_) : len(len_), vals(2 * len) {}
    // explicit Seg(int _sz)
    //     : sz(static_cast<int>(std::bit_ceil<uint32_t>(_sz))),
    //       val(2 * sz) {}
    template <std::ranges::sized_range Container>
      requires requires(Container c) { Node(*std::ranges::begin(c)); }
    explicit Seg(const Container& c) : Seg(static_cast<int>(std::ranges::size(c))) {
      int i = len;
      for (const auto& v : c)
        vals[i++] = Node(v);

      build(0, len - 1);
    }

    void pull(int ind) { vals[ind].pull(vals[2 * ind], vals[2 * ind + 1]); }
    void build(int l, int r) {
      l += len, r += len;
      l /= 2, r /= 2;
      for (int k = 2; l >= 1; l /= 2, r /= 2, k *= 2) {
        for (int i = l; i <= r; ++i)
          vals[i].pull(vals[2 * i], vals[2 * i + 1]);
      }
    }

    template <class F, class... Args>
      requires std::invocable<F, Node&, const Args&...>
    void upd(const F& f, int i, const Args&... args) {
      std::invoke(f, vals[i += len], args...);
      for (i /= 2; i >= 1; i /= 2)
        pull(i);
    }

    Output query(int l, int r) {
      Output vl = Node::ID, vr = Node::ID;
      for (l += len, r += len; l <= r; l /= 2, r /= 2) {
        if (l % 2 == 1)
          vl = Node::comb(vl, vals[l++].value());
        if (r % 2 == 0)
          vr = Node::comb(vals[r--].value(), vr);
      }
      return Node::comb(vl, vr);
    }
  };
} // namespace seg
using seg::Seg;
