#pragma once
#include <zinx.h>
#include "AOIWorld.h"
#include "msg.pb.h"

class GameProtocol;
class GameMsg;
class GameRole :
    public Irole, public Player
{
    float x;
    float y;
    float z;
    float v;

public:
    // 通过 Irole 继承
    bool Init() override;
    UserData* ProcMsg(UserData& _poUserData) override;
    void Fini() override;

    GameProtocol* m_proto = nullptr;

    /* 姓名与Pid */
    std::string m_Name;
    int m_Pid;

private:
    GameMsg* CreateLoginNameID();
    GameMsg* CreateSrdPlayersPosition();
    GameMsg* CreateSelfPosition();
    GameMsg* CreateLogoffNameID();

    void ViewAppear(GameRole* _pRole);
    void ViewLost(GameRole* _pRole);

    /* 处理聊天 */
    // void ProcChatMsg(pb::Talk* pTalk);
    void ProcChatMsg(google::protobuf::Message* pMsg);
    void ProcNewPos(google::protobuf::Message* pMsg);

    // 通过 Player 继承
    float GetX() override;

    float GetY() override;

};

