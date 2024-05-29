#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"

bool GameRole::Init()
{
    /* 初始化姓名和Pid */
    m_Pid = m_protocol->m_channel->GetFd();
    m_Name = std::string("Tom") + std::to_string(m_Pid);

    /* 发送上线消息 */
    ZinxKernel::Zinx_SendOut(*CreateLoginIDName(), *m_protocol); // 有什么用 ??? 

    /* 发送周围玩家位置给自己 */


    /* 发送自己位置给周围玩家 */


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
