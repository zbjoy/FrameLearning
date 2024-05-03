#pragma once
#include <zinx.h>
#include <ZinxTCP.h>

class InOutCtl :
    public Irole
{
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    bool isOpen = true;
    Ichannel* pOutChannel;
};

class AddDatePreCtl : public Irole
{
public:

    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    bool isNeedDatePre = false;
};

class Echo : public Irole
{
public:

    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};

class myTcpData : public ZinxTcpData
{
public:
    myTcpData(int _fd) : ZinxTcpData(_fd) {}
    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class myTcpConnFact : public IZinxTcpConnFact
{
public:

    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
