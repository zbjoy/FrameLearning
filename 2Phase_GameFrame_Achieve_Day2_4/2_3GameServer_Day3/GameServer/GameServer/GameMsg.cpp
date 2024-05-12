#include "GameMsg.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg) : enMsgType(_type), pMsg(_pMsg)
{
}

GameMsg::GameMsg(MSG_TYPE _type, std::string strstream) : enMsgType(_type)
{
	switch (enMsgType)
	{
	case MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case MSG_TYPE_NEW_POITION:
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

	/* 将参数解析成消息对象 */
	pMsg->ParseFromString(strstream);
}

std::string GameMsg::serialize()
{
	std::string retStr;
	pMsg->SerializeToString(&retStr);
	return retStr;
}

MultiMsg::~MultiMsg()
{
	for (auto single : m_msg_list)
	{
		if (single != NULL)
		{
			delete single;
			single = NULL;
		}
	}
}
