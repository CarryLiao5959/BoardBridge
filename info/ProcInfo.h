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

class ProcInfo :public InfoStrategy {
public:
    virtual void sendInfo(Info& info) {
        log_debug("ProcInfo InfoStrategy");
        
        string filepath = dirname + filename;
        FILE* fp = Singleton<FileHandler>::instance(filepath)->open_pipe(cmd.c_str());
        Singleton<FileHandler>::instance()->read_pipe(fp);
        char* buf = Singleton<FileHandler>::instance()->get_buf();
        pclose(fp);

        // get_cmd_content();
        JsonHandler* jh = new JsonHandler(dirname);
        log_debug("get_recentproc_json");
        jh->set_filename(filename);

        log_debug("buf: %s", buf);
        jh->get_proc_json(buf);
        Singleton<DBHandler>::instance()->get_json_data(dirname+filename);
        Singleton<DBHandler>::instance()->save_rproc_to_db();

        Singleton<FileHandler>::instance()->set_filename(filepath);
        size_t nbytes = Singleton<FileHandler>::instance()->read_file(info.get_pkg().content_buf);
        log_debug("%d,%d,%s", info.get_pkg().cmd_type, info.get_pkg().cmd_detail, info.get_pkg().content_buf);
        info.send_info(nbytes);
        // send_info(nbytes, sockfd, package);
    }
private:
    static const string cmd;
    static const string dirname;
    static const string filename;
};

const string ProcInfo::cmd = "ps -eo pid,ppid,comm,state,stime --sort=-start_time | head -n 20";
const string ProcInfo::dirname = "info/";
const string ProcInfo::filename = "recentproc.json";