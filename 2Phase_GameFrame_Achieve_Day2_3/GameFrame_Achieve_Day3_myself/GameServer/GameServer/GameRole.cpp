#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"
#include "GameChannel.h"
#include "msg.pb.h"

static AOIWorld world(0, 400, 0, 400, 20, 20);

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pMsg = new pb::SyncPid();
    pMsg->set_pid(Pid);
    pMsg->set_username(szName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSrdPlayerPosition()
{
    pb::SyncPlayers* pMsg = new pb::SyncPlayers();

    auto player_list = world.GetSrdPlayerPosition(this);

    for (auto single : player_list)
    {
        // pb::SyncPlayer* pMsg = new pb::SyncPlayer();
        auto pPlayer = pMsg->add_ps();

        auto pRole = dynamic_cast<GameRole*>(single);
        pPlayer->set_pid(pRole->Pid);
        pPlayer->set_username(pRole->szName);

        auto pPostion = pPlayer->mutable_p();
        pPostion->set_x(pRole->x);
        pPostion->set_y(pRole->y);
        pPostion->set_z(pRole->z);
        pPostion->set_v(pRole->v);

    }

    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_SRD_POSITION, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSelfPosition()
{
    pb::BroadCast* pMsg = new pb::BroadCast();
    pMsg->set_pid(Pid);
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
}

bool GameRole::Init()
{
    bool bRet = false;

    Pid = m_proto->m_channel->GetFd();

    szName = szName + std::to_string(Pid);
    x = x + 10 * Pid;
    z = z + 10 * Pid;

    bRet = world.Add_Player(this);

    if (bRet == true)
    {
		ZinxKernel::Zinx_SendOut(*CreateIDNameLogin(), *m_proto);
        ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPosition(), *m_proto);
        ZinxKernel::Zinx_SendOut(*CreateSelfPosition(), *m_proto);
    }

    if (bRet == false)
    {
        std::cout << "GameRole::Init fail" << std::endl;
    }
    return bRet;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);
    for (auto single : multiMsg.m_msg_list)
    {
        /* 调试代码 */
        std::cout << "GameRole::ProcMsg() [google debug for multiMsg]" << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;

        ZinxKernel::Zinx_SendOut(*single, *m_proto);
    }

    return nullptr;
}

void GameRole::Fini()
{
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
