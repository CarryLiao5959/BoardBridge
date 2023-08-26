#pragma once

#include "Logger.h"
#include "Server.h"
#include "Singleton.h"
#include "System.h"
#include <dirent.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace bb::util;
using namespace bb::server;
using namespace std;
namespace bb {
namespace util {
class System {
  private:
    string m_root_path;

  public:
    System() {}
    ~System() {}
    void init() {
        get_root_path();
        Logger::instance()->open_ofstream(m_root_path + "/log/server.log",ios::out|ios::trunc);

        log_debug("open log/server.log");

        // IniFile *ini = Singleton<IniFile>::instance();
        // ini->load(m_root_path + "/config/main.ini");
        // debug("load config/main.ini");

        // const string ip = (*ini)["server"]["ip"];
        // int port = (*ini)["server"]["port"];
        // int threads = (*ini)["server"]["threads"];
        // int max_conn = (*ini)["server"]["max_conn"];
        // int wait_time = (*ini)["server"]["wait_time"];

        Server *server = Singleton<Server>::instance();
        // server->set_threads(threads);
        // server->set_connects(max_conn);
        // server->set_wait_time(wait_time);
        // server->listen(ip, port);
        server->start();
    }

    void get_root_path() {
        if (m_root_path != "") {
            return;
        }
        char path[1024];
        memset(path, 0, 1024);
        int cnt = readlink("/proc/self/exe", path, 1024);
        if (cnt < 0 || cnt >= 1024) {
            return;
        }
        for (int i = cnt; i >= 0; --i) {
            if (path[i] == '/') {
                path[i] = '\0';
                break;
            }
        }
        m_root_path = path;
    }
};
} // namespace util
} // namespace yazi
