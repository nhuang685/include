#ifndef TEMPL_ILSEG_H
#define TEMPL_ILSEG_H

namespace seg {
  template <class Node>
  concept LSegNode = requires(Node n, const Node& cn, Node& ch, int k, const typename Node::Output& val) {
    typename Node::Output;
    std::default_initializable<Node>;
    { Node::ID } -> std::convertible_to<typename Node::Output>;
    { Node::comb(val, val) } -> std::same_as<typename Node::Output>;
    { cn.value() } -> std::same_as<typename Node::Output>;
    { n.pull(cn, cn, k) } -> std::same_as<void>;
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
    template <std::ranges::sized_range R>
      requires std::constructible_from<Node, std::ranges::range_value_t<R>>
    explicit LSeg(const R& a)
        : len((int)(std::bit_ceil((unsigned int)std::ranges::size(a)))), h(std::bit_width<unsigned int>(len)),
          vals(2 * len) {
      int i = len;
      for (const auto& v : a)
        vals[i++] = Node(v);
      build(0, len - 1);
    }

    void pull(int ind, int k) { vals[ind].pull(vals[2 * ind], vals[2 * ind + 1], k); }
    void build(int l, int r) {
      l += len, r += len;
      l /= 2, r /= 2;
      for (int k = 2; l >= 1; l /= 2, r /= 2, k *= 2) {
        for (int i = l; i <= r; ++i)
          pull(i, k);
      }
    }

    void push_node(int ind, int k) { vals[ind].push(vals[2 * ind], vals[2 * ind + 1], k); }
    void push(int l, int r) {
      l += len, r += len;
      for (int s = h - 1; s >= 1; --s) {
        int k = 1 << s;
        for (int i = (l >> s); i <= (r >> s); ++i)
          push_node(i, k);
      }
    }

    template <class F, class... Args>
      requires std::invocable<F, Node&, Args..., int>
    void upd(F&& f, int l, int r, Args&&... args) {
      if (l > r)
        return;
      push(l, l), push(r, r);

      bool cl = false, cr = false;
      int k = 1;
      for (l += len, r += len; l <= r; l /= 2, r /= 2, k *= 2) {
        if (cl)
          pull(l - 1, k);
        if (cr)
          pull(r + 1, k);
        if (l % 2 == 1)
          std::invoke(f, vals[l++], std::forward<Args>(args)..., k), cl = true;
        if (r % 2 == 0)
          std::invoke(f, vals[r--], std::forward<Args>(args)..., k), cr = true;
      }
      for (--l, ++r; r >= 1; l /= 2, r /= 2, k *= 2) {
        if (cl && l >= 1)
          pull(l, k);
        if (cr && (!cl || l != r))
          pull(r, k);
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
    Output query() const { return vals[1].value(); }

    template <class Pred> int first_true(int l, int r, Pred&& cond) {
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
    template <class Pred> int last_true(int l, int r, Pred&& cond) {
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

#ifdef LOCAL
    void debug(int l, int r) {
      std::vector<Output> v;
      LSeg ls = *this;
      for (int i = l; i <= r; ++i)
        v.push_back(ls.query(i, i));
      dbg(v);
    }
#else
    void debug(int l, int r) {}
#endif
  };
} // namespace seg
using seg::LSeg;

#endif
