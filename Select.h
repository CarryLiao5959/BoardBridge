#pragma once

#include "Socket.h"

class Select {
public:
  Select();
  Select(int max_fd);
  Select(fd_set &rdset, int max_fd);
  ~Select();

  int select();


    void clear_set();
  void add_fd(int fd);
  void rm_fd(int fd);
    bool isset(int fd,bool tmp);

  void set_max_fd(int max_fd);
  int get_max_fd();

protected:
  fd_set m_rdset;
  fd_set m_tmpset;
  int m_max_fd;
};