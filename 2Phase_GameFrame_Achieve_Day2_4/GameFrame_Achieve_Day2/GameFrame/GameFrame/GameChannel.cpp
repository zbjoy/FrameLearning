#include "GameChannel.h"
#include "GameRole.h"

GameChannel::~GameChannel()
{
    if (m_proto != NULL)
    {
		ZinxKernel::Zinx_Del_Proto(*m_proto);
		delete m_proto;
    }
}

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
    pProto->m_channel = pChannel;

    GameRole* pRole = new GameRole();
    /* 将协议与业务绑定 */
    pProto->m_role = pRole;
    pRole->m_proto = pProto;

    /* 将协议对象添加到kernel中 */
    ZinxKernel::Zinx_Add_Proto(*pProto); // 第一次这样 // 感觉是为了好找
    ZinxKernel::Zinx_Add_Role(*pRole);
    /* 将pProto加入ZinxKernel中有什么意义??? */
    return pChannel;
}
