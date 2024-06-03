#pragma once
#include <list>
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
	std::list<Ichannel*> m_channel_list;

public:
	static ZinxKernel& GetInstance()
	{
		return zinxKernel;
	}

	bool Init();
	void run();

	void Add_Channel(Ichannel* _pChannel);
	void Del_Channel(Ichannel* _pChannel);
	void Mod_Channel_AddOut(Ichannel* _pChannel);
	void Mod_Channel_DelOut(Ichannel* _pChannel);
};

