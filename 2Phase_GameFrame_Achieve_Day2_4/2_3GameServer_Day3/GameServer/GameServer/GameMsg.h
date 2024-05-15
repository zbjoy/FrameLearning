#pragma once
#include <zinx.h>
#include <list>
#include "msg.pb.h"

class GameMsg :
    public UserData
{
public:
    GameMsg() {};
    ~GameMsg() 
    {
        if (pMsg != nullptr)
        {
            delete pMsg;
            pMsg = nullptr;
        }
    }
    google::protobuf::Message* pMsg = nullptr;
    enum MSG_TYPE
    {
         MSG_TYPE_LOGIN_ID_NAME = 1,
         MSG_TYPE_CHAT_CONTENT = 2,
         MSG_TYPE_NEW_POITION = 3,
         MSG_TYPE_BROADCAST = 200,
         MSG_TYPE_LOGOFF_ID_NAME = 201,
         MSG_TYPE_SRD_POSITION = 202
    } enMsgType;

    /* 已知消息内容创建消息对象 */
    GameMsg(MSG_TYPE _type, google::protobuf::Message* _pMsg);
    /* 将字节流转换为消息结构 */
    GameMsg(MSG_TYPE _type, std::string strstream);

    /* 序列化消息 */
    std::string serialize();
};

class MultiMsg : public UserData
{
public:
    MultiMsg() {};
    ~MultiMsg();
    std::list<GameMsg*> m_msg_list;
};

