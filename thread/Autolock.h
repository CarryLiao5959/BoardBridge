#pragma once
#include "Mutex.h"


namespace bb{
namespace thread{

class Autolock {
public:
  Autolock(Mutex *mutex) {
    m_mutex=mutex;
    m_mutex->lock();
  }
  ~Autolock() {
    m_mutex->unlock();
  }

private:
  Mutex *m_mutex;
};
}

}