#pragma once
#include <zinx.h>
#include <ZinxTCP.h>

class StdInChannel :
    public Ichannel
{
public:
    // 通过 Ichannel 继承
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class StdOutChannel : public Ichannel
{
public:

    // 通过 Ichannel 继承
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class myTcpData : public ZinxTcpData
{
public:
    myTcpData(int _fd) : ZinxTcpData(_fd) {}

    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class myFact : public IZinxTcpConnFact
{
public:

    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
