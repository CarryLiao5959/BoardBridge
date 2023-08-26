#include "TaskDispatcher.h"

#include "Singleton.h"
#include "Logger.h"
using namespace bb::util;
#include "ThreadPool.h"
using namespace bb::thread;

#include <unistd.h>

TaskDispatcher::TaskDispatcher() {}
TaskDispatcher::~TaskDispatcher() {}

void TaskDispatcher::init_threadpool(int threadpool_num) {
    log_debug("init TaskDispatcher");
    Singleton<ThreadPool>::instance()->create(threadpool_num);
    start_thread();
}

void TaskDispatcher::run() {
    // disable signal interrupts
    sigset_t mask;
    if (0 != sigfillset(&mask)) {
        return;
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL)) {
        return;
    }

    while (true) {
        // try to acquire task
        m_mutex.lock();
        while (m_tasks.empty()) {
            log_debug("TaskDispatcher m_tasks is empty");
            m_cond.wait(&m_mutex);
        }

        // got task
        Task *task = m_tasks.front();
        m_tasks.pop_front();
        m_mutex.unlock();

        // assign task
        ThreadPool *threadpool = Singleton<ThreadPool>::instance();
        if (threadpool->get_idle_thread_numbers() > 0) {
            log_debug("let threadpool to assign task");
            threadpool->assign(task);
        } else {
            log_debug("there is no idle_thread in threadpool to assign task");
            m_mutex.lock();
            m_tasks.push_front(task);
            m_mutex.unlock();
        }
    }
}

void TaskDispatcher::assign(Task *task) {
    m_mutex.lock();
    m_tasks.push_back(task);
    m_mutex.unlock();
    m_cond.signal();
}