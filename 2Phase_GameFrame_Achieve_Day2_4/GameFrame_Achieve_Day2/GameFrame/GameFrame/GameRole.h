#pragma once
#include <zinx.h>
#include "AOIWorld.h"
#include "GameMsg.h"

class GameProtocol;
class GameRole :
    public Irole, public Player
{
    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;

    int iPid = 0;
    std::string szName;

    GameMsg* CreateIDNameLogin();

    GameMsg* CreateSrdPlayerPostion();

    GameMsg* CreateSelfPostion();

    GameMsg* CreateIDNameLogoff();
public:
    GameRole();
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProtocol* m_proto = NULL;

    // 通过 Player 继承
    float GetX() override;
    float GetY() override;
};

