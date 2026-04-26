#ifndef TEMPL_P_POINTER_H
#define TEMPL_P_POINTER_H

#include <bits/stdc++.h>

template <class T> struct Pointer {
  static inline std::vector<T> vals;
  static void alloc(int len) { vals.reserve(len); }
  int ind = -1;
  template <class... Args> static Pointer make(Args&&... args) {
    vals.emplace_back(std::forward<Args>(args)...);
    return Pointer{(int)vals.size() - 1};
  }
  T* operator->() { return &vals[ind]; }
  const T* operator->() const { return &vals[ind]; }
  T& operator*() { return vals[ind]; }
  const T& operator*() const { return vals[ind]; }
  T* get() { return &vals[ind]; }
  const T* get() const { return &vals[ind]; }
  explicit operator bool() const { return ind != -1; }
  bool operator==(const Pointer& rhs) const = default;

  Pointer clone() const { return ind != -1 ? make(vals[ind]) : make(T{}); }
};

#endif
