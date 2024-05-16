#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"

static AOIWorld world(0, 400, 0, 400, 20, 20);

GameMsg* GameRole::CreateLoginNameID()
{
    auto pMsg = new pb::SyncPid();
    pMsg->set_pid(m_Pid);
    pMsg->set_username(m_Name);
    GameMsg* pRetMsg = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    std::cout << "CreateLoginNameID(): google debug information:" << std::endl;
    std::cout << pMsg->Utf8DebugString();
    return pRetMsg;
}

GameMsg* GameRole::CreateSrdPlayersPosition()
{
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    std::list<Player*> playerList = world.GetSrdPlayersPosition(this);

    for (auto single : playerList)
    {
		auto player = pMsg->add_ps();
        GameRole* tempRole = dynamic_cast<GameRole*>(single);
        player->set_pid(tempRole->m_Pid);
        player->set_username(tempRole->m_Name);
        auto pos = player->mutable_p();

        /* 为什么这里又没有y和z互换 */
        pos->set_x(tempRole->x);
        pos->set_y(tempRole->y);
        pos->set_z(tempRole->z);
        pos->set_v(tempRole->v);

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
    auto pos = pMsg->mutable_p();
    pos->set_x(x);
    pos->set_y(y);
    pos->set_z(z);
    pos->set_v(v);

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

bool GameRole::Init()
{
    m_Pid = m_proto->m_channel->GetFd();
    // m_Name = "Tom" + m_Pid 是错的, 但不知道为什么
    m_Name = std::string("Tom") + std::to_string(m_Pid);

    /* 初始化位置 */
    x = 100 + 10 * m_Pid;
    z = 100 + 10 * m_Pid;

    world.Add_Player(this);

    /* 发送登录时的姓名和Pid给客户端 */
    ZinxKernel::Zinx_SendOut(*CreateLoginNameID(), *m_proto);
    
    /* 发送周围玩家的上线信息给客户端 */
    ZinxKernel::Zinx_SendOut(*CreateSrdPlayersPosition(), *m_proto);

    /* 把自己上线的消息发给周围玩家 */
    std::list<Player*> playerList = world.GetSrdPlayersPosition(this);
    for (auto single : playerList)
    {
        auto player = dynamic_cast<GameRole*>(single);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *player->m_proto);
    }

    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GameMsg* pRetMsg = nullptr;
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);

    return NULL;
}

void GameRole::Fini()
{
}
