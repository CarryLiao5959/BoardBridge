#pragma once

#include "Socket.h"

class CSocket : public Socket {

public:
  CSocket();
  CSocket(const string& ip, const int port);
  ~CSocket();

  int sock_conn();

  int set_recv_timeout(int second);
  int set_send_timeout(int second);
  int gethostbyname(const char* name);
};