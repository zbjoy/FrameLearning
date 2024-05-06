#include "GameChannel.h"
#include "GameProto.h"

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

    return pChannel;
}
