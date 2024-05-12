#include "GameRole.h"
#include "msg.pb.h"
#include "GameMsg.h"
#include "GameProto.h"
#include "GameChannel.h"
#include <iostream>

static AOIWorld world(0, 400, 0, 400, 20, 20);
GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(iPid);
    pMsg->set_username(szName);
    std::cout << "添加一个游戏玩家: " << std::endl;
    std::cout << "iPid: " << iPid << std::endl;
    std::cout << "username: " << szName << std::endl;
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRet;
}
GameMsg* GameRole::CreateSrdPlayerPosition()
{
    std::list<Player*>player_list = world.GetSrdPlayer(this);
    pb::SyncPlayer* pMsg = new pb::SyncPlayer();
    
    for (auto single : player_list)
    {
        GameRole* pGameRole = dynamic_cast<GameRole*>(single);
        // auto tmp = pMsg->add_players();
        auto tmp = pMsg->add_ps();
        tmp->set_pid(pGameRole->iPid);
        tmp->set_username(pGameRole->szName);
        auto p = tmp->mutable_p();
        p->set_x(pGameRole->x);
        p->set_y(pGameRole->y);
        p->set_z(pGameRole->z);
        p->set_v(pGameRole->v);
    }
    GameMsg* pRet = new GameMsg((GameMsg::MSG_TYPE_SRD_POSITION), pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(iPid);
    pMsg->set_username(szName);
    pMsg->set_tp(2);

    pb::Position* pPostion = pMsg->mutable_p();
    pPostion->set_x(x);
    pPostion->set_y(y);
    pPostion->set_z(z);
    pPostion->set_v(v);

    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRet;
}

GameRole::GameRole()
{
    szName = "Tom";
    x = 100;
    z = 100;
    std::cout << "姓名" << szName << std::endl;
}

bool GameRole::Init()
{
    iPid = m_proto->m_channel->GetFd();
    // szName = std::string("Tom") + std::to_string(iPid);
    // x = x + 10 * iPid;
    // z = z + 10 * iPid;

    world.Add_Player(this);

    /* 发送自己的位置 */
    ZinxKernel::Zinx_SendOut(*CreateIDNameLogin(), *m_proto);
    ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPosition(), *m_proto);
    ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *m_proto);
    /* 将附近人的位置发送给自己 */
    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);
    
    //for (auto single : multiMsg.m_msgs)
    //{
    //    ZinxKernel::Zinx_SendOut(*single, *m_proto);
    //}

    for (auto single : multiMsg.m_msgs)
    {
        std::cout << "type: " << single->enMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;
        ZinxKernel::Zinx_SendOut(*single, *m_proto);
    }
    return nullptr;
}

void GameRole::Fini()
{
    world.Del_Player(this);
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
