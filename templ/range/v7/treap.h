#ifndef TEMPL_TREAP_H
#define TEMPL_TREAP_H

namespace treap {
  using u32 = unsigned int;
  inline std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

  template <class Node>
  concept TreapNode = requires(Node n, const Node& cn, Pointer<Node> p, const typename Node::Output& val) {
    typename Node::Output;
    std::default_initializable<Node>;
    { n.prior } -> std::same_as<u32&>;
    { cn.prior } -> std::same_as<const u32&>;
    { n.ch } -> std::same_as<std::array<Pointer<Node>, 2>&>;
    { cn.ch } -> std::same_as<const std::array<Pointer<Node>, 2>&>;
    { n.sub } -> std::same_as<int&>;
    { cn.sub } -> std::same_as<const int&>;
    { Node::ID } -> std::convertible_to<typename Node::Output>;
    { Node::comb(val, val) } -> std::same_as<typename Node::Output>;
    { cn.value() } -> std::same_as<typename Node::Output>;
    { cn.needs_push() } -> std::same_as<bool>;
    { n.push() } -> std::same_as<void>;
    { n.pull() } -> std::same_as<void>;
  };

  template <TreapNode Node> struct Treap {
    using P = Pointer<Node>;
    using Output = typename Node::Output;

    P rt;
    Treap() = default;
    explicit Treap(P n) : rt{n} {}
    template <std::ranges::sized_range R>
      requires std::constructible_from<Node, std::ranges::range_value_t<R>>
    explicit Treap(const R& a) {
      int i = 0;
      for (const auto& v : a)
        insert(i++, v);
    }

    static void push(P n) {
      if (!n || !n->needs_push())
        return;
      n->push();
    }
    static void pull(P n) {
      if (!n)
        return;
      n->sub = (n->ch[0] ? n->ch[0]->sub : 0) + (n->ch[1] ? n->ch[1]->sub : 0) + 1;
      n->pull();
    }

    static std::pair<P, P> split(P n, int left) {
      if (!n)
        return {{}, {}};
      push(n);
      int nl = (n->ch[0] ? n->ch[0]->sub : 0) + 1;
      if (nl <= left) {
        auto [mid, r] = split(n->ch[1], left - nl);
        n->ch[1] = mid;
        pull(n);
        return {n, r};
      }
      auto [l, mid] = split(n->ch[0], left);
      n->ch[0] = mid;
      pull(n);
      return {l, n};
    }
    std::pair<Treap, Treap> split(int left) {
      auto [l, r] = split(rt, left);
      return {Treap{l}, Treap{r}};
    }

    static P merge(P l, P r) {
      if (!l)
        return r;
      if (!r)
        return l;
      push(l);
      push(r);
      if (l->prior > r->prior) {
        // if ((int)(rng() % (l->sub + r->sub)) < l->sub) {
        // root at l
        P nr = merge(l->ch[1], r);
        l->ch[1] = nr;
        pull(l);
        return l;
      }
      P nl = merge(l, r->ch[0]);
      r->ch[0] = nl;
      pull(r);
      return r;
    }
    static Treap merge(Treap l, Treap r) { return Treap{merge(l.rt, r.rt)}; }

    template <class... Args> P create(Args... args) {
      P n = P::make(std::forward<Args>(args)...);
      n->prior = rng();
      return n;
    }
    void insert(int ind, P n) {
      auto [l, r] = split(rt, ind);
      rt = merge(merge(l, n), r);
    }
    template <class... Args>
      requires std::constructible_from<Node, Args...>
    void insert(int ind, Args... args) {
      insert(ind, create(std::forward<Args>(args)...));
    }

    void erase(int ind) {
      auto [l, mr] = split(rt, ind);
      auto [m, r] = split(mr, 1);
      rt = merge(l, r);
    }

    template <class F, class... Args> void upd(F&& f, int l, int r, Args... args) {
      auto [ll, mrr] = split(rt, l);
      auto [m, rr] = split(mrr, r - l + 1);
      std::invoke(f, *m, std::forward<Args>(args)...);
      rt = merge(merge(ll, m), rr);
    }

    Output query(int l, int r) {
      if (l > r)
        return Node::ID;
      Output res = Node::ID;
      upd([&res](const Node& n) { res = n.value(); }, l, r);
      return res;
    }
  };
} // namespace treap
using treap::Treap;

#endif
