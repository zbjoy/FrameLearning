#pragma once
#include <zinx.h>
#include "AOIWorld.h"
#include "msg.pb.h"

class GameMsg;
class GameProtocol;
class GameRole :
    public Irole, public Player
{
public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    // 通过 Player 继承
    float GetX() override;
    float GetY() override;


public:
    GameProtocol* m_protocol;

private:
    GameMsg* CreateLoginIDName();
    GameMsg* CreateSrdPosition();
    GameMsg* CreateSelfPosition();

    void ProcChatMsg(google::protobuf::Message* _pMsg);
    // void ProcChatContent(pb::Talk* pMsg);
    void ProcMoveMsg(google::protobuf::Message* _pMsg);

private:
    int m_Pid;
    std::string m_Name;

    float x;
    float y;
    float z;
    float v;
};

