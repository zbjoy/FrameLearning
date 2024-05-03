#include "RoleProcess.h"
#include "CmdMsg.h"
#include "CmdVerify.h"

bool InOutCtl::Init()
{
    /* 处理完后给AddDatePreCtl类 */
    Irole* pRet = NULL;
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
    SetNextProcessor(*pRet);
    return true;
}

UserData* InOutCtl::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, _input, _poUserData);
    switch (_input.mCmd)
    {
    case OPEN:
        if (isOpen == false && pOutChannel != NULL)
        {
            ZinxKernel::Zinx_Add_Channel(*pOutChannel);
            pOutChannel = NULL;
        }
        break;
    case CLOSE:
        if (isOpen == true)
        {
            isOpen = false;
            /* 暂时写为stdout, 之后应该要修改为CmdMsg中对应的szInfo */
            pOutChannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
            ZinxKernel::Zinx_Del_Channel(*pOutChannel);
        }
        break;
    case EXIT:
        ZinxKernel::Zinx_Exit();
    }
    return new CmdMsg(_input);
}

void InOutCtl::Fini()
{
}

bool AddDatePreCtl::Init()
{
    /* 交给回显类的对象 : Echo */
    Irole* pRet = NULL;
    auto roleList = ZinxKernel::Zinx_GetAllRole();
    for (auto pTempRole : roleList)
    {
        auto pRole = dynamic_cast<Echo*>(pTempRole);
        if (pRole)
        {
            pRet = pRole;
            break;
        }
    }
    SetNextProcessor(*pRet);
    return true;
}

UserData* AddDatePreCtl::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, _input, _poUserData);

    if (_input.isCmd && _input.mCmd == DATE)
    {
        isNeedDatePre = true;
    }
    else if (_input.isCmd == true && _input.mCmd == CLOSEDATE)
    {
        isNeedDatePre = false;
    }

    if (_input.isCmd == false && isNeedDatePre)
    {
        time_t tmp;
        time(&tmp);
        char* p = ctime(&tmp);
        p[strlen(p) - 1] = 0;
        _input.szData = std::string(p) + " : " + _input.szData;
    }
    return new CmdMsg(_input);
}

void AddDatePreCtl::Fini()
{
}

bool Echo::Init()
{
    return true;
}

UserData* Echo::ProcMsg(UserData& _poUserData)
{
    GET_REF2DATA(CmdMsg, _output, _poUserData);
    ZinxKernel::Zinx_SendOut(*new CmdMsg(_output), *CmdVerify::getInstance());
    return nullptr;
}

void Echo::Fini()
{
}

AZinxHandler* myTcpData::GetInputNextStage(BytesMsg& _oInput)
{
    return CmdVerify::getInstance();
}

ZinxTcpData* myTcpConnFact::CreateTcpDataChannel(int _fd)
{
    return new myTcpData(_fd);
}
