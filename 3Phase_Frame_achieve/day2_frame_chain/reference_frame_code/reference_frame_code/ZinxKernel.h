#pragma once
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include "Ichannel.h"

class ZinxKernel
{
public:
	void Zinx_Run();
	void Zinx_Add_Channel(Ichannel* _pChannel);
	void Zinx_Del_Channel(Ichannel* _pChannel);
	void Zinx_Mod_Channel(Ichannel* _pChannel);

	static void Zinx_Sendout(std::string _output, Ichannel* _pChannel);

	static ZinxKernel& GetInstance()
	{
		return zinxKernel;
	}

private:
	ZinxKernel() {
		int fd = epoll_create(1);
		if (fd >= 0)
		{
			m_epoll_fd = fd;
		}
	}
	~ZinxKernel() {
		if (m_epoll_fd != -1)
		{
			close(m_epoll_fd);
		}
	}

	void Zinx_Res_Channel(Ichannel* _pChannel);

private:
	static ZinxKernel zinxKernel;

	int m_epoll_fd;
};

