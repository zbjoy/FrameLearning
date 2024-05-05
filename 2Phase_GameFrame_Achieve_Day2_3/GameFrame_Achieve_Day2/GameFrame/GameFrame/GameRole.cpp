#include "GameRole.h"
#include "GameProtocol.h"
#include "GameMsg.h"

bool GameRole::Init()
{
    return true;
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
}
