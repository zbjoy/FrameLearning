#include "GameMsg.h"

GameMsg::GameMsg(GameMsg::MSG_TYPE _type, google::protobuf::Message* _pmsg) : enMsgType(_type), pMsg(_pmsg)
{
}

GameMsg::GameMsg(GameMsg::MSG_TYPE _type, std::string _strstream) : enMsgType(_type)
{
	switch (enMsgType)
	{
	case MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case MSG_TYPE_NEW_POSITION:
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

	pMsg->ParseFromString(_strstream);
}

std::string GameMsg::serialize()
{
	std::string temp;
	pMsg->SerializeToString(&temp);
	return temp;
}

