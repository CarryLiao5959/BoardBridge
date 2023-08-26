#pragma once

#include "Task.h"
using namespace bb::task;
#include "Thread.h"

#include <list>
#include <pthread.h>
#include <signal.h>

namespace bb {
namespace thread {

class TaskDispatcher : public Thread {
  public:
    TaskDispatcher();
    ~TaskDispatcher();

    void init_threadpool(int threadpool_num);
    void assign(Task *task);

    virtual void run() override;

  protected:
    std::list<Task *> m_tasks;
};

} // namespace thread
} // namespace bb