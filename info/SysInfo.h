#pragma once

#include "InfoStrategy.h"
#include "Singleton.h"
#include "JsonHandler.h"
using namespace bb::util;
#include "FileHandler.h"
#include "Info.h"
using namespace bb::info;
#include "ConstPara.h"
#include "DBHandler.h"
using namespace bb::db;

#include<string>

using namespace std;

class SysInfo :public InfoStrategy {
public:
    virtual void sendInfo(Info& info) {
        log_debug("SysInfo InfoStrategy");
        
        string filename = "info/sys.json";
        FILE* fp = Singleton<FileHandler>::instance(filename)->open_pipe(cmd.c_str());
        Singleton<FileHandler>::instance()->read_pipe(fp);
        char* buf = Singleton<FileHandler>::instance()->get_buf();
        pclose(fp);
        
        // get_cmd_content();
        JsonHandler* jh = new JsonHandler("info/");
        log_debug("get_sys_json");
        jh->set_filename("sys.json");

        log_debug("buf: %s", buf);
        jh->get_sys_json(buf);
        Singleton<DBHandler>::instance()->get_json_data("info/sys.json");
        Singleton<DBHandler>::instance()->save_sys_to_db();
        
        size_t nbytes = Singleton<FileHandler>::instance(filename)->read_file(info.get_pkg().content_buf);
        log_debug("%d,%d,%s", info.get_pkg().cmd_type, info.get_pkg().cmd_detail, info.get_pkg().content_buf);
        info.send_info(nbytes);
        // send_info(nbytes, sockfd, package);
    }
private:
    static const string cmd;
};

const string SysInfo::cmd = "top -bn1 | head -n 5";