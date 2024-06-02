#pragma once
#include "Ichannel.h"

class ZinxKernel
{
	ZinxKernel() {
		Init();
	};
	~ZinxKernel() {};

private:
	static ZinxKernel zinxKernel;

	int m_epoll_fd = -1;

public:
	static ZinxKernel& GetInstance()
	{
		return zinxKernel;
	}

	bool Init();
	void run();

	void Add_Channel(Ichannel* _pChannel);
	void Del_Channel(Ichannel* _pChannel);
};

