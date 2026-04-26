#ifndef TEMPL_PSEG_H
#define TEMPL_PSEG_H

namespace seg {
  template <class Node>
  concept SegNode = requires(Node n, const Node& cn, int k, const typename Node::Output& val) {
    typename Node::Output;
    requires std::default_initializable<Node>;
    { Node::ID } -> std::convertible_to<typename Node::Output>;
    { Node::comb(val, val) } -> std::same_as<typename Node::Output>;
    { cn.value() } -> std::same_as<typename Node::Output>;
    { n.pull(cn, cn, k) } -> std::same_as<void>;
  };

  template <SegNode Node> struct Seg {
    using Output = typename Node::Output;
    int len{};
    std::vector<Node> vals;
    Seg() = default;
#ifndef SEG_POWER_2
    explicit Seg(int len_) : len(len_), vals(2 * len) {}
#else
    explicit Seg(int len_) : len((int)std::bit_ceil<unsigned int>(len_)), vals(2 * len) {}
#endif
    template <std::ranges::sized_range R>
      requires std::constructible_from<Node, std::ranges::range_value_t<R>>
    explicit Seg(const R& a) : Seg((int)std::ranges::size(a)) {
      int i = len;
      for (const auto& v : a)
        vals[i++] = Node(v);
      build(0, len - 1);
    }

    // TODO: verify logic with k is correct for non powers of 2
    void pull(int ind, int k) { vals[ind].pull(vals[2 * ind], vals[2 * ind + 1], k); }
    void build(int l, int r) {
      l += len, r += len;
      l /= 2, r /= 2;
      for (int k = 2; l >= 1; l /= 2, r /= 2, k *= 2) {
        for (int i = l; i <= r; ++i)
          pull(i, k);
      }
    }

    template <class F, class... Args>
      requires std::invocable<F, Node&, Args...>
    void upd(F&& f, int i, Args&&... args) {
      std::invoke(f, vals[i += len], std::forward<Args>(args)...);
      int k = 2;
      for (i /= 2; i >= 1; i /= 2, k *= 2)
        pull(i, k);
    }

    Output query(int l, int r) const {
      Output vl = Node::ID, vr = Node::ID;
      for (l += len, r += len; l <= r; l /= 2, r /= 2) {
        if (l % 2 == 1)
          vl = Node::comb(vl, vals[l++].value());
        if (r % 2 == 0)
          vr = Node::comb(vals[r--].value(), vr);
      }
      return Node::comb(vl, vr);
    }
    Output query() const { return vals[1].value(); }

#ifdef SEG_POWER_2
    template <class Pred> int first_true(int l, Pred&& cond) {
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
        if (cond(vals[2 * ind]))
          ind = 2 * ind;
        else if (cond(vals[2 * ind + 1]))
          ind = 2 * ind + 1;
        else
          return -1;
        s /= 2;
      }
      if (cond(vals[ind]))
        return ind - len;
      return -1;
    }
    template <class Pred> int last_true(int r, Pred&& cond) {
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
        if (cond(vals[2 * ind + 1]))
          ind = 2 * ind + 1;
        else if (cond(vals[2 * ind]))
          ind = 2 * ind;
        else
          return -1;
        s /= 2;
      }
      if (cond(vals[ind]))
        return ind - len;
      return -1;
    }
#endif

#ifdef LOCAL
    void debug(int n) {
      std::vector<Output> v;
      for (int i = 0; i < n; ++i)
        v.push_back(query(i, i));
      dbg(v);
    }
#else
    void debug(int n) {}
#endif
  };
} // namespace seg
using seg::Seg;

#endif
