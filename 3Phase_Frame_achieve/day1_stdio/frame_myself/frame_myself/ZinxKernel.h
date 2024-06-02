#pragma once
#include "Ichannel.h"

class ZinxKernel
{
public:
	static ZinxKernel& GetInstance()
	{
		return zinxKernel;
	}

	void Zinx_Add_Channel(Ichannel* _pChannel);
	void Zinx_Del_Channel(Ichannel* _pChannel);

	/* 用来解决Ichannl中用不了m_epoll_fd的问题 */
	void Zinx_SendOut(Ichannel* _pChannel)
	{
		epoll_event evt;
		evt.data.ptr = _pChannel;
		evt.events = EPOLLIN | EPOLLOUT;
		epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
	}

	void Zinx_DelMod(Ichannel* _pChannel)
	{
		epoll_event evt;
		evt.data.ptr = _pChannel;
		evt.events = EPOLLIN;
		epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
	}

	void ZinxKernel_Run();

private:
	ZinxKernel() {
		Init();
	};
	~ZinxKernel() {};

	bool Init();
	
private:
	static ZinxKernel zinxKernel;

	int m_epoll_fd = -1;
};

