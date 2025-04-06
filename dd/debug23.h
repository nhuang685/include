#include "bits/stdc++.h"

template <class T> void dbg1(const T &t) { std::println(std::cerr, "{}", t); }

template <class F, class T> void dbg3(const F &f, const T &t) {
  std::println(std::cerr, "{}", std::invoke(f, t));
}

template <class F, class T> void dbg4(const F &f, const T &t) {
  std::println(std::cerr, "{}", t | std::views::transform(f));
}

template <class T, class... Args> void dbg1(const T &t, const Args &...args) {
  std::print(std::cerr, "{}, ", t);
  dbg1(args...);
}

template <class F, class T, class... Args>
void dbg3(const F &f, const T &t, const Args &...args) {
  std::print(std::cerr, "{}, ", std::invoke(f, t));
  dbg3(f, args...);
}

template <class F, class T, class... Args>
void dbg4(const F &f, const T &t, const Args &...args) {
  std::print(std::cerr, "{}, ", t | std::views::transform(f));
  dbg4(f, args...);
}
