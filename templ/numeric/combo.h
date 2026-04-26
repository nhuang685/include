#ifndef TEMPL_COMBO_H
#define TEMPL_COMBO_H

#include "bits/stdc++.h"

namespace modular {
  template <class Mod, int LIM = 1 << 22> struct Combo {
    Combo() = delete;
    static inline bool init = false;
    static inline std::vector<Mod> fac_v, ifac_v;
    static void reset_fac() { init = false; }
    static void init_fac() {
      if (init)
        return;
      fac_v.resize(LIM + 1);
      fac_v[0] = Mod{1};
      for (int i = 1; i <= LIM; ++i)
        fac_v[i] = fac_v[i - 1] * Mod{i};
      ifac_v.resize(LIM + 1);
      ifac_v[LIM] = fac_v[LIM].inv();
      for (int i = LIM - 1; i >= 0; --i)
        ifac_v[i] = ifac_v[i + 1] * Mod{i + 1};
      init = true;
    }
    static Mod fac(int v) {
      if (!init)
        init_fac();
      return fac_v[v];
    }
    static Mod ifac(int v) {
      if (!init)
        init_fac();
      return ifac_v[v];
    }
    static Mod comb(int n, int k) {
      if (n < 0 || k < 0 || n < k)
        return Mod{0};
      return fac(n) * ifac(n - k) * ifac(k);
    }
    static Mod perm(int n, int k) {
      if (n < 0 || k < 0 || n < k)
        return Mod{0};
      return fac(n) * ifac(n - k);
    }
    static Mod inv(int v) {
      assert(v > 0);
      return fac(v - 1) * ifac(v);
    }
  };
} // namespace modular

#endif // TEMPL_COMBO_H
