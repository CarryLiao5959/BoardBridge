#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

const uint32_t buf_size = 1024;

struct MsgHead{
  char tag[32];
  uint32_t cmd;
};

class Socket {
public:
  Socket();
  Socket(const string& ip, const int port);
  ~Socket();

  int sock_init();
  int sock_bind();
  int sock_listen();

  int set_keep_alive();
  int set_reuse_addr();

  int get_fd();

protected:
  string m_ip;
  int m_port;
  int m_sockfd;
};