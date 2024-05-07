#pragma once
#include <zinx.h>
#include "AOIWorld.h"

class GameProto;
class GameMsg;
class GameRole :
    public Irole, public Player
{
    GameMsg* CreateIDNameLogin();
    GameMsg* CreateSrdPlayerPosition();
    GameMsg* CreateSelfPosition();

    float x = 0;
    float y = 0;
    float z = 0;
    float v = 0;

    int iPid = 0;
    std::string szName;

public:
    GameRole();
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProto* m_proto = nullptr;

    // 通过 Player 继承
    float GetX() override;
    float GetY() override;
};

