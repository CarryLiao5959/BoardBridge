#pragma once
#include <pthread.h>

namespace bb {
namespace thread {
class Mutex {
    friend class Condition;

  public:
    Mutex() {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
        pthread_mutex_init(&m_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }
    ~Mutex() { pthread_mutex_destroy(&m_mutex); }

    int lock() { return pthread_mutex_lock(&m_mutex); }
    int trylock() { return pthread_mutex_trylock(&m_mutex); }
    int unlock() { return pthread_mutex_unlock(&m_mutex); }

  private:
    pthread_mutex_t m_mutex;
};
} 
} 