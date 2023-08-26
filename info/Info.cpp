#include "Info.h"
using namespace bb::info;
#include "Logger.h"
using namespace bb::util;

#include <stdio.h>

Info::Info() {}

Info::Info(int sockfd) {
    m_sockfd = sockfd;
}

Info::~Info() {}

const char *Info::cmd_typename[] = {
    "EXIT",
    "SYS_INFO",
    "PROC_INFO",
    "CMD_NUM"
};

const char *Info::commands[] = {
    "",
    "top -bn1 | head -n 5",
    "ps -eo pid,ppid,comm,state,stime --sort=-start_time | head -n 20"
};

FILE *Info::open_pipe(int cmd, const char *type = "r") {
    log_debug("open_pipe");
    FILE *fp;
    fp = popen(commands[cmd], type);
    log_debug("commands[cmd]: %s", commands[cmd]);
    if (fp == NULL) {
        log_error("Error executing command");
        return NULL;
    }
    return fp;
}

size_t Info::read_pipe(FILE *fp) {
    log_debug("read_pipe");
    memset(m_buf, 0, buf_size);
    if (!fgets(m_buf, buf_size, fp)) {
        log_error("read failed");
        return -1;
    }
    int line_len = strlen(m_buf);
    int nbytes = fread(m_buf + line_len, 1, buf_size - line_len - 1, fp);
    log_debug("m_buf=[%s]", m_buf);
    if (ferror(fp)) {
        log_error("read failed");
        return -1;
    }
    return line_len + nbytes;
}

int Info::send_info(int nbytes) {
    log_debug("m_sockfd=[%d]", m_sockfd);
    int error = 0;
    socklen_t len = sizeof(error);
    int retval = getsockopt(m_sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
    if (retval != 0) {
        log_error("getsockopt failed: errno=%d errstr=%s", errno, strerror(errno));
    } else if (error != 0) {
        log_error("m_sockfd error: errno=%d errstr=%s", errno, strerror(errno));
    }
    int ret = send(m_sockfd, m_buf, nbytes, 0);
    if (ret < 0) {
        log_error("send failed: errno=%d errstr=%s", errno, strerror(errno));
        return ret;
    }
    log_debug("send success");
    memset(m_buf, 0, buf_size);
    return ret;
}

void Info::info(int cmd) {
    FILE *fp = open_pipe(cmd);
    size_t nbytes = read_pipe(fp);
    send_info(nbytes);
    pclose(fp);
}