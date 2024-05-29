#pragma once
#include <zinx.h>

class GameMsg;
class GameProtocol;
class GameRole :
    public Irole
{
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

public:
    GameProtocol* m_protocol;

private:
    GameMsg* CreateLoginIDName();

private:
    int m_Pid;
    std::string m_Name;
};

