#pragma once
#include "Ichannel.h"

class StdoutChannel :
    public Ichannel
{
public:
    // 通过 Ichannel 继承
    int GetFd() override;
    std::string ReadFd(std::string _input) override;
    void WriteFd(std::string _output) override;
    void data_process(std::string _output) override;
};

