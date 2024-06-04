#include "zinx.h"

using namespace std;

void AZinxHandler::Handle(IZinxMsg& _oInput)
{
	IZinxMsg* poNextMsg = nullptr;
	AZinxHandler* poNextHandler = nullptr;

	poNextMsg = InternelHandle(_oInput);

	if (nullptr != poNextMsg)
	{
		poNextHandler = GetNextHandler(*poNextMsg);
		if (nullptr != poNextHandler)
		{
			poNextHandler->Handle(*poNextMsg);
		}
		delete poNextMsg;
	}
	return;
}

IZinxMsg* Ichannel::InternelHandle(IZinxMsg& _oInput)
{
	IZinxMsg* poRet = nullptr;
	GET_REF2DATA(SysIOReadyMsg, oIoStat, _oInput);
	if (oIoStat.m_emIODic == SysIOReadyMsg::IN)
	{
		BytesMsg* poBytes = new BytesMsg(oIoStat);
		if (true == ReadFd(poBytes->szData))
		{
			poBytes->szInfo = GetChannelInfo();
			poRet = poBytes;
		}
		else
		{
			delete poBytes;
		}
	}
	else if (oIoStat.m_emIODic == SysIOReadyMsg::OUT)
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		if (false == HasOutput())
		{
			ZinxKernel::Zinx_SetChannelOut(*this);
		}
		// 加入发送缓冲区
		m_WriteBuffer.push_back(oBytes.szData);
	}

	return poRet;
}

void Ichannel::FlushOut()
{
	while (false == m_WriteBuffer.empty())
	{
		if (true == WriteFd(m_WriteBuffer.front()))
		{
			m_WriteBuffer.pop_front();
		}
		else
		{
			break;
		}
	}
}

/* ----------------------- *

/* ----------------------- */

AZinxHandler* Ichannel::GetNextHandler(IZinxMsg& _oNextMsg)
{
	AZinxHandler* poRet = NULL;
	GET_REF2DATA(SysIOReadyMsg, oIoStat, _oNextMsg);
	if (oIoStat.m_emIODic == SysIOReadyMsg::IN)
	{
		GET_REF2DATA(BytesMsg, oBytes, _oNextMsg);
		poRet = GetInputNextStage(oBytes);
	}

	return poRet;
}


std::string Ichannel::Conver2Printable(std::string& _szData)
{
	char* pcTemp = (char*)calloc(1UL, _szData.size() * 3 + 1);
	int iCurPos = 0;

	if (nullptr != pcTemp)
	{
		for (int i = 0; i < _szData.size(); ++i)
		{
			iCurPos += sprintf(pcTemp + iCurPos, "%02X ", (unsigned char)_szData[i]);
		}
		pcTemp[iCurPos] = 0;
	}

	std::string szRet = std::string(pcTemp);
	free(pcTemp);

	return szRet;
}
