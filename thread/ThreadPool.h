#pragma once

#include "Thread.h"
#include "Mutex.h"
#include "Autolock.h"
#include "Condition.h"
#include "Task.h"
#include "WorkThread.h"

#include <set>

namespace bb {
namespace thread {

class ThreadPool {
  public:
    ThreadPool() : m_thread_cnt(0) {}
    ~ThreadPool() {}

    void create(int thread_cnt) {
        Autolock lock(&m_mutex_idle);
        m_thread_cnt = thread_cnt;
        for (int i = 1; i <= thread_cnt; i++) {
            Thread *thread = new WorkerThread(i);
            log_debug("WorkerThread created %d", i);
            m_list_idle.insert(thread);
            thread->start_thread();
        }
    }

    Thread *get_idle_thread();

    void move_to_idle_list(Thread *thread);
    void move_to_busy_list(Thread *thread);

    int get_idle_thread_numbers();
    int get_busy_thread_numbers();

    void assign(Task *task);

  private:
    int m_thread_cnt;

    std::set<Thread *> m_list_idle;
    std::set<Thread *> m_list_busy;

    Mutex m_mutex_idle;
    Mutex m_mutex_busy;

    Condition m_cond_idle;
    Condition m_cond_busy;
};

} // namespace thread
} // namespace bb
