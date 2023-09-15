#ifndef CONSTPARA_H
#define CONSTPARA_H

#include <cstdint>

#define buf_size 4096

struct InfoPackage {
    uint64_t cmd_type;          // 命令类型
    uint64_t cmd_detail;        // 特定命令类型下的附加信息
    uint64_t content_len;       // 内容长度
    char content_buf[buf_size];  // 内容
};

#endif // CONSTPARA_H