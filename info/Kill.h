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
#include<errno.h>
#include<string.h>
#include<cstdlib>

using namespace std;

class Kill :public InfoStrategy {
public:
    virtual void sendInfo(Info& info) {
        log_debug("Kill InfoStrategy");
        int cmd = info.get_pkg().cmd_type;
        int cmd_detail = info.get_pkg().cmd_detail;

        char str[64] = "kill -9 ";
        strcat(str, to_string(cmd_detail).c_str());
        strcat(str, string(" 2>&1 | tee /dev/stderr").c_str());
        log_debug("str: %s", str);
        FILE* fp = popen(str, "r");
        if(fp==NULL){
            log_error("%s",strerror(errno));
            return;
        }
	    log_debug("kill pid %d success",cmd_detail);

        while (fgets(info.get_pkg().content_buf, 1024, fp) != NULL) {
            log_info("%s", info.get_pkg().content_buf);
        }
        pclose(fp);

        strcat(info.get_pkg().content_buf, "\nkill proc success");
        
        log_debug("%d,%d,%s", info.get_pkg().cmd_type, info.get_pkg().cmd_detail, info.get_pkg().content_buf);
        info.send_info(1024);
    }
};
