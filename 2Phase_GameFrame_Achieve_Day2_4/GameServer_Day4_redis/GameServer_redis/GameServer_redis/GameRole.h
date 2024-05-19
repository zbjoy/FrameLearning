#pragma once
#include <zinx.h>
#include "AOIWorld.h"

class GameMsg;
class GameProtocol;
class GameRole :
    public Irole, public Player
{
    std::string m_Name;
    int m_pid;

    float x;
    float y;
    float z;
    float v;

public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProtocol* m_protocol;

private:

    GameMsg* CreateLoginIDName();
    GameMsg* CreateSrdPlayerPosition();
    GameMsg* CreateSelfPosition();

    // 通过 Player 继承
    float GetX() override;
    float GetY() override;
};

