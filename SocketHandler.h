#pragma once

#include "SSocket.h"

class SocketHandler
{
public:
    SocketHandler();
    SocketHandler(SSocket * server);
    ~SocketHandler();
    void handle();

private:
    SSocket * m_server;
};