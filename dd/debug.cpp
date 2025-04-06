/**
 *
 * @file debug.cpp
 * @author n685
 * @brief
 * @date 2023-08-18
 *
 *
 */
#include <bits/stdc++.h>

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

template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::pair<T, U> &p);
template <typename... Types>
std::ostream &operator<<(std::ostream &out, const std::tuple<Types...> &value);

template <class T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v);
template <class T, std::size_t sz>
std::ostream &operator<<(std::ostream &out, const std::array<T, sz> &v);

template <class T>
std::ostream &operator<<(std::ostream &out, const std::set<T> &v);
template <class T>
std::ostream &operator<<(std::ostream &out, const std::unordered_set<T> &v);
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::map<T, U> &v);
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::unordered_map<T, U> &v);
template <class T>
std::ostream &operator<<(std::ostream &out, const std::multiset<T> &v);
template <class T>
std::ostream &operator<<(std::ostream &out,
                         const std::unordered_multiset<T> &v);
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::multimap<T, U> &v);
template <class T, class U>
std::ostream &operator<<(std::ostream &out,
                         const std::unordered_multimap<T, U> &v);

template <class T>
std::ostream &printRange(std::ostream &out, const T &begin, const T &end) {
  out << "{";
  for (auto it = begin; it != end; ++it) {
    out << *it;
    if (std::next(it) != end)
      out << ", ";
  }
  out << "}";
  return out;
}

template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::pair<T, U> &p) {
  out << "(" << p.first << ", " << p.second << ")";
  return out;
}
// Source: https://stackoverflow.com/a/31116392/12128483
template <typename Type, unsigned N, unsigned Last> struct TuplePrinter {
  static void print(std::ostream &out, const Type &value) {
    out << std::get<N>(value) << ", ";
    TuplePrinter<Type, N + 1, Last>::print(out, value);
  }
};

template <typename Type, unsigned N> struct TuplePrinter<Type, N, N> {
  static void print(std::ostream &out, const Type &value) {
    out << std::get<N>(value);
  }
};

template <typename... Types>
std::ostream &operator<<(std::ostream &out, const std::tuple<Types...> &value) {
  out << '(';
  TuplePrinter<std::tuple<Types...>, 0, sizeof...(Types) - 1>::print(out,
                                                                     value);
  return out << ')';
}

template <class T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T, std::size_t sz>
std::ostream &operator<<(std::ostream &out, const std::array<T, sz> &v) {
  return printRange(out, v.begin(), v.end());
}

template <class T>
std::ostream &operator<<(std::ostream &out, const std::set<T> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T>
std::ostream &operator<<(std::ostream &out, const std::unordered_set<T> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::map<T, U> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::unordered_map<T, U> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T>
std::ostream &operator<<(std::ostream &out, const std::multiset<T> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T>
std::ostream &operator<<(std::ostream &out,
                         const std::unordered_multiset<T> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T, class U>
std::ostream &operator<<(std::ostream &out, const std::multimap<T, U> &v) {
  return printRange(out, v.begin(), v.end());
}
template <class T, class U>
std::ostream &operator<<(std::ostream &out,
                         const std::unordered_multimap<T, U> &v) {
  return printRange(out, v.begin(), v.end());
}

void lineInfo(int num, const std::string &v) {
  cerr << "Line " << num << ": [" << v << "] = ";
}

template <class T> void dbg1(T t) { cerr << t << '\n'; }
template <class T> void dbg2(const T &t) {
  printRange(cerr, t.begin(), t.end());
  cerr << '\n';
}
template <class F, class T> void dbg3(const F &f, const T &t) {
  cerr << f(t) << '\n';
}

template <class T, class... Args> void dbg1(const T &t, const Args &...args) {
  cerr << t << ", ";
  dbg1(args...);
}
template <class T, class... Args> void dbg2(const T &t, const Args &...args) {
  printRange(cerr, t.begin(), t.end());
  cerr << ", ";
  dbg2(args...);
}
template <class F, class T, class... Args>
void dbg3(const F &f, const T &t, const Args &...args) {
  cerr << f(t) << ", ";
  dbg3(f, args...);
}

int main() {
#ifdef LOCAL
  cin.open("input.txt");
  cout.rdbuf()->pubsetbuf(0, 0);
  cout.open("output.txt");
#else
  cin.tie(nullptr)->sync_with_stdio(false);
#endif

  // cout << std::vector{1, 2, 3} << '\n';
  // std::map<int, int> m;
  // m[0] = 1;
  // m[1] = 2;
  // m[5] = 3;
  // std::sort(v.begin(), v.end());
}
