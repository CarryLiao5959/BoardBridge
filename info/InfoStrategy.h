#pragma once

#include "ConstPara.h"

class InfoStrategy {
public:
    virtual void sendInfo(InfoPackage* package, int sockfd) = 0;
    virtual ~InfoStrategy() {}
};