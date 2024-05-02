#pragma once
#include <zinx.h>
#include <iostream>

using namespace std;

class CmdVerify :
    public Iprotocol
{
    CmdVerify();
    // ~CmdVerify() = delete;
    ~CmdVerify();
    static CmdVerify* m_instance;

public:
    // 通过 Iprotocol 继承
    UserData* raw2request(std::string _szInput) override;
    std::string* response2raw(UserData& _oUserData) override;
    Irole* GetMsgProcessor(UserDataMsg& _oUserDataMsg) override;
    Ichannel* GetMsgSender(BytesMsg& _oBytes) override;
    static CmdVerify* getInstance()
    {
		return m_instance;
	}

    string szOutChannel;
    bool isNeedDataPre = false;
};

