#ifndef TEMPL_P_POINTER_H
#define TEMPL_P_POINTER_H

#include <bits/stdc++.h>

template <class T> struct Block {
    static inline std::vector<T> vals;
    static inline std::vector<int> free;
    static void alloc(int len) { vals.reserve(len); }
    int ind = -1;
    template <class... Args> static Block make(Args&&... args) {
        if (!free.empty()) {
            int i = free.back();
            vals[i] = T{std::forward<Args>(args)...};
            free.pop_back();
            return Block{i};
        }
        vals.emplace_back(std::forward<Args>(args)...);
        return Block{(int)vals.size() - 1};
    }
    T* operator->() { return &vals[ind]; }
    const T* operator->() const { return &vals[ind]; }
    T& operator*() { return vals[ind]; }
    const T& operator*() const { return vals[ind]; }
    T* get() { return &vals[ind]; }
    const T* get() const { return &vals[ind]; }
    explicit operator bool() const { return ind != -1; }
    bool operator==(const Block& rhs) const = default;
    void destroy() {
        if (ind == -1)
            return;
        free.push_back(ind);
    }
};

#endif
