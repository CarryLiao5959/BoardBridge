#pragma once

#include "Socket.h"

#define EXIT 0
#define SYS_INFO 1
#define PROC_INFO 2

class Info{

public:
    Info();
    Info(int  sockfd);
    ~Info();

    FILE * open_pipe(int cmd,const char *type);
    size_t read_pipe(FILE *fp);

    int send_info(int nbytes);

    void info(int cmd);

private:
    int  m_sockfd;
    char m_buf[buf_size];
    static const char* commands[10];

};