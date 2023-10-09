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

class CPUop :public InfoStrategy {
public:
    virtual void sendInfo(Info& info) {
        log_debug("CPUop InfoStrategy");
        int cmd = info.get_pkg().cmd_type;
        int cmd_detail = info.get_pkg().cmd_detail;

        char str[64] = "echo 0 | sudo tee /sys/devices/system/cpu/cpu";
        strcat(str, to_string(cmd_detail).c_str());
        strcat(str, string("/online").c_str());
        log_debug("str: %s", str);
        FILE* fp = popen(str, "r");
        if(fp==NULL){
            log_error("%s",strerror(errno));
            return;
        }
	    log_debug("excute str cmd success");

        char temp_buf[1024];
        while (fgets(temp_buf, 1024, fp) != NULL) {
            log_info("%s", info.get_pkg().content_buf);
            strcat(info.get_pkg().content_buf, temp_buf); // 追加到缓冲区末尾
        }
        fp=popen("cat /sys/devices/system/cpu/online","r");
        if(fp==NULL){
            log_error("%s",strerror(errno));
            return;
        }
	    log_debug("excute str cmd success");

        while (fgets(temp_buf, 1024, fp) != NULL) {
            log_info("%s", info.get_pkg().content_buf);
            strcat(info.get_pkg().content_buf, temp_buf); // 追加到缓冲区末尾
        }

        pclose(fp);

        strcat(info.get_pkg().content_buf, "\nkill cpu success");
        
        log_debug("%d,%d,%s", info.get_pkg().cmd_type, info.get_pkg().cmd_detail, info.get_pkg().content_buf);
        info.send_info(1024);
    }
};
