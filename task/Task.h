#pragma once

#include "Mutex.h"
using namespace bb::thread;
#include "Info.h"
using namespace bb::info;

namespace bb {
namespace task {

class Task {
  public:
    Task();
    Task(void *data);
    virtual ~Task();

    virtual void run() = 0;
    virtual void destroy() = 0;

    void *get_data();
    void set_data(void *data);
    int get_cmd()const{
        return m_cmd;
    }
    void set_cmd(int cmd,int cmd_detail=0);

  protected:
    void *m_data;
    Mutex m_mutex;
    int m_cmd;
    int m_cmd_detail;
};

} // namespace thread
} // namespace bb
