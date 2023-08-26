#pragma once

#include "Socket.h"
#include "SSocket.h"
#include "Select.h"
using namespace bb::select;
#include "Autolock.h"
#include "ObjectPool.h"
#include "Logger.h"
using namespace bb::util;

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
    ObjectPool<Socket> m_sockpool;
    Mutex m_mutex;
};

}
}