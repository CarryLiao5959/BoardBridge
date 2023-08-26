#include "WorkThread.h"

#include "Singleton.h"
#include "Task.h"
#include "ThreadPool.h"
using namespace bb::thread;
#include "Socket.h"
using namespace bb::socket;

void WorkerThread::run() {
    // 让工作线程屏蔽掉所有的信号
    sigset_t mask;
    if (0 != sigfillset(&mask)) {
        log_error("workerthread sigfillset fail!");
    }
    if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL)) {
        log_error("workerthread pthread_sigmask fail!");
    }

    // 当线程被取消或正常退出时调用 cleanup 
    pthread_cleanup_push(cleanup, this);

    while (true) {
        // 尝试获取工作线程的任务
        m_mutex.lock();
        while (m_task == NULL){
            m_cond.wait(&m_mutex);
        }
        int sockfd = ((Socket *)(m_task->get_data()))->get_fd();
        log_debug("thread %d find task from socket %d cmd %d", m_tid, sockfd, m_task->get_cmd());
        m_mutex.unlock();

        // 确保线程在执行任务时不会被取消
        int rc = 0;
        int old_state = 0;
        rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_state);

        // 执行任务
        m_task->run();

        // 销毁任务
        m_task->destroy();
        m_task = NULL;

        // 把此工作线程放回线程池的空闲队列中
        Singleton<ThreadPool>::instance()->move_to_idle_list(this);

        // 线程状态设为可取消
        rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_state);

        // cancel-point 检查是否有取消请求
        pthread_testcancel();
    }
    
    // 从清理句柄堆栈中移除最近的清理函数
    pthread_cleanup_pop(1);
}