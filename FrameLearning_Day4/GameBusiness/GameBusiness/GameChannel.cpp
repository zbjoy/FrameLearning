#include "GameChannel.h"

AZinxHandler* GameChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return m_proto;
}

ZinxTcpData* GameConnFact::CreateTcpDataChannel(int _fd)
{
    /* 创建TcpData */
    GameChannel* pChannel = new GameChannel(_fd);
    /* 创建一个protocol */
    GameProtocol* pProto = new GameProtocol();
    /* 将protocol与TcpData绑定 */
    pChannel->m_proto = pProto;

    /* 将协议对象添加到kernel中 */
    ZinxKernel::Zinx_Add_Proto(*pProto); // 第一次这样 // 感觉是为了好找
    /* 将pProto加入ZinxKernel中有什么意义??? */
    return pChannel;
}
