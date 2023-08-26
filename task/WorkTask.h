#pragma once

#include "Socket.h"
#include "SocketHandler.h"
using namespace bb::socket;
#include "Task.h"
using namespace bb::thread;
#include "Logger.h"
using namespace bb::util;

namespace bb {
namespace task {

struct MsgHead {
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};

class WorkTask : public Task {
  public:
    WorkTask(Socket *socket) : Task(socket) {
        log_debug("new WorkTask sockfd: %d", socket->get_fd());
    }

    virtual ~WorkTask() {}

    virtual void run() override;
    virtual void destroy() override {
        log_debug("WorkTask destory");
        delete this;
    }
};

} // namespace task
} // namespace bb
