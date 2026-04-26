#include <bits/stdc++.h>

namespace seg {
  template <class Node>
  concept LSegNode = requires(Node n, Node& ch, int k) {
    typename Node::Output;
    requires std::same_as<std::decay_t<decltype(Node::ID)>, typename Node::Output>;
    { std::as_const(n).value() } -> std::same_as<typename Node::Output>;
    { Node::comb(Node::ID, Node::ID) } -> std::same_as<typename Node::Output>;
    Node();
    { n.pull(n, n) } -> std::same_as<void>;
    { n.push(ch, ch, k) } -> std::same_as<void>;
  };

  // based on https://codeforces.com/blog/entry/18051
  template <LSegNode Node> struct LSeg {
    using Output = typename Node::Output;
    int len{}, h{};
    std::vector<Node> vals;

    LSeg() = default;
    explicit LSeg(int len_)
        : len((int)(std::bit_ceil<unsigned int>(len_))), h(std::bit_width<unsigned int>(len)), vals(2 * len) {
      build(0, len - 1);
    }
    template <std::ranges::sized_range Container>
      requires requires(Container c) { Node(*std::ranges::begin(c)); }
    explicit LSeg(const Container& c)
        : len((int)(std::bit_ceil((unsigned int)std::ranges::size(c)))), h(std::bit_width<unsigned int>(len)),
          vals(2 * len) {
      int i = len;
      for (const auto& v : c)
        vals[i++] = Node(v);
      build(0, len - 1);
    }

    void pull(int ind) { vals[ind].pull(vals[2 * ind], vals[2 * ind + 1]); }
    void push_node(int ind, int k) { vals[ind].push(vals[2 * ind], vals[2 * ind + 1], k); }
    void push(int l, int r) {
      l += len, r += len;
      for (int s = h - 1; s >= 1; --s) {
        int k = 1 << s;
        for (int i = (l >> s); i <= (r >> s); ++i)
          push_node(i, k);
      }
    }
    void build(int l, int r) {
      for (l = (l + len) / 2, r = (r + len) / 2; l >= 1; l /= 2, r /= 2) {
        for (int i = l; i <= r; ++i)
          pull(i);
      }
    }

    template <class F, class... Args>
      requires std::invocable<F, Node&, const Args&..., int>
    void upd(const F& f, int l, int r, const Args&... args) {
      if (l > r)
        return;
      push(l, l), push(r, r);

      bool cl = false, cr = false;
      int k = 1;
      for (l += len, r += len; l <= r; l /= 2, r /= 2, k *= 2) {
        if (cl)
          pull(l - 1);
        if (cr)
          pull(r + 1);
        if (l % 2 == 1)
          std::invoke(f, vals[l++], args..., k), cl = true;
        if (r % 2 == 0)
          std::invoke(f, vals[r--], args..., k), cr = true;
      }
      for (--l, ++r; r >= 1; l /= 2, r /= 2) {
        if (cl && l >= 1)
          pull(l);
        if (cr && (!cl || l != r))
          pull(r);
      }
    }
    Output query(int l, int r) {
      if (l > r)
        return Node::ID;
      push(l, l), push(r, r);

      Output ll = Node::ID, rr = Node::ID;
      for (l += len, r += len; l <= r; l /= 2, r /= 2) {
        if (l % 2 == 1)
          ll = Node::comb(ll, vals[l++].value());
        if (r % 2 == 0)
          rr = Node::comb(vals[r--].value(), rr);
      }
      return Node::comb(ll, rr);
    }

    int walk_l(int l, int r, const std::function<bool(const Node&)>& cond) {
      if (l > r)
        return -1;
      push(l, l);
      l += len;
      if (cond(vals[l]))
        return l - len;

      int ind = -1;
      int s = 1;
      for (; l > 1; l /= 2, s *= 2) {
        if (l % 2 == 0 && cond(vals[l + 1])) {
          ind = l + 1;
          break;
        }
      }
      if (ind == -1)
        return -1;

      while (ind < len) {
        vals[ind].push(vals[2 * ind], vals[2 * ind + 1], s);
        if (cond(vals[2 * ind]))
          ind = 2 * ind;
        else if (cond(vals[2 * ind + 1]))
          ind = 2 * ind + 1;
        else
          return -1;
        s /= 2;
      }
      if (cond(vals[ind]) && ind - len <= r)
        return ind - len;
      return -1;
    }
    int walk_r(int l, int r, const std::function<bool(const Node&)>& cond) {
      if (l > r)
        return -1;
      push(r, r);
      r += len;
      if (cond(vals[r]))
        return r - len;

      int ind = -1;
      int s = 1;
      for (; r > 1; r /= 2, s *= 2) {
        if (r % 2 == 1 && cond(vals[r - 1])) {
          ind = r - 1;
          break;
        }
      }
      if (ind == -1)
        return -1;

      while (ind < len) {
        vals[ind].push(vals[2 * ind], vals[2 * ind + 1], s);
        if (cond(vals[2 * ind + 1]))
          ind = 2 * ind + 1;
        else if (cond(vals[2 * ind]))
          ind = 2 * ind;
        else
          return -1;
        s /= 2;
      }
      if (cond(vals[ind]) && ind - len >= l)
        return ind - len;
      return -1;
    }
  };
} // namespace seg
using seg::LSeg;
