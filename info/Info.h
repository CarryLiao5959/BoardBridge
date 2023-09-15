#pragma once

#include "Socket.h"
using namespace bb::socket;

#include "ConstPara.h"
#include<map>
#include<memory>

class InfoStrategy;
class SysInfo;

namespace bb {
namespace info {

  class Info {
    friend class SysInfo;
    std::map<int, std::unique_ptr<InfoStrategy>> strategies;
  public:
    Info();
    Info(int sockfd);
    Info(int sockfd, u_int64_t cmd_type, u_int64_t cmd_detail);
    ~Info();

    void init_map();

    FILE *open_pipe(const char *type);
    size_t read_pipe(FILE *fp);
    size_t read_file(const string filename);
    int send_info(int nbytes);
    void info();

    void get_cmd_content();
    int get_sockfd() { return m_sockfd; }
    char* get_buf() { return m_buf; }
    struct InfoPackage& get_pkg() { return m_package; }

    void executeStrategy(int cmdType);

    
private:
  InfoStrategy* strategy;

protected:
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