#include "CmdVerify.h"
#include "CmdMsg.h"
#include "RoleProcess.h"

CmdVerify* CmdVerify::m_instance = new CmdVerify();

UserData* CmdVerify::raw2request(std::string _szInput)
{
    CmdMsg* pRet = new CmdMsg();
    pRet->szData = _szInput;
    if (_szInput == "exit")
    {
        pRet->isCmd = true;
        pRet->mCmd = EXIT;
        pRet->szData = "";
    }
    else if (_szInput == "open")
    {
        pRet->isCmd = true;
        pRet->mCmd = OPEN;
        pRet->szData = "";
    }
    else if (_szInput == "close")
    {
        pRet->isCmd = true;
        pRet->mCmd = CLOSE;
        pRet->szData = "";
    }
    else if (_szInput == "date")
    {
        pRet->isCmd = true;
        pRet->mCmd = DATE;
        pRet->szData = "";
    }
    else if (_szInput == "closedate")
    {
        pRet->isCmd = true;
        pRet->mCmd = CLOSEDATE;
        pRet->szData = "";
    }
    return pRet;
}

std::string* CmdVerify::response2raw(UserData& _oUserData)
{
    // 不知道不 new 一个 CmdMsg 可不可以
    GET_REF2DATA(CmdMsg, input, _oUserData);
    //CmdMsg* pOut = new CmdMsg(input);
    return new std::string(input.szData);
    //return new std::string(pOut->szData);
}

Irole* CmdVerify::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    szOutInfo = _oUserDataMsg.szInfo;
    if (szOutInfo == "stdin")
    {
        szOutInfo = "stdout";
    }

    /* 把消息交给业务层去处理 */
    CmdMsg* cmdMsg = dynamic_cast<CmdMsg*>(_oUserDataMsg.poUserData);
    /* 可以尝试一下从_oUserDataMsg中把CmdMsg中的szInfo能不能赋值传给下一个对象 */
    Irole* pRet = NULL;
    if (cmdMsg->isCmd)
    {
        /* 返回给InOutCtl处理 */
		auto roleList = ZinxKernel::Zinx_GetAllRole();
		for (auto pTempRole : roleList)
		{
			auto pRole = dynamic_cast<InOutCtl*>(pTempRole);
			if (pRole)
			{
				pRet = pRole;
				break;
			}
		}
    }
    else
    {
        /* 返回给AddDatePreCtl处理 */
		auto roleList = ZinxKernel::Zinx_GetAllRole();
		for (auto pTempRole : roleList)
		{
			auto pRole = dynamic_cast<AddDatePreCtl*>(pTempRole);
			if (pRole)
			{
				pRet = pRole;
				break;
			}
		}
    }
    return pRet;
}

Ichannel* CmdVerify::GetMsgSender(BytesMsg& _oBytes)
{
    /* 给szOutInfo */
    return ZinxKernel::Zinx_GetChannel_ByInfo(szOutInfo);
}
