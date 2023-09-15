#pragma once

#include "Socket.h"
using namespace bb::socket;
#include"InfoStrategy.h"
#include "ConstPara.h"

namespace bb {
namespace info {

class Info {

  public:
    Info();
    Info(int sockfd);
    Info(int sockfd, u_int64_t cmd_type, u_int64_t cmd_detail);
    ~Info();

    FILE *open_pipe(const char *type);
    size_t read_pipe(FILE *fp);
    size_t read_file(const string filename);
    int send_info(int nbytes);
    void info();

    void get_cmd_content();

private:
  InfoStrategy* strategy;

private:
    int m_sockfd;
    char m_buf[buf_size];
    struct InfoPackage m_package;

  public:
    static const char *commands[10];
    static const char *cmd_typename[10];
    static const char *filepaths[10];
};
} // namespace info
} // namespace bb