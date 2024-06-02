#pragma once
#include <string>
#include "Ichannel.h"

class StdioChannel :
    public Ichannel
{
    std::string content;
public:
    // 通过 Ichannel 继承
    int GetFd() override;
    void ReadFd() override;
    void WriteFd() override;
};

