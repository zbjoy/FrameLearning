#include "ZinxKernel.h"
#include <sys/epoll.h>
#include <iostream>
#include <error.h>

bool ZinxKernel::Init()
{
	int fd = epoll_create(1);
	if (fd == -1)
	{
		perror("epoll_create(): ");
		return false;
	}
	m_epoll_fd = fd;
}

void ZinxKernel::run()
{
	while (true)
	{
		int ready_count = -1;
		epoll_event evts[200];
		ready_count = epoll_wait(m_epoll_fd, evts, 200, -1);
		/* 被信号打断 */
		if (ready_count <= 0)
		{
			if (errno == EINTR)
			{
				continue;
			}
		}

		for (int i = 0; i < ready_count; i++)
		{
			if (0 != (evts->events & EPOLLIN))
			{
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				pChannel->ReadFd();
				pChannel->WriteFd();
			}

			if (0 != (evts->events & EPOLLOUT))
			{
				std::cout << "--------------------" << std::endl;
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				pChannel->WriteFd();
			}
		}
	}
}

void ZinxKernel::Add_Channel(Ichannel* _pChannel)
{
	epoll_event evt;
	evt.data.ptr = _pChannel;
	evt.events = EPOLLIN;
	epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, _pChannel->GetFd(), &evt);
}

void ZinxKernel::Del_Channel(Ichannel* _pChannel)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
}
