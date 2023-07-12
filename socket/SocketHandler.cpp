#include "SocketHandler.h"

using namespace yazi::socket;

SocketHandler::SocketHandler() {}
SocketHandler::~SocketHandler() {}

void SocketHandler::listen(const string &ip, int port)
{
    m_server = new ServerSocket(ip, port);
}

void SocketHandler::handle(int max_connects, int wait_time)
{
    m_epoll = new EventPoller(false);
    m_epoll->create(max_connects);
    m_epoll->add(m_server->m_sockfd, m_server, (EPOLLIN | EPOLLHUP | EPOLLERR));
    m_sockpool.init(max_connects);
    while (1)
    {
        int num = m_epoll->wait(wait_time);
        if (num == 0)
        {
            continue;
        }
        for (int i = 0; i < num; i++)
        {
            // a new client try to connect -> server
            if (m_server == static_cast<Socket *>(m_epoll->m_events[i].data.ptr))
            {
                int soctfd = m_server->accept();
                // 1 connect <-> 1 socket
                Socket *socket = m_sockpool.allocate();
                if (socket == nullptr)
                {
                    break;
                }
                socket->m_sockfd = soctfd;
                socket->set_non_blocking();
                attach(socket);
            }
            else
            {
                // handle client event
                Socket *socket = static_cast<Socket *>(m_epoll->m_events[i].data.ptr);
                if (m_epoll->m_events[i].events & EPOLLHUP)
                {
                    detach(socket);
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLERR)
                {
                    detach(socket);
                    remove(socket);
                }
                else if (m_epoll->m_events[i].events & EPOLLIN)
                {
                    detach(socket);
                    Task * task = TaskFactory::create(socket);
                    Singleton<TaskDispatcher>::instance()->assign(task);
                }
            }
        }
    }
}

void SocketHandler::attach(Socket *socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->add(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket *socket)
{
    AutoLock lock(&m_mutex);
    m_epoll->del(socket->m_sockfd, (void *)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket *socket)
{
    socket->close();
    m_sockpool.release(socket);
}