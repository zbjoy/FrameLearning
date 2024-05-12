#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"

GameChannel::~GameChannel()
{
    ZinxKernel::Zinx_Del_Proto(*m_proto);
    delete m_proto;
}

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    GameChannel* pChannel = new GameChannel(_fd);
    GameProtocol* pProto = new GameProtocol();

    pChannel->m_proto = pProto;
    pProto->m_channel = pChannel;

    GameRole* pRole = new GameRole();
    pProto->m_role = pRole;
    pRole->m_proto = pProto;

    ZinxKernel::Zinx_Add_Proto(*pProto);
    ZinxKernel::Zinx_Add_Role(*pRole);
    /* 发送给proto层 */
    return pChannel;
}
