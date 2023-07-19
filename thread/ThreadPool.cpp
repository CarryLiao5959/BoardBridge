#include "ThreadPool.h"
#include "WorkerThread.h"
#include "Logger.h"
using namespace yazi::util;
using namespace yazi::thread;

ThreadPool::ThreadPool() : m_threads(0)
{
}

ThreadPool::~ThreadPool()
{
}

Thread *ThreadPool::get_idle_thread()
{
    AutoLock lock(&m_mutex_idle);
    while (m_list_idle.size() == 0)
        m_cond_idle.wait(&m_mutex_idle);
    return *m_list_idle.begin();
}

void ThreadPool::move_to_idle_list(Thread *thread)
{
    m_mutex_idle.lock();
    m_list_idle.insert(thread);
    m_cond_idle.signal();
    m_mutex_idle.unlock();

    m_mutex_busy.lock();
    std::set<Thread *>::iterator it = m_list_busy.find(thread);
    if (it != m_list_busy.end())
        m_list_busy.erase(it);
    m_cond_busy.signal();
    m_mutex_busy.unlock();
}

void ThreadPool::move_to_busy_list(Thread *thread)
{
    m_mutex_busy.lock();
    while (m_list_busy.size() == (size_t)(m_threads))
        m_cond_busy.wait(&m_mutex_busy);
    m_list_busy.insert(thread);
    m_mutex_busy.unlock();

    m_mutex_idle.lock();
    std::set<Thread *>::iterator it = m_list_idle.find(thread);
    if (it != m_list_idle.end())
        m_list_idle.erase(it);
    m_mutex_idle.unlock();
}

int ThreadPool::get_idle_thread_numbers()
{
    AutoLock lock(&m_mutex_idle);
    return m_list_idle.size();
}

int ThreadPool::get_busy_thread_numbers()
{
    AutoLock lock(&m_mutex_busy);
    return m_list_busy.size();
}

void ThreadPool::assign(Task *task)
{
    if (task == NULL)
    {
        info("task is null");
        return;
    }

    Thread *thread = get_idle_thread();
    if (thread != NULL)
    {
        move_to_busy_list(thread);
        info("task is given to thread %d", thread->m_tid);
        thread->set_task(task);
    }
    else
    {
        info("there is no idle thread to handle task");
    }
}

void ThreadPool::create(int threads)
{
    AutoLock lock(&m_mutex_idle);
    m_threads = threads;
    for (int i = 0; i < threads; i++)
    {
        Thread *thread = new WorkerThread();
        m_list_idle.insert(thread);
        thread->start();
    }
}