#include "Socket.h"
using namespace bb::socket;
#include "Logger.h"
using namespace bb::util;

Socket::Socket(){

}

Socket::Socket(const string& ip,const int port):m_ip(ip),m_port(port),m_sockfd(0){

}

Socket::~Socket(){

}

int Socket::sock_bind(){
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    if(m_ip!=" "){
        serv_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    }else{
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    serv_addr.sin_port = htons(m_port);
    int ret=bind(m_sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(ret<0){
        log_error("sock_bind error: errno=%d errstr=%s", errno, strerror(errno));
        exit(1);
    }
    log_info("bind success");
    return ret;
}

int Socket::sock_listen(){
    int ret = listen(m_sockfd, 5);
    if(ret<0){
        log_error("sock_listen error: errno=%d errstr=%s", errno, strerror(errno));
        exit(1);
    }
    log_info("listen success");
    return ret;
}

int Socket::sock_init(){
    int sockfd=::socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        log_error("create socket error: errno=%d errstr=%s", errno, strerror(errno));
        exit(1);
    }
    m_sockfd=sockfd;
    log_info("sock set success: %d", sockfd);
    return sockfd;
}

int Socket::set_keep_alive(){
    int opt = 1;
    unsigned int len = sizeof(opt);
    int ret=setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &opt, len);
    if(ret<0){
        log_error("set_keep_alive error: errno=%d errstr=%s", errno, strerror(errno));
        exit(1);
    }
    log_info("setsockopt SO_KEEPALIVE success");
    return ret;
}

int Socket::set_reuse_addr(){
    int opt = 1;
    unsigned int len = sizeof(opt);
    int ret=setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    if(ret<0){
        log_error("set_reuse_addr error: errno=%d errstr=%s", errno, strerror(errno));
        exit(1);
    }
    log_info("setsockopt SO_REUSEADDR success");
    return ret;
}

int Socket::get_fd(){
    return m_sockfd;
}

bool Socket::close() {
    if (m_sockfd > 0) {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
    log_warn("Socket closed: %d", m_sockfd);
    return true;
}
