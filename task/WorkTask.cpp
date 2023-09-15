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

    Info* info = new Info(socket->get_fd(), m_cmd, m_cmd_detail);
    log_info("cmd: %s",Info::cmd_typename[m_cmd]);
    info->info();

    Singleton<SocketHandler>::instance()->remove_to_pool(socket);
}