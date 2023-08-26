#include "Thread.h"

#include "Autolock.h"
using namespace bb::thread;
#include "Logger.h"
using namespace bb::util;

Thread::Thread() : m_tid(0), m_task(NULL) {
    log_debug("Thread created, tid is %d", m_tid);
}

Thread::Thread(int tid) : m_tid(tid), m_task(NULL) {
    log_debug("Thread created, tid is %d", m_tid);
}

Thread::~Thread() {}

void Thread::start_thread() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM); // 设置线程为系统范围，允许线程与系统中的其他线程竞争资源。
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); // 当线程终止时，它会自动释放所有资源，不需要额外的处理。
    pthread_create(&m_tid, &attr, thread_func, (void *)this);
    pthread_attr_destroy(&attr);
}

void *Thread::thread_func(void *ptr) {
    Thread *thread = (Thread *)ptr;
    thread->run();
    return ptr;
}

void Thread::stop_thread() {
    pthread_exit(PTHREAD_CANCELED);
}

void Thread::set_task(Task *task) {
    Autolock lock(&m_mutex);
    m_task = task;
    m_cond.signal();
}

Task *Thread::get_task() {
    Autolock lock(&m_mutex);
    return m_task;
}