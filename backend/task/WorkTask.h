#pragma once

#include "Socket.h"
#include "Task.h"
#include "SocketHandler.h"
using namespace yazi::socket;
using namespace yazi::thread;

namespace yazi {
namespace task {

struct MsgHead {
    char flag[8];
    uint32_t cmd;
    uint32_t len;
};

const uint32_t buf_size[6] = {
    0,
    1024, 
    1024 * 2, 
    1024 * 4, 
    1024 * 4, 
    1024 * 8,
};

class WorkTask : public Task {
  public:
    WorkTask(Socket *socket) : Task(socket) {
        debug("new WorkTask sockfd: %d", socket->getfd());
    }

    virtual ~WorkTask() {}

    virtual void run() override;
    virtual void destroy() override {
        debug("work job destory");
        delete this;
    }
    void echo(int msg_head_len);
    void text();
    void img();
    void gif();
    void mp4();
};

} // namespace task
} // namespace yazi