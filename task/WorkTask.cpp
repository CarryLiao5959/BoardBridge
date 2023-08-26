#include "WorkTask.h"

#include "Logger.h"
#include "Singleton.h"
using namespace bb::util;
#include "Info.h"
using namespace bb::info;
using namespace bb::task;
using namespace bb::socket;

using namespace std;

void WorkTask::run() {
    log_debug("WorkTask running");

    Socket *socket = (Socket *)m_data;

    // char m_buf[1024];
    // strcpy(m_buf, "hello");
    // int ret = send(socket->get_fd(), m_buf, 10, 0);
    // if (ret < 0) {
    //     log_error("send failed: errno=%d errstr=%s", errno, strerror(errno));
    //     return;
    // }
    // log_debug("send success");

    Info* info = new Info(socket->get_fd());
    log_info("cmd: %s",Info::cmd_typename[m_cmd]);
    info->info(m_cmd);
}