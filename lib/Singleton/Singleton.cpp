#include "Singleton.hpp"
#include <queue>

namespace {

constexpr int kMaxFinalizersSize = 256;
std::mutex gMutex;
int gNumFinalizersSize = 0;
// SingletonFinalizer::FinalizerFunc gFinalizers[kMaxFinalizersSize];
std::queue<SingletonFinalizer::FinalizerFunc> finalizers;
}

void SingletonFinalizer::addFinalizer(FinalizerFunc func) {
    std::lock_guard<std::mutex> lock(gMutex);
    assert(gNumFinalizersSize < kMaxFinalizersSize);
    // gFinalizers[gNumFinalizersSize++] = func;
    finalizers.emplace(func);
}

void SingletonFinalizer::finalize() {
    std::lock_guard<std::mutex> lock(gMutex);
    // for (int i = gNumFinalizersSize - 1; i >= 0; --i) {
    //     (*gFinalizers[i])();
    // }

    while(finalizers.size()) {
        auto& f = finalizers.front();
        finalizers.pop();
    }
    gNumFinalizersSize = 0;
}