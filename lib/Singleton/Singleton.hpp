#pragma once

#include <cassert>
#include <mutex>
#include <memory>

class SingletonFinalizer {
public:
    using FinalizerFunc = void(*)();
    static void addFinalizer(FinalizerFunc func);
    static void finalize();
};

template <typename T>
class Singleton final {
public:
    static T& get_instance() {
        std::call_once(initFlag, create);
        assert(instance);
        return *instance;
    }
    virtual ~Singleton() = 0;
private:
    static void create() {
        instance = std::make_unique<T>();
        SingletonFinalizer::addFinalizer(&Singleton<T>::destroy);
    }

    static void destroy() {
        delete instance.release();
        instance = nullptr;
    }

    static std::once_flag initFlag;
    static std::unique_ptr<T> instance;
};

template <typename T> std::once_flag Singleton<T>::initFlag;
template <typename T> std::unique_ptr<T> Singleton<T>::instance = nullptr;