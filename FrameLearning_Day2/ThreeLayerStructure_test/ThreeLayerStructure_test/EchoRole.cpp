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

bool OutputCtl::Init()
{
    Irole* pRole = NULL;
    auto roleList = ZinxKernel::Zinx_GetAllRole();
    for (auto pDate : roleList)
    {
		auto pDateRole = dynamic_cast<DataPreCtl*>(pDate);
		if (pDateRole)
		{
            pRole = pDateRole;
            break;
        }
    }
    SetNextProcessor(*pRole);
    return true;
}

UserData* OutputCtl::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, input, _poUserData);
    if (input.isExit)
    {
        ZinxKernel::Zinx_Exit();
    }
    if (input.isOpen)
    {
        if (pOut != NULL)
			ZinxKernel::Zinx_Add_Channel(*pOut);
        pOut = NULL;
    }
    else
    {
        Ichannel* pChannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
        ZinxKernel::Zinx_Del_Channel(*pChannel);
        pOut = pChannel;
    }

    return new CmdMsg(input);
}

void OutputCtl::Fini()
{
}

bool DataPreCtl::Init()
{
    Irole* pRole = NULL;
    auto roleList = ZinxKernel::Zinx_GetAllRole();
    for (auto pEcho : roleList)
    {
        auto pEchoRole = dynamic_cast<EchoRole*>(pEcho);
        if (pEchoRole)
        {
			pRole = pEchoRole;
			break;
		}
    }
    SetNextProcessor(*pRole);
    return true;
}

UserData* DataPreCtl::ProcMsg(UserData& _poUserData)
{
	GET_REF2DATA(CmdMsg, input, _poUserData);
    char* p = NULL;
    if (input.isNeedDataPre)
    {
        time_t tmp;
        time(&tmp);
        p = ctime(&tmp);
        p[strlen(p) - 1] = 0;
		input.szUserData = string(p) + " : " + input.szUserData;
	}
    CmdMsg* pRet = new CmdMsg(input);
    /*if (input.isCmd)
    {
        return nullptr;
    }*/
    return pRet;
}

void DataPreCtl::Fini()
{
}
