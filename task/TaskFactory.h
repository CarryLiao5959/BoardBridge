#pragma once

#include "Socket.h"
#include "Task.h"
#include "WorkTask.h"
using namespace bb::socket;
using namespace bb::thread;

namespace bb {
namespace task {
class TaskFactory {
  public:
    static Task *create(Socket *socket) {
        return new WorkTask(socket);
    }
};
} // namespace task
} // namespace yazi