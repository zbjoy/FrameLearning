#pragma once
#include <zinx.h>

/* 不能加 "GameChannel.h" 否则会出现循环引用 */
class GameChannel;
class GameRole;
class GameProtocol :
    public Iprotocol
{
public:
    // 通过 Iprotocol 继承
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

    std::string szLast;
    GameChannel* m_channel = NULL;
    GameRole* m_role = NULL;
};

