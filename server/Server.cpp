#include "Server.h"
using namespace bb::server;
#include "Singleton.h"
#include "Logger.h"
using namespace bb::util;
#include "TaskDispatcher.h"
#include "Autolock.h"
using namespace bb::thread;
#include "SocketHandler.h"
using namespace bb::socket;

Server::Server() : m_ip(""), m_port(8080), m_threadpool_num(8), m_socketpool_num(8), m_wait_time(10) {}

Server::~Server() {}

void Server::start() {
    TaskDispatcher *task_dispatcher = Singleton<TaskDispatcher>::instance();
    task_dispatcher->init_threadpool(m_threadpool_num);

    SocketHandler *socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(m_ip, m_port);
    socket_handler->handle(m_socketpool_num, m_wait_time);
}

void Server::set_ip(const string &ip) {
    m_ip = ip;
    log_debug("set_ip: %s", m_ip.c_str());
}

void Server::set_port(int port) {
    m_port = port;
    log_debug("set_port: %d", m_port);
}

void Server::set_wait_time(int wait_time) {
    m_wait_time = wait_time;
    log_debug("set_wait_time: %d", m_wait_time);
}

void Server::set_threadpool_num(int threadpool_num) {
    m_threadpool_num = m_threadpool_num;
    log_debug("set_threadpool_num: %d", m_threadpool_num);
}

void Server::set_socketpool_num(int socketpool_num) {
    m_socketpool_num = socketpool_num;
    log_debug("set_socketpool_num: %d", m_socketpool_num);
}