#include "Info.h"
using namespace bb::info;
#include "Logger.h"
#include "JsonHandler.h"
#include "Singleton.h"
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
    "PROC_INFO(recently)",
    "PROC_INFO(from start)",
    "BACKUP",
    "CONTROL",
    "KILL SERVER"};

const char *Info::commands[] = {
    "",
    "top -bn1 | head -n 5",
    "ps -eo pid,ppid,comm,state,stime --sort=-start_time | head -n 20",
    "ps -eo pid,ppid,comm,state,stime | head -n 20",
    "ls",
    "kill -9 "};

const char *Info::filepaths[] = {
    "",
    "/home/carry/BoardBridge/info/sys.json",
    "/home/carry/BoardBridge/info/recentproc.json",
    "/home/carry/BoardBridge/info/proc.json",
    };

FILE *Info::open_pipe(const char *type = "r") {
    log_debug("open_pipe");
    FILE *fp;
    int cmd = m_package.cmd_type;
    switch (cmd) {
    case 0:
        return nullptr;
    case 1:
    case 2:
    case 3:
    case 4:
        fp = popen(commands[cmd], type);
        break;
    case 5:
        char str[50];
        strcpy(str, commands[cmd]);
        strcat(str, to_string(m_package.cmd_detail).c_str());
        strcat(str, string(" 2>&1 | tee /dev/stderr").c_str());
        log_debug("str: %s", str);
        fp = popen(str, type);
        break;
    }
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
    // memset(m_package.content_buf, 0, buf_size);
    if(feof(fp)){
        return 0;
    }
    if (!fgets(m_buf, buf_size, fp)) {
        log_error("fgets failed|errno=%d,errstr=%s", errno, strerror(errno));
        return -1;
    }
    int line_len = strlen(m_buf);
    int nbytes = fread(m_buf + line_len, 1, buf_size - line_len - 1, fp);
    log_debug("m_buf=[%s],len=[%d]", m_buf, line_len + nbytes);
    if (ferror(fp)) {
        log_error("read failed|errno=%d,errstr=%s", errno, strerror(errno));
        return -1;
    }
    return line_len + nbytes;
}

size_t Info::read_file(const string filename) {
    log_debug("read_file");

    ifstream file(filename,ifstream::binary);
    if(!file){
        log_error("open file %s fail", filename.c_str());
        return -1;
    }
    // Go to the end of the file
    file.seekg(0, std::ios::end);
    // Get the current position (which is the file size)
    std::streampos fileSize = file.tellg();

    // Go back to the start of the file and read its content
    file.seekg(0, std::ios::beg);
    file.read(m_package.content_buf, fileSize);

    file.close();
    return fileSize;
}

void Info::get_cmd_content() {
    string m_cmd_content = m_buf;
    // log_debug("%s", m_cmd_content.c_str());
    JsonHandler *jh = new JsonHandler("/home/carry/BoardBridge/info/");
    switch (m_package.cmd_type) {
    case 1:
        log_debug("get_sys_json");
        jh->set_filename("sys.json");
        jh->get_sys_json(m_cmd_content);
        break;
    case 2:
        log_debug("get_recent_proc_json");
        jh->set_filename("recentproc.json");
        jh->get_proc_json(m_cmd_content);
        break;
    case 3:
        log_debug("get_proc_json");
        jh->set_filename("proc.json");
        jh->get_proc_json(m_cmd_content);
        break;
    default:
        log_warn("invalid command!");
    }
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

    m_package.content_len = nbytes;

    int ret = send(m_sockfd, &m_package, sizeof(m_package), 0);
    if (ret < 0) {
        log_error("send failed: errno=%d errstr=%s", errno, strerror(errno));
        return ret;
    }
    log_debug("send success");
    memset(m_package.content_buf, 0, buf_size);
    return ret;
}

void Info::info(int cmd,int cmd_detail) {
    m_package.cmd_type = cmd;
    m_package.cmd_detail = cmd_detail;
    FILE *fp = open_pipe();
    if(cmd==5){
        pclose(fp);
        return;
    }
    read_pipe(fp);
    get_cmd_content();
    size_t nbytes = read_file(filepaths[cmd]);
    send_info(nbytes);
    pclose(fp);
}