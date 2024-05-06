#pragma once
#include <zinx.h>

class GameProto;
class GameMsg;
class GameRole :
    public Irole
{
    GameMsg* CreateSrdPlayerPosition();
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProto* m_proto;
};

