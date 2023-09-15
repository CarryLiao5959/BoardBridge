#pragma once

#include "InfoStrategy.h"
#include "Singleton.h"
using namespace bb::util;
#include "FileHandler.h"
#include "Info.h"
using namespace bb::info;
#include "ConstPara.h"

#include<string>

using namespace std;

class SysInfo :public InfoStrategy {
public:
    virtual void sendInfo(InfoPackage* package, int sockfd) {
        string filename = "info/sys.json";
        // FILE* fp = Singleton<FileHandler>::instance(filename)->open_pipe();
        // size_t nbytes = Singleton<FileHandler>::instance()->read_pipe(fp);
        // get_cmd_content();
        size_t nbytes = Singleton<FileHandler>::instance(filename)->read_file(package->content_buf);
        log_debug("%d,%d,%s", package->cmd_type, package->cmd_detail, package->content_buf);
        send_info(nbytes, sockfd, package);
        // pclose(fp);
    }
    
    int send_info(int nbytes, int sockfd, InfoPackage* package) {
        log_debug("sockfd=[%d]", sockfd);
        int error = 0;
        socklen_t len = sizeof(error);
        int retval = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        if (retval != 0) {
            log_error("getsockopt failed: errno=%d errstr=%s", errno, strerror(errno));
        } else if (error != 0) {
            log_error("sockfd error: errno=%d errstr=%s", errno, strerror(errno));
        }

        package->content_len = nbytes;

        int ret = send(sockfd, &package, sizeof(package), 0);
        if (ret < 0) {
            log_error("send failed: errno=%d errstr=%s", errno, strerror(errno));
            return ret;
        }
        log_debug("send success");
        memset(package->content_buf, 0, buf_size);
        return ret;
    }
private:
    static const string cmd;
};

const string SysInfo::cmd = "top -bn1 | head -n 5";