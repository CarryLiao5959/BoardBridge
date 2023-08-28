#include "SocketHandler.h"
using namespace bb::socket;
#include "TaskFactory.h"
using namespace bb::task;
#include "Singleton.h"
#include "Info.h"
using namespace bb::info;

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

void SocketHandler::listen(const string &ip, int port) {
    m_server = new SSocket(ip, port);
    int serv_sock = m_server->sock_init();
    m_server->set_reuse_addr();
    m_server->set_keep_alive();
    m_server->sock_bind();
    m_server->sock_listen();
}

void SocketHandler::handle(int max_connects, int wait_time) {
    int serv_sock = m_server->get_fd();
    m_select = new Select(serv_sock);
    log_info("serv_sock: %d", serv_sock);
    m_select->add_fd(serv_sock);
    m_sockpool.init(max_connects);
    while (1)
    {
        int infds = m_select->select();
        for (int efd = 0; efd <= m_select->get_max_fd(); efd++) {
            if (!m_select->isset(efd, true)) {
                continue;
            }
            // client connect
            if (efd == serv_sock)
            {
                log_debug("efd from serv_sock [%d]", serv_sock);
                int conn_sock = m_server->sock_accept();
                Socket *socket = m_sockpool.get();
                if (socket == nullptr) {
                    log_warn("no socket available in socketpool");
                    break;
                }
                socket->m_sockfd = conn_sock;
                active_sock[conn_sock]=socket;
                if (conn_sock > 0) {
                    attach_sock(socket);
                }
            }
            // client send/recv
            else
            {
                Socket *socket;
                if (active_sock.count(efd)) {
                    socket = active_sock[efd];
                }else{
                    socket = m_sockpool.get();
                    if (socket == nullptr) {
                        log_warn("no socket available in socketpool");
                        break;
                    }
                    socket->m_sockfd = efd;
                }
                log_info("efd from cli_sock [%d]", efd);
                //recv
                struct InfoPackage package;
                memset(&package, 0, sizeof(package));
                int len = recv(efd, &package, sizeof(package), 0);
                log_debug("recv data: %s", package.content_buf);
                if (len == -1)
                {
                    log_error("recv fail");
                    detach_sock(socket);
                    remove_to_pool(socket);
                } else if (len == 0) {
                    log_warn("recv timeout");
                    detach_sock(socket);
                    remove_to_pool(socket);
                } else {
                    detach_sock(socket);
                    Task *task = TaskFactory::create(socket);
                    task->set_cmd(package.cmd_type, package.cmd_detail);
                    Singleton<TaskDispatcher>::instance()->assign(task);
                }
            }
        }
    }
}

void SocketHandler::attach_sock(Socket *socket) {
    Autolock lock(&m_mutex);
    m_select->add_fd(socket);
}

void SocketHandler::detach_sock(Socket *socket) {
    Autolock lock(&m_mutex);
    m_select->rm_fd(socket);
}

void SocketHandler::remove_to_pool(Socket *socket) {
    socket->close();
    m_sockpool.put(socket);
}