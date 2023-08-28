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
namespace bb {
namespace socket {
const uint32_t buf_size = 4096;

struct MsgHead{
  char tag[32];
  uint32_t cmd;
};

class Socket {
  friend class SocketHandler;

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
  bool close();

protected:
  string m_ip;
  int m_port;
  int m_sockfd;
  };
}}