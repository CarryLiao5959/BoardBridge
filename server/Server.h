#pragma once

#include <string>
using namespace std;

namespace bb {
namespace server {

class Server {
  public:
    Server();
    ~Server();

    void start();

    void set_ip(const string &ip);
    void set_port(int port);
    void set_wait_time(int wait_time);
    void set_threadpool_num(int threadpool_num);
    void set_socketpool_num(int socketpool_num);

  private:
    string m_ip;
    int m_port;
    int m_threadpool_num;
    int m_socketpool_num;
    int m_wait_time;
};

} // namespace server
} // namespace bb