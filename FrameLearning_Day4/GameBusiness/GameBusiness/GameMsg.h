#pragma once
#include <zinx.h>
#include <google/protobuf/message.h>

class GameMsg :
    public UserData
{
public:
    google::protobuf::Message* pMsg = NULL;
    enum MSG_TYPE
    {
        MSG_TYPE_LOGIN_ID_NAME = 1,
        MSG_TYPE_CHAT_CONTENT = 2,
        MSG_TYPE_NEW_POSTION = 3,
        MSG_TYPE_BROADCAST = 200,
        MSG_TYPE_LOGOFF_ID_NAME = 201,
        MSG_TYPE_SRD_POSTION = 202
    } enMsgType;

    GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg);
    GameMsg(MSG_TYPE _type, std::string _strstream);

    std::string serialize();
};

