#include "Ichannel.h"
#include "ZinxKernel.h"

void Ichannel::data_sendout(std::string _output)
{
	m_buffer.append(_output);
	ZinxKernel::GetInstance().Zinx_SendOut(this);
}
