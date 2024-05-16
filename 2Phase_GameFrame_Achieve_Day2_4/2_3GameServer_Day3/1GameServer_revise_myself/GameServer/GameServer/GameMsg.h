#pragma once
#include <zinx.h>
#include <list>
#include "msg.pb.h"

class GameMsg :
    public UserData
{
public:
    google::protobuf::Message* pMsg = nullptr;

    enum MSG_TYPE
    {
        MSG_TYPE_LOGIN_ID_NAME = 1, // 一般为server2client
        MSG_TYPE_CHAT_CONTENT = 2, // 一般为client2server
        MSG_TYPE_NEW_POITION = 3,
        MSG_TYPE_BROADCAST = 200, // 一般为server2client
        MSG_TYPE_LOGOFF_ID_NAME = 201, // 一般为server2client
        MSG_TYPE_SRD_POSITION = 202 // 一般为client2server, 在玩家上线时服务器会向玩家发送周围玩家的位置, 使用这个类型
    } enMsgType;

    GameMsg(MSG_TYPE _type, std::string _strstream);

    GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg);

    /* 序列化消息 */
    std::string serialize();
};

class MultiMsg : public UserData
{
public:
    std::list<GameMsg*> m_msg_list;
};

