#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#define buf_size 4096

struct InfoPackage {
    u_int64_t cmd_type;
    u_int64_t cmd_detail;
    u_int64_t content_len;
    char content_buf[buf_size];
};

const char *cmd_typename[] = {
    "EXIT",
    "SYS_INFO",
    "PROC_INFO",
    "BACKUP",
    "CONTROL",
    "CMD_NUM"
};