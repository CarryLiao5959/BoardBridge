#include "SSocket.h"
#include "SocketHandler.h"

int main()
{
    const string serv_ip="127.0.0.1";
    const int serv_port=5005;

    SSocket* socket=new SSocket(serv_ip,serv_port);
    int serv_sock = socket->sock_init();
    socket->set_reuse_addr();
    socket->set_keep_alive();
    socket->sock_bind();
    socket->sock_listen();

    SocketHandler *handler=new SocketHandler(socket);
    handler->handle();

    close(serv_sock);
    return 0;
}