#pragma once
#include <vector>
#include <zinx.h>
#include "msg.pb.h"

class GameMsg :
    public UserData
{
public:
    google::protobuf::Message* pMsg;

    enum MSG_TYPE
    {
        MSG_TYPE_LOGIN_ID_NAME = 1,
        MSG_TYPE_CHAT_CONTENT = 2,
        MSG_TYPE_NEW_POSITION = 3,
        MSG_TYPE_BROADCAST = 200,
        MSG_TYPE_LOGOFF_ID_NAME = 201,
        MSG_TYPE_SRD_POSITION = 202
    } enMsgType;

    GameMsg(GameMsg::MSG_TYPE _type, google::protobuf::Message* _pmsg);
    GameMsg(GameMsg::MSG_TYPE _type, std::string _strstream);

    std::string serialize();
};

class MultiMsg : public UserData
{
public:
    std::vector<GameMsg*> m_msg_list;
};

