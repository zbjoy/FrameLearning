#pragma once
#include <zinx.h>

class GameChannel;
class GameRole;
class GameProto :
    public Iprotocol
{
    std::string szLast;
public:
    // 通过 Iprotocol 继承
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

    GameChannel* m_channel = NULL;
    GameRole* m_role = NULL;
};

