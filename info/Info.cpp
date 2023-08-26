#include "Info.h"
using namespace bb::info;

#include <stdio.h>

Info::Info() {}

Info::Info(int  sockfd) {
    m_sockfd = sockfd;
}

Info::~Info() {}

const char* Info::commands[] = {
    "",
    "top -bn1 | head -n 5",
    "ps -eo pid,ppid,comm,state,stime --sort=-start_time | head -n 20"
};

FILE* Info::open_pipe(int cmd, const char* type = "r") {
    FILE* fp;
    fp = popen(commands[cmd],type);
    printf("commands[cmd]: %s\n",commands[cmd]);
    if (fp == NULL)
    {
        perror("Error executing command");
        return NULL;
    }
    return fp;
}

size_t Info::read_pipe(FILE* fp) {
    memset(m_buf, 0, buf_size);
    if (!fgets(m_buf, buf_size, fp)) {
        perror("read failed");
        return -1;
    }
    int line_len = strlen(m_buf);
    int nbytes = fread(m_buf + line_len, 1, buf_size - line_len - 1, fp);
    if (ferror(fp))
    {
        perror("read failed");
        return -1;
    }
    return line_len + nbytes;
}

int Info::send_info(int nbytes) {
    int ret = send(m_sockfd, m_buf, nbytes, 0);
    if (ret < 0)
    {
        perror("send failed\n");
        return ret;
    }
    printf("send success");
    memset(m_buf, 0, buf_size);
    return ret;
}

void Info::info(int cmd)
{
    FILE* fp = open_pipe(cmd);
    size_t nbytes = read_pipe(fp);
    send_info(nbytes);
    pclose(fp);
}