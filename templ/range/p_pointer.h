#ifndef TEMPL_P_POINTER_H
#define TEMPL_P_POINTER_H

#include <bits/stdc++.h>

template <class T> struct Pointer {
  T* val = nullptr;
  template <class... Args> static Pointer make(Args&&... args) { return Pointer{new T{std::forward<Args>(args)...}}; }
  T* operator->() { return val; }
  const T* operator->() const { return val; }
  T& operator*() { return *val; }
  const T& operator*() const { return *val; }
  T* get() { return val; }
  const T* get() const { return val; }
  explicit operator bool() const { return val != nullptr; }
  bool operator==(const Pointer& rhs) const = default;

  Pointer clone() const { return val ? make(*val) : make(T{}); }
};

#endif
