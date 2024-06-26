#pragma once
#include <zinx.h>
class EchoRole :
    public Irole
{
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};

class OutputCtl : public Irole
{
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    Ichannel* pOut;
};

class DataPreCtl : public Irole
{
public:

    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;
};

