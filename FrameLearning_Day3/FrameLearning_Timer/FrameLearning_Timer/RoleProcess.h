#pragma once
#include <zinx.h>

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

