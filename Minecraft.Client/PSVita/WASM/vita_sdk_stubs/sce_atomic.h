#pragma once

#ifdef __cplusplus
#include <atomic>

inline int sceAtomicCompareAndSwap32(volatile int *ptr, int oldVal, int newVal) {
    std::atomic<int> *ap = reinterpret_cast<std::atomic<int>*>(const_cast<int*>(ptr));
    int expected = oldVal;
    ap->compare_exchange_strong(expected, newVal);
    return expected;
}

inline long long sceAtomicCompareAndSwap64(volatile long long *ptr, long long oldVal, long long newVal) {
    std::atomic<long long> *ap = reinterpret_cast<std::atomic<long long>*>(const_cast<long long*>(ptr));
    long long expected = oldVal;
    ap->compare_exchange_strong(expected, newVal);
    return expected;
}

#else
// C fallback: safe for single-threaded WASM execution
static inline int sceAtomicCompareAndSwap32(volatile int *ptr, int oldVal, int newVal) {
    int old = *ptr;
    if (old == oldVal) {
        *ptr = newVal;
    }
    return old;
}

static inline long long sceAtomicCompareAndSwap64(volatile long long *ptr, long long oldVal, long long newVal) {
    long long old = *ptr;
    if (old == oldVal) {
        *ptr = newVal;
    }
    return old;
}
#endif
