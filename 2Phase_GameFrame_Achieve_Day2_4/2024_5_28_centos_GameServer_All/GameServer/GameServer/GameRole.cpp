#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"

bool GameRole::Init()
{

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
