#include "CmdVerify.h"
#include "EchoRole.h"
#include "CmdMsg.h"
#include <iostream>

CmdVerify* CmdVerify::m_instance = new CmdVerify();

CmdVerify::CmdVerify()
{
}

CmdVerify::~CmdVerify()
{
}

UserData* CmdVerify::raw2request(std::string _szInput)
{
    auto pRet = new CmdMsg();
    pRet->szUserData = _szInput;
    if (_szInput == "close")
    {
        pRet->isCmd = true;
        pRet->isOpen = false;
    }
    else if (_szInput == "open")
    {
		pRet->isCmd = true;
		pRet->isOpen = true;
	}
    else if (_szInput == "exit")
    {
        pRet->isCmd = true;
		pRet->isExit = true;
    }
    return pRet;
}

std::string* CmdVerify::response2raw(UserData& _oUserData)
{
    GET_REF2DATA(CmdMsg, output, _oUserData);
    return new std::string(output.szUserData);
}

Irole* CmdVerify::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    /* 交给业务层处理 */
    auto roleList = ZinxKernel::Zinx_GetAllRole();
    Irole* pRet = nullptr;

    CmdMsg* pMsg = dynamic_cast<CmdMsg*>(_oUserDataMsg.poUserData);

    for (auto pRole : roleList)
    {
        if (pMsg->isCmd == true)
        {
            auto pOutCtl = dynamic_cast<OutputCtl*>(pRole);
            if (pOutCtl)
            {
				pRet = pOutCtl;
				break;
			}
        }
        else
        {
			auto pEchoRole = dynamic_cast<EchoRole*>(pRole);
			if (pEchoRole)
			{
				pRet = pEchoRole;
				break;
			}
        }
    }
    return pRet;
}

Ichannel* CmdVerify::GetMsgSender(BytesMsg& _oBytes)
{
    return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
}
