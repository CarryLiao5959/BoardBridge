#pragma once

#include "Thread.h"
#include "Logger.h"

#include <pthread.h>
#include <signal.h>

using namespace bb::util;

namespace bb {
namespace thread {

class WorkerThread : public Thread {
  public:
    WorkerThread() : Thread() {}
    WorkerThread(int tid) : Thread(tid) {}
    virtual ~WorkerThread() {}

    virtual void run() override;

    static void cleanup(void *ptr) {
        log_info("workerthread cleanup handler: %x", ptr);
    }
};

} // namespace thread
} // namespace bb