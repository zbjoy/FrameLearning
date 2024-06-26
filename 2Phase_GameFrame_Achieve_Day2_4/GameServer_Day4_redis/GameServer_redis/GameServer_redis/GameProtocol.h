#pragma once
#include <zinx.h>

class GameRole;
class GameChannel;
class GameProtocol :
    public Iprotocol
{
public:
    GameProtocol() {};
    ~GameProtocol();
    // 通过 Iprotocol 继承
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

    GameChannel* m_channel;
    GameRole* m_role;

private:
    std::string szLast;
};

