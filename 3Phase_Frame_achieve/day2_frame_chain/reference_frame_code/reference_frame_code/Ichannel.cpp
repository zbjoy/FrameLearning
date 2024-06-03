#include "Ichannel.h"
#include "ZinxKernel.h"
#include <unistd.h>

void Ichannel::flushout()
{
	//char* buf = new char[m_buffer.size()];
	//m_buffer.copy(buf, m_buffer.size(), 0);
	//if (-1 != write(GetFd(), buf, m_buffer.size()))
	//{
	//	m_buffer.clear();
	//}

	//delete buf;

	if (WriteFd(m_buffer) == true)
	{
		m_buffer.clear();
	}
}

IZinxMsg* Ichannel::Internel_Handle(IZinxMsg* _inputMsg)
{
	sysIODicMsg* pMsg = dynamic_cast<sysIODicMsg*>(_inputMsg);
	if (pMsg != nullptr)
	{
		if (sysIODicMsg::IO_IN == pMsg->m_DIC)
		{
			std::string temp;
			if (ReadFd(temp) == true)
			{
				return new ByteMsg(temp, *pMsg);
			}
		}
		else
		{
			auto pOut = dynamic_cast<ByteMsg*>(_inputMsg);
			if (pOut != nullptr)
			{
				// ZinxKernel::Zinx_Sendout(pOut->content, this);
				m_buffer.append(pOut->content);
				ZinxKernel::GetInstance().Zinx_Mod_Channel(this);
			}
		}
	}
    return nullptr;
}

IZinxHandler* Ichannel::GetNext(IZinxMsg* _next_input)
{
	auto pByte = dynamic_cast<ByteMsg*>(_next_input);
	if (pByte != nullptr)
	{
		return GetInputNextStage(pByte);
	}
    return nullptr;
}

Ichannel::Ichannel()
{
}

Ichannel::~Ichannel()
{
}
