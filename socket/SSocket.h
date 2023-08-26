#pragma once

#include "Socket.h"
namespace bb {
namespace socket {
class SSocket : public Socket {

public:
  SSocket();
  SSocket(const string& ip, const int port);
  ~SSocket();

  int sock_accept();
};}}