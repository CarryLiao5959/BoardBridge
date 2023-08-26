#include "CSocket.h"
using namespace bb::socket;

#include <netdb.h>

CSocket::CSocket() : Socket() {

}

CSocket::CSocket(const string& ip, const int port) :Socket(ip, port) {

}

CSocket::~CSocket() {

}

int CSocket::sock_conn() {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    serv_addr.sin_port = htons(m_port);
    socklen_t addr_len;
    addr_len = sizeof(serv_addr);
    if (connect(m_sockfd, (struct sockaddr*)&serv_addr, addr_len) < 0)
    {
        perror("connect error");
        return -1;
    }
    printf("connect serv_addr success\n");
    return 0;
}

int CSocket::set_recv_timeout(int second) {
    struct timeval timeout;
    timeout.tv_sec = second;
    timeout.tv_usec = 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt SO_RCVTIMEO failed");
        return -1;
    }
    printf("setsockopt SO_RCVTIMEO success\n");
    return 0;
}

int CSocket::set_send_timeout(int second) {
    struct timeval timeout;
    timeout.tv_sec = second;
    timeout.tv_usec = 0;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0)
    {
        perror("setsockopt SO_RCVTIMEO failed\n");
        return -1;
    }
    printf("setsockopt SO_SNDTIMEO success\n");
    return 0;
}

int CSocket::gethostbyname(const char* name) {
    struct hostent* host;
    host = ::gethostbyname(name);
    if (host == NULL)
    {
        perror("gethostbyname error");
        return -1;
    }
    printf("gethostbyname success\n");
    printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)host->h_addr_list[0]));
    return 0;
}