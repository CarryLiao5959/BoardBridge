#include "SSocket.h"
using namespace bb::socket;
#include "Logger.h"
using namespace bb::util;

SSocket::SSocket() : Socket() {

}

SSocket::SSocket(const string& ip, const int port) :Socket(ip, port) {

}

SSocket::~SSocket() {

}

int SSocket::sock_accept() {
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    int conn_sock = accept(m_sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (conn_sock == -1)
    {
        log_error("sock_accept error: errno=%d errstr=%s", errno, strerror(errno));
        return conn_sock;
    }
    log_info("accept success: conn_sock %d", conn_sock);
    return conn_sock;
}