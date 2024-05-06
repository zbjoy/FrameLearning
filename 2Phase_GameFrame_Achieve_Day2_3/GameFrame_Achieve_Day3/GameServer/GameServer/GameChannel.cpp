#include "GameChannel.h"
#include "GameProto.h"
#include "GameRole.h"

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    GameChannel* pChannel = new GameChannel(_fd);
    GameProto* pProto = new GameProto();

    pChannel->m_proto = pProto;
    pProto->m_channel = pChannel;

    GameRole* pRole = new GameRole();
    pProto->m_role = pRole;
    pRole->m_proto = pProto;

    return pChannel;
}
