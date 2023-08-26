#pragma once

#include "Mutex.h"
#include <pthread.h>

namespace bb {
namespace thread {
class Condition {
  public:
    Condition() { pthread_cond_init(&m_cond, NULL); }
    ~Condition() { pthread_cond_destroy(&m_cond); }

    int wait(Mutex *mutex) {
        return pthread_cond_wait(&m_cond, &mutex->m_mutex);
    }
    int signal() { return pthread_cond_signal(&m_cond); }
    int broadcast() { return pthread_cond_broadcast(&m_cond); }

  protected:
    pthread_cond_t m_cond;
};
} 
} 