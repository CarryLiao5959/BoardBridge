#pragma once

#include "Autolock.h"
#include "TaskDispatcher.h"
using namespace bb::thread;

#include <list>
using namespace std;

namespace bb {
namespace util {

template <typename T>
class ObjectPool {
  public:
    ObjectPool() {}

    ~ObjectPool() {
        Autolock lock(&m_mutex);
        for (auto it = m_pool.begin(); it != m_pool.end(); it++) {
            if ((*it) != nullptr) {
                delete (*it);
            }
        }
        m_pool.clear();
    }

    void init(int n) {
        Autolock lock(&m_mutex);
        for (int i = 0; i < n; i++) {
            T *p = new T();
            m_pool.push_back(p);
        }
    }

    T *get() {
        Autolock lock(&m_mutex);
        if (m_pool.size() == 0) {
            return nullptr;
        }
        T *p = m_pool.front();
        m_pool.pop_front();
        return p;
    }

    void put(T *p) {
        Autolock lock(&m_mutex);
        m_pool.push_back(p);
    }

  private:
    list<T *> m_pool;
    Mutex m_mutex;
};

} // namespace util
} // namespace bb