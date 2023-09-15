#pragma once
#include"InfoStrategy.h"
#include "Info.h"
using namespace bb::info;

class InfoStrategy {
public:
    virtual void sendInfo(Info& info) = 0;
    virtual ~InfoStrategy() {}
};