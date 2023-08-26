#pragma once

#include "Socket.h"
using namespace bb::socket;

namespace bb {
namespace info {

class Info {

  public:
    Info();
    Info(int sockfd);
    ~Info();

    FILE *open_pipe(int cmd, const char *type);
    size_t read_pipe(FILE *fp);

    int send_info(int nbytes);

    void info(int cmd);

  private:
    int m_sockfd;
    char m_buf[buf_size];
  public:
    static const char *commands[10];
    static const char *cmd_typename[10];
};
} // namespace info
} // namespace bb