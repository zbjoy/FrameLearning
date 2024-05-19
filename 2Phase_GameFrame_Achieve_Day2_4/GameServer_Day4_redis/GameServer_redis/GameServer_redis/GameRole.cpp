#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include <iostream>
#include <string.h>

static AOIWorld world(0, 400, 0, 400, 20, 20);

bool GameRole::Init()
{
    m_pid = m_protocol->m_channel->GetFd();
    m_Name = std::string("Tom") + std::to_string(m_pid);

    x = 100 + 3 * m_pid;
    z = 100 + 3 * m_pid;

    world.Add_Player(this);

    /* 向客户端发送上线消息 */
    // 但是这有什么意义呢???
    ZinxKernel::Zinx_SendOut(*CreateLoginIDName(), *m_protocol);
    /* 向客户端发送周围玩家信息 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPosition(), *m_protocol);
    /* 向周围玩家发送自己位置信息 */
    auto player_list = world.GetSrdPlayerPosition(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_protocol);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{

    return nullptr;
}

void GameRole::Fini()
{
}

GameMsg* GameRole::CreateLoginIDName()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPlayerPosition()
{
    std::list<Player*>player_list = world.GetSrdPlayerPosition(this);
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();
    auto msg_players = pMsg->mutable_ps();

    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        auto msg_player = msg_players->Add();
        msg_player->set_pid(player->m_pid);
        msg_player->set_username(player->m_Name);
        
        auto msg_pos = msg_player->mutable_p();
        msg_pos->set_x(player->x);
        msg_pos->set_y(player->y);
        msg_pos->set_z(player->z);
        msg_pos->set_v(player->v);

    }
    
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);

    return pRetMsg;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_pid);
    pMsg->set_username(m_Name);
    pMsg->set_tp(2);
    auto msg_pos = pMsg->mutable_p();
    msg_pos->set_x(x);
    msg_pos->set_y(y);
    msg_pos->set_z(z);
    msg_pos->set_v(v);
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_BROADCAST, pMsg);
    return pRetMsg;
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
