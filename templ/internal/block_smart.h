#ifndef TEMPL_P_POINTER_H
#define TEMPL_P_POINTER_H

#include <bits/stdc++.h>

template <class T> struct Block {
    std::shared_ptr<T> val;
    template <class... Args> static Block make(Args&&... args) {
        return Block{std::make_shared<T>(std::forward<Args>(args)...)};
    }
    static Block clone_create(Block b) {
        if (!b)
            return make();
        T content = *b;
        return make(std::move(content));
    }
    T* operator->() { return val.get(); }
    const T* operator->() const { return val.get(); }
    T& operator*() { return *val; }
    const T& operator*() const { return *val; }
    T* get() { return val.get(); }
    const T* get() const { return val.get(); }
    explicit operator bool() const { return (bool)val; }
    bool operator==(const Block& rhs) const = default;

    static void destroy(Block) {}
};

#endif
