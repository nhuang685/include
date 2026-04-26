#ifndef TEMPL_P_POINTER_H
#define TEMPL_P_POINTER_H

#include <bits/stdc++.h>

template <class T> struct Pointer {
  std::shared_ptr<T> val;
  template <class... Args> static Pointer make(Args&&... args) {
    return Pointer{std::make_shared<T>(std::forward<Args>(args)...)};
  }
  T* operator->() { return val.get(); }
  const T* operator->() const { return val.get(); }
  T& operator*() { return *val; }
  const T& operator*() const { return *val; }
  T* get() { return val.get(); }
  const T* get() const { return val.get(); }
  explicit operator bool() const { return (bool)val; }
  bool operator==(const Pointer& rhs) const = default;

  Pointer clone() const { return val ? make(*val) : make(T{}); }
};

#endif
