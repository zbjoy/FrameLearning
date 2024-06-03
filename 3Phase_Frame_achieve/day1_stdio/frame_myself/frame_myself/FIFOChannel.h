#pragma once
#include "Ichannel.h"

class FIFOChannel :
    public Ichannel
{
public:
    FIFOChannel(std::string _file, bool _bIsRead = true) : file(_file), bIsRead(_bIsRead) {};

    // 通过 Ichannel 继承
    int GetFd() override;

    std::string ReadFd(std::string _input) override;

    void WriteFd(std::string _output) override;

    void data_process(std::string _output) override;


    // 通过 Ichannel 继承
    bool Init() override;

    void Fini() override;

    FIFOChannel* pHandler = nullptr;

private:

    std::string file;
    bool bIsRead;
    int m_fd;
};

