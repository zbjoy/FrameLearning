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

GameRole::GameRole()
{
    szName = "Tom";
}

bool GameRole::Init()
{
    bool bRet = false;
    /* 将自己添加到世界中 */
    iPid = m_proto->m_channel->GetFd();
    
    bRet = w.Add_Player(this);

    if (true == bRet)
    {
        ZinxKernel::Zinx_SendOut(*CreateIDNameLogin(), *m_proto);
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
