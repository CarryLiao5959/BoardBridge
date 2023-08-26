#pragma once

#include "Mutex.h"
#include "Condition.h"
#include "Task.h"
using namespace bb::task;

#include <pthread.h>

namespace bb {
namespace thread {

class Thread {
    friend class ThreadPool;

  public:
    Thread();
    Thread(int tid);
    ~Thread();

    virtual void run() = 0;

    void start_thread();
    void stop_thread();

    void set_task(Task *task);
    Task *get_task();

  protected:
    /*
    @brief 线程回调函数
    @note  使用静态成员函数或全局函数作为线程入口，并显式传递 this 指针或其他上下文信息给它。
    */
    static void *thread_func(void *ptr);

  protected:
    pthread_t m_tid;
    Task *m_task;
    Mutex m_mutex;
    Condition m_cond;
};

}
} 