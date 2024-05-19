#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_protocol;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    GameChannel* pChannel = new GameChannel(_fd);
    GameProtocol* pProtocol = new GameProtocol();

    pChannel->m_protocol = pProtocol;
    pProtocol->m_channel = pChannel;

    GameRole* pRole = new GameRole();
    pRole->m_protocol = pProtocol;
    pProtocol->m_role = pRole;

    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    ZinxKernel::Zinx_Add_Role(*pRole);
    return pChannel;
}
