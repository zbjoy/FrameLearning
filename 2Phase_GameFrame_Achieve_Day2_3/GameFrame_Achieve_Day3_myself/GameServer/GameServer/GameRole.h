#pragma once
#include <zinx.h>
#include "AOIWorld.h"

class GameMsg;
class GameProtocol;
class GameRole :
    public Irole, public Player
{
    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;

    GameMsg* CreateIDNameLogin();

    GameMsg* CreateSrdPlayerPosition();

    GameMsg* CreateSelfPosition();
    
public:
    GameRole();
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProtocol* m_proto = nullptr;

    std::string szName;
    int Pid;

    // 通过 Player 继承
    float GetX() override;
    float GetY() override;
};

