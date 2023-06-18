#include "SocketHandler.h"
#include "Select.h"
#include "Info.h"

SocketHandler::SocketHandler()
{
}

SocketHandler::SocketHandler(SSocket* server)
{
    m_server = server;
}

SocketHandler::~SocketHandler()
{
    if (m_server != NULL)
    {
        delete m_server;
        m_server = NULL;
    }
}

void SocketHandler::handle() {
    int serv_sock = m_server->get_fd();
    Select* select = new Select(serv_sock);
    select->add_fd(serv_sock);
    while (1)
    {
        int infds = select->select();
        for (int efd = 0; efd <= select->get_max_fd(); efd++)
        {
            if (!select->isset(efd, true))
            {
                continue;
            }
            // client connect
            if (efd == serv_sock)
            {
                printf("efd from serv_sock [%d]\n", serv_sock);
                int conn_sock = m_server->sock_accept();
                if (conn_sock > 0)
                {
                    select->add_fd(conn_sock);
                }
            }
            // client send/recv
            else
            {
                printf("efd from cli_sock [%d]\n", efd);
                //recv
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(efd, buf, 1024, 0);
                if (len == -1)
                {
                    printf("recv fail\n");
                    select->rm_fd(efd);
                }
                else if (len == 0)
                {
                    printf("recv timeout\n");
                    select->rm_fd(efd);
                }
                else
                {
                    buf[len] = '\0';
                    printf("recv data: %s\n", buf);
                    Info* info = new Info(efd);
                    printf("atoi(buf): %d\n",atoi(buf));
                    info->info(atoi(buf));
                }
            }
        }
    }
}

