#include "ZinxKernel.h"
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <cerrno>
// #include <error.h>
#include <sys/epoll.h>

void ZinxKernel::Zinx_Add_Channel(Ichannel* _pChannel)
{
	if (_pChannel->Init())
	{
		epoll_event evt;
		evt.data.ptr = _pChannel;
		evt.events = EPOLLIN;
		epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, _pChannel->GetFd(), &evt);
	}
}

void ZinxKernel::Zinx_Del_Channel(Ichannel* _pChannel)
{
	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
	_pChannel->Fini();
}

void ZinxKernel::ZinxKernel_Run()
{
	while (true)
	{
		epoll_event evts[200];

		int ready_count = epoll_wait(m_epoll_fd, evts, 200, -1);
		if (ready_count <= 0)
		{
			// epoll_wait可能被信号打断
			if (errno == EINTR)
			{
				continue;
			}
		}

		for (int i = 0; i < ready_count; ++i)
		{
			if (evts[i].events & EPOLLIN)
			{
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				std::string temp;
				temp = pChannel->ReadFd(temp);
				pChannel->data_process(temp);

				/* 暂时在这里输出 */
				// std::cout << temp << std::endl;
			}

			if (evts[i].events & EPOLLOUT)
			{
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				// pChannel->WriteFd();
				pChannel->flush_buffer();
				ZinxKernel::GetInstance().Zinx_DelMod(pChannel);
			}
		}

	}
}

bool ZinxKernel::Init()
{
	m_epoll_fd = epoll_create(1);
	if (m_epoll_fd == -1)
	{
		perror("epoll_create(): ");
		return false;
	}
}
