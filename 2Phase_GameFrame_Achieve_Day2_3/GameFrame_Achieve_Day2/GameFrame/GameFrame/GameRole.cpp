#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "GameChannel.h"

static AOIWorld w(0, 400, 0, 400, 20, 20);

GameMsg* GameRole::CreateIDNameLogin()
{
    pb::SyncPid* pmsg = new pb::SyncPid();
    pmsg->set_pid(iPid);
    pmsg->set_username(szName);
    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pmsg);
    return pRet;
}

GameMsg* GameRole::CreateSrdPlayerPostion()
{
    pb::SyncPlayer* pMsg = new pb::SyncPlayer();

    auto player_list = w.GetSrdPlayer(this);

    for (auto single : player_list)
    {
        // pb::SyncPlayer* pMsg = new pb::SyncPlayer();
        auto pPlayer = pMsg->add_ps();

        auto pRole = dynamic_cast<GameRole*>(single);
        pPlayer->set_pid(pRole->iPid);
        pPlayer->set_username(pRole->szName);

        auto pPostion = pPlayer->mutable_p();
        pPostion->set_x(pRole->x);
        pPostion->set_y(pRole->y);
        pPostion->set_z(pRole->z);
        pPostion->set_v(pRole->v);

    }

    GameMsg* pRet = new GameMsg(GameMsg::MSG_TYPE_SRD_POSTION, pMsg);
    return pRet;
}

GameMsg* GameRole::CreateSelfPostion()
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
}

bool GameRole::Init()
{
    bool bRet = false;
    /* 将自己添加到世界中 */
    iPid = m_proto->m_channel->GetFd();
    /* 不知道这样会不会有姓名 */
    // szName = "Tom";
    
    bRet = w.Add_Player(this);

    if (true == bRet)
    {
        ZinxKernel::Zinx_SendOut(*CreateIDNameLogin(), *m_proto);
        ZinxKernel::Zinx_SendOut(*CreateSrdPlayerPostion(), *m_proto);
        ZinxKernel::Zinx_SendOut(*CreateSelfPostion(), *m_proto);
    }
    return bRet;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, oInput, _poUserData);
    /* 测试代码 */
    std::cout << "RocMsg()" << std::endl;
    for (auto single : oInput.m_msg_list)
    {
        std::cout << "type : " << single->enMsgType << std::endl;
        std::cout << single->pMsg->Utf8DebugString() << std::endl;
        ZinxKernel::Zinx_SendOut(*single, *m_proto);

    }

    // ZinxKernel::Zinx_SendOut(_poUserData, *m_proto);
    return nullptr;
}

void GameRole::Fini()
{
    w.Del_Player(this);
}

float GameRole::GetX()
{
    return x;
}

float GameRole::GetY()
{
    return z;
}
