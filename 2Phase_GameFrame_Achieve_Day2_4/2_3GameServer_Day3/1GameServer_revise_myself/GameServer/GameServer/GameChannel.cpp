#include "GameChannel.h"
#include "GameProtocol.h"
#include "GameRole.h"

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    GameChannel* pChannel = new GameChannel(_fd);
    GameProtocol* pProtocol = new GameProtocol();

    /* 给pChannel的m_proto初始化 */
    pChannel->m_proto = pProtocol;
    pProtocol->m_channel = pChannel;

    GameRole* pRole = new GameRole();
    pRole->m_proto = pProtocol;
    pProtocol->m_role = pRole;

    ZinxKernel::Zinx_Add_Proto(*pProtocol);
    ZinxKernel::Zinx_Add_Role(*pRole);

    return pChannel;
}

GameChannel::~GameChannel()
{
    if (m_proto != nullptr)
    {
		ZinxKernel::Zinx_Del_Proto(*m_proto);
		delete m_proto;
		m_proto = nullptr;
    }
}
