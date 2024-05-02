#include "CmdVerify.h"
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
    return roleList.front();
}

Ichannel* CmdVerify::GetMsgSender(BytesMsg& _oBytes)
{
    return ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
}
