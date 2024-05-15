#include "GameMsg.h"

GameMsg::GameMsg(MSG_TYPE _type, std::string _strstream) : enMsgType(_type)
{
	switch (_type)
	{
	case MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case MSG_TYPE_CHAT_CONTENT:
		/* 为什么不是弄成BoardCast */
		// 因为一般用这个构造的是接收client的消息
		pMsg = new pb::Talk();
		break;
	case MSG_TYPE_NEW_POITION:
		/* 为什么不是弄成BoardCast */
		pMsg = new pb::Position();
		break;
	case MSG_TYPE_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case MSG_TYPE_LOGOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case MSG_TYPE_SRD_POSITION:
		pMsg = new pb::SyncPlayers();
		break;
	default:
		break;
	}

	/* 将参数对象解析成消息对象 */
	pMsg->ParseFromString(_strstream);

}

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg) : enMsgType(_type), pMsg(_pMsg)
{
}

std::string GameMsg::serialize()
{
	std::string retStr;
	pMsg->SerializeToString(&retStr);
	return retStr;
}
