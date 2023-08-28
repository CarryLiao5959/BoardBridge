#pragma once

#include "Socket.h"
#include "SSocket.h"
#include "Select.h"
using namespace bb::select;
#include "Autolock.h"
#include "ObjectPool.h"
#include "Logger.h"
using namespace bb::util;

#include <map>
#include <string>
#include <vector>
using namespace std;

namespace bb {
namespace socket {

class SocketHandler
{
public:
    SocketHandler();
    SocketHandler(SSocket * server);
    ~SocketHandler();

    void listen(const string &ip, int port);
    void handle(int max_connects, int wait_time);

    void attach_sock(Socket *socket);
    void detach_sock(Socket *socket);
    void remove_to_pool(Socket *socket);

  private:
    SSocket * m_server;
    Select *m_select;
    map<int, Socket *> active_sock;
    ObjectPool<Socket> m_sockpool;
    Mutex m_mutex;
};

}
}