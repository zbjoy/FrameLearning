#pragma once
#include <ZinxTCP.h>

class GameRole;
class GameChannel;
class GameProtocol :
    public Iprotocol
{
public:

    // 通过 Iprotocol 继承
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;

    GameRole* m_role = nullptr;
    GameChannel* m_channel = nullptr;

private:
    std::string szData;
};

