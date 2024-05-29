#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"

static AOIWorld world(0, 400, 0, 400, 20, 20);

bool GameRole::Init()
{
    /* 初始化姓名和Pid */
    m_Pid = m_protocol->m_channel->GetFd();
    m_Name = std::string("Tom") + std::to_string(m_Pid);
    x = 50 + 10 * m_Pid;
    z = 50 + 10 * m_Pid;

    /* 将玩家添加到世界类 */
    world.Add_Player(this);

    /* 发送上线消息 */
    ZinxKernel::Zinx_SendOut(*CreateLoginIDName(), *m_protocol); // 有什么用 ??? 

    /* 发送周围玩家位置给自己 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPosition(), *m_protocol);
    
    /* 发送自己位置给周围玩家 */
    auto player_list = world.GetSrdPlayersPosition(this);
    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_protocol);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    /* 解析协议层传来的消息 */
    GET_REF2DATA(MultiMsg, _multiMsg, _poUserData);
    
    for (auto pGameMsg : _multiMsg.m_msg_list)
    {
        /* 处理GameMsg传来的对应的业务消息 */
    }

    return nullptr;
}

void GameRole::Fini()
{
}

GameMsg* GameRole::CreateLoginIDName()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);

    std::cout << "(玩家上线) GameMsg::CreateLoginIDName (google debug): " << std::endl;
    std::cout << pMsg->Utf8DebugString() << std::endl;

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPosition()
{
    auto player_list = world.GetSrdPlayersPosition(this);
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();
    auto msg_players = pMsg->mutable_ps();

    for (auto single : player_list)
    {
        auto player = dynamic_cast<GameRole*>(single);
        pb::Player* msg_player = msg_players->Add();
        msg_player->set_pid(player->m_Pid);
        msg_player->set_username(player->m_Name);
        pb::Position* msg_pos = msg_player->mutable_p();
        msg_pos->set_x(player->x);
        msg_pos->set_y(player->y);
        msg_pos->set_z(player->v);
        msg_pos->set_v(player->z);

        std::cout << "GameRole::CreateSrdPosition() google debug:" << std::endl;
        std::cout << msg_player->Utf8DebugString() << std::endl;
    }

    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
    return pRetMsg;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(m_Pid);
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
