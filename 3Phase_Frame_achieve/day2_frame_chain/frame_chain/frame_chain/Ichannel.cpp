#include "Ichannel.h"
#include "ZinxKernel.h"

void Ichannel::data_sendout(std::string _output)
{
	/* 为啥写这么麻烦 (感觉不如 append(_output) ) */
	m_buffer.append(_output.begin(), _output.end());
	/* 修改epoll监听类型, 添加out方向的监听 */
	ZinxKernel::GetInstance().Mod_Channel_AddOut(this);

}
