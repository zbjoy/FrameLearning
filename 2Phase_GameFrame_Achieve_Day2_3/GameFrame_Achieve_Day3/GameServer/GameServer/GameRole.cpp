#include "GameRole.h"
#include "msg.pb.h"
#include "GameMsg.h"
#include "GameProto.h"

GameMsg* GameRole::CreateSrdPlayerPosition()
{
    return nullptr;
}

bool GameRole::Init()
{
    /* 发送自己的位置 */
    // ZinxKernel::Zinx_SendOut(, *m_proto);
    /* 将附近人的位置发送给自己 */
    return true;
}

UserData* GameRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(MultiMsg, multiMsg, _poUserData);
    
    for (auto single : multiMsg.m_msgs)
    {
        ZinxKernel::Zinx_SendOut(*single, *m_proto);
    }
    return nullptr;
}

void GameRole::Fini()
{
}
