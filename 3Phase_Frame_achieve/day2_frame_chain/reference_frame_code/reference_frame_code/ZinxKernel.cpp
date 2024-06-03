#include "ZinxKernel.h"

ZinxKernel ZinxKernel::zinxKernel;

void ZinxKernel::Zinx_Run()
{
	while (true)
	{
		epoll_event evts[200];
		int ready_count = epoll_wait(m_epoll_fd, evts, 200, -1);

		if (ready_count == 0)
		{
			if (EINTR == errno)
			{
				continue;
			}

		}
		for (int i = 0; i < ready_count; ++i)
		{
			if ((evts[i].events & EPOLLIN) != 0)
			{
				auto pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				sysIODicMsg dic(sysIODicMsg::IO_IN);
				pChannel->Handle(&dic);
			}

			if ((evts[i].events & EPOLLOUT) != 0)
			{
				auto pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				pChannel->flushout();
				Zinx_Res_Channel(pChannel);
			}
		}
	}
}

void ZinxKernel::Zinx_Add_Channel(Ichannel* _pChannel)
{
	if (_pChannel->Init())
	{
		struct epoll_event stEvent;
		stEvent.events = EPOLLIN;
		stEvent.data.ptr = _pChannel;

		epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, _pChannel->GetFd(), &stEvent);
	}
}

void ZinxKernel::Zinx_Del_Channel(Ichannel* _pChannel)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
	_pChannel->Fini();
}

void ZinxKernel::Zinx_Sendout(std::string _output, Ichannel* _pChannel)
{
	sysIODicMsg iodic(sysIODicMsg::IO_OUT);
	ByteMsg byte(_output, iodic);
	_pChannel->Handle(&byte);
}

void ZinxKernel::Zinx_Mod_Channel(Ichannel* _pChannel)
{
	epoll_event evt;
	evt.data.ptr = _pChannel;
	evt.events = EPOLLIN | EPOLLOUT;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
}

void ZinxKernel::Zinx_Res_Channel(Ichannel* _pChannel)
{
	epoll_event evt;
	evt.data.ptr = _pChannel;
	evt.events = EPOLLIN;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
}
