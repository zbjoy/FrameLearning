#include "GameMsg.h"
#include "msg.pb.h"

GameMsg::GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg) : enMsgType(_type), pMsg(_pMsg)
{
}

GameMsg::GameMsg(MSG_TYPE _type, std::string _strstream)
{
	/* 通过简单工厂构造消息对象 */
	switch (_type)
	{
	case GameMsg::MSG_TYPE_LOGIN_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_CHAT_CONTENT:
		pMsg = new pb::Talk();
		break;
	case GameMsg::MSG_TYPE_NEW_POSTION:
		pMsg = new pb::Position();
		break;
	case GameMsg::MSG_TYPE_BROADCAST:
		pMsg = new pb::BroadCast();
		break;
	case GameMsg::MSG_TYPE_LOGOFF_ID_NAME:
		pMsg = new pb::SyncPid();
		break;
	case GameMsg::MSG_TYPE_SRD_POSTION:
		pMsg = new pb::SyncPlayer();
		break;
	default:
		break;
	}

	/* 将参数内容解析成strstream对象 */
	pMsg->ParseFromString(_strstream);
}

/* 序列化消息 */
std::string GameMsg::serialize()
{
	std::string ret;
	pMsg->SerializeToString(&ret);
	return ret;
}
