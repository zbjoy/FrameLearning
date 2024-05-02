#include "EchoRole.h"
#include "CmdMsg.h"
#include "CmdVerify.h"
#include "StdInOutChannel.h"

bool EchoRole::Init()
{
    return true;
}

UserData* EchoRole::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, input, _poUserData);
    // 第一次写成了:  CmdMsg* output = new CmdMsg();  但是这样会导致输出为\n, 所以不能太依赖Github的Copilot
    CmdMsg* output = new CmdMsg(input);
    ZinxKernel::Zinx_SendOut(*output, *(CmdVerify::getInstance()));
    return nullptr;
}

void EchoRole::Fini()
{
}
