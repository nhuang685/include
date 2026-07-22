#ifndef TEMPL_SPLAY_H
#define TEMPL_SPLAY_H

namespace splay {
  template <class Node>
  concept SplayNode = requires(Node n, const Node& cn, Pointer<Node> p, const typename Node::Output& val) {
    typename Node::Output;
    std::default_initializable<Node>;
    { n.par } -> std::same_as<Pointer<Node>&>;
    { cn.par } -> std::same_as<const Pointer<Node>&>;
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

  template <SplayNode Node> struct Splay {
    using P = Pointer<Node>;
    using Output = typename Node::Output;

    P rt;
    Splay() = default;
    explicit Splay(P n) : rt{n} {}
    template <std::ranges::sized_range R>
      requires std::constructible_from<Node, std::ranges::range_value_t<R>>
    explicit Splay(const R& a) {
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

    static int dir(P n) { return n->par->ch[1] == n; }
    static void rotate(P n) {
      push(n);
      int d = dir(n);
      P p = n->par, mid = n->ch[d ^ 1];
      p->ch[d] = mid;
      if (mid)
        mid->par = p;
      if (p->par)
        p->par->ch[dir(p)] = n;
      n->par = p->par;
      n->ch[d ^ 1] = p;
      p->par = n;
      pull(p);
      pull(n);
    }

    static void splay(P n, P stop) {
      if (!n)
        return;
      while (n->par != stop) {
        P p = n->par, gp = p->par;
        if (gp == stop)
          rotate(n);
        else if (dir(n) == dir(p)) {
          rotate(p); // zig-zig
          rotate(n);
        } else {
          rotate(n); // zig-zag
          rotate(n);
        }
      }
    }
    void splay(P n) {
      splay(n, P{});
      rt = n;
    }

    static P find(P n, int left) {
      if (!n || left <= 0 || left > n->sub)
        return P{};
      while (true) {
        int nl = n->ch[0] ? n->ch[0]->sub : 0;
        if (left == nl + 1)
          return n;
        push(n);
        if (left <= nl)
          n = n->ch[0];
        else {
          n = n->ch[1];
          left -= nl + 1;
        }
      }
      assert(false);
    }

    static std::pair<P, P> split(P n, int left) {
      if (!n)
        return {{}, {}};
      if (left <= 0)
        return {{}, n};
      if (left >= n->sub)
        return {n, {}};
      P l = find(n, left);
      splay(l, P{});
      push(l);
      P r = l->ch[1];
      l->ch[1] = P{};
      if (r)
        r->par = P{};
      pull(l);
      return {l, r};
    }
    std::pair<Splay, Splay> split(int left) {
      auto [l, r] = split(rt, left);
      return {Splay{l}, Splay{r}};
    }

    static P merge(P l, P r) {
      if (!l)
        return r;
      if (!r)
        return l;
      P mid = find(l, l->sub);
      splay(mid, P{});
      push(mid);
      mid->ch[1] = r;
      r->par = mid;
      pull(mid);
      return mid;
    }
    static Splay merge(Splay l, Splay r) { return Splay{merge(l.rt, r.rt)}; }

    // void insert(int ind, P n) {
    //   if (!rt) {
    //     rt = n;
    //     return;
    //   }
    //   P cur;
    //   if (ind > 0) {
    //     cur = find(rt, ind);
    //     push(cur);
    //     if (!cur->ch[1]) {
    //       cur->ch[1] = n;
    //       n->par = cur;
    //       splay(n);
    //       return;
    //     }
    //     cur = cur->ch[1];
    //   } else
    //     cur = rt;
    //   cur = find(cur, 1);
    //   push(cur);
    //   cur->ch[0] = n;
    //   n->par = cur;
    //   splay(n);
    // }
    template <class... Args>
      requires std::constructible_from<Node, Args...>
    void insert(int ind, Args... args) {
      insert(ind, P::make(std::forward<Args>(args)...));
    }

    // void erase(int ind) {
    //   P n = find(rt, ind + 1);
    //   splay(n);
    //   push(n);
    //   if (n->ch[0])
    //     n->ch[0]->par = P{};
    //   if (n->ch[1])
    //     n->ch[1]->par = P{};
    //   rt = merge(n->ch[0], n->ch[1]);
    // }

    void insert(int ind, P n) {
      auto [l, r] = split(rt, ind);
      rt = merge(merge(l, n), r);
    }

    void erase(int ind) {
      auto [l, mr] = split(rt, ind);
      auto [m, r] = split(mr, 1);
      rt = merge(l, r);
    }

    // template <class F, class... Args> void upd(F&& f, int l, int r, Args... args) {
    //   auto [ll, mrr] = split(rt, l);
    //   auto [m, rr] = split(mrr, r - l + 1);
    //   std::invoke(f, *m, std::forward<Args>(args)...);
    //   rt = merge(merge(ll, m), rr);
    // }
    template <class F, class... Args> void upd(F&& f, int l, int r, Args... args) {
      if (l == 0) {
        if (r == rt->sub - 1)
          std::invoke(f, *rt, std::forward<Args>(args)...);
        else {
          P nr = find(rt, r + 2);
          splay(nr);
          push(nr);
          std::invoke(f, *nr->ch[0], std::forward<Args>(args)...);
          pull(nr);
        }
      } else if (r == rt->sub - 1) {
        P nl = find(rt, l);
        splay(nl);
        push(nl);
        std::invoke(f, *nl->ch[1], std::forward<Args>(args)...);
        pull(nl);
      } else {
        P nr = find(rt, r + 2);
        splay(nr);
        P nl = find(rt, l);
        splay(nl, nr);
        push(nl);
        std::invoke(f, *nl->ch[1], std::forward<Args>(args)...);
        pull(nl);
        pull(nr);
      }
    }

    Output query(int l, int r) {
      if (l > r)
        return Node::ID;
      Output res = Node::ID;
      upd([&res](const Node& n) { res = n.value(); }, l, r);
      return res;
    }
  };
} // namespace splay
using splay::Splay;

#endif
