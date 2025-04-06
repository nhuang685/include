/**
 * @file dbg.cpp
 * @author n685
 * @brief
 * @date 2023-08-18
 *
 *
 */
#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace __gnu_pbds;

template <class T>
using OST = tree<T, null_type, std::less<T>, rb_tree_tag,
                 tree_order_statistics_node_update>;

#ifdef LOCAL
std::ifstream cin;
std::ofstream cout;
using std::cerr;
#else
using std::cin;
using std::cout;
#define cerr                                                                   \
  if (false)                                                                   \
  std::cerr
#endif

#ifdef LOCAL
#include "dd/debug.h"
#define dbg(...) lineInfo(__LINE__, #__VA_ARGS__), dbg1(__VA_ARGS__)
#define dbgR(...) lineInfo(__LINE__, #__VA_ARGS__), dbg2(__VA_ARGS__)
void nline() { cerr << '\n'; }
#else
#define dbg(...) 42
#define dbgR(...) 420
void nline() {}
#endif

auto main() -> int {
#ifdef LOCAL
  cin.open("input.txt");
  cout.rdbuf()->pubsetbuf(0, 0);
  cout.open("output.txt");
#else
  cin.tie(nullptr)->sync_with_stdio(false);
#endif

  OST<int> ost;
  ost.insert(3);
  ost.insert(2);
  ost.insert(1);

  dbgR(ost);
}
