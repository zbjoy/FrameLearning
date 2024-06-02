#pragma once
#include "Ichannel.h"

class StdinChannel :
    public Ichannel
{

public:
    // 通过 Ichannel 继承
    int GetFd() override;
    std::string ReadFd(std::string _input) override;
    void WriteFd(std::string _output) override;

    void SetOut(Ichannel* _pChannel)
    {
        pOut = _pChannel;
    }

    // 通过 Ichannel 继承
    void data_process(std::string _output) override;

private:
    Ichannel* pOut;
};

