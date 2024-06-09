#include "ZinxKernel.h"
#include "Ichannel.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>

int ZinxKernel::m_epoll_fd = -1;
bool ZinxKernel::m_exit_status = false;

void ZinxKernel::Zinx_Run()
{
	while (!m_exit_status)
	{
		epoll_event evts[200] = { 0 };
		int ready_count = epoll_wait(m_epoll_fd, evts, 200, -1);
		if (ready_count <= 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				// 被信号打断了
				continue;
			}
			else
			{
				exit(1);
			}
		}

		for (int i = 0; i < ready_count; i++)
		{
			if ((evts[i].events & EPOLLIN) == true)
			{
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				if (pChannel != nullptr)
				{
					std::string temp;
					pChannel->ReadFd(temp);

					/* --------------------------------------------- */
					// pChannel->m_buffer = temp;
					/* --------------------------------------------- */
				}

			}

			if ((evts[i].events & EPOLLOUT) == true)
			{
				Ichannel* pChannel = static_cast<Ichannel*>(evts[i].data.ptr);
				if (pChannel != nullptr)
				{
					/* --------------------------------------------- */
					// pChannel->WriteFd();
					/* --------------------------------------------- */

					Zinx_Set_Restoration(pChannel);
				}
			}
		}
	}
}

bool ZinxKernel::Zinx_Add_Channel(Ichannel* _pChannel)
{
	if (_pChannel == nullptr)
	{
		return false;
	}
	bool bRet = false;
	if (_pChannel->Init())
	{
		/* 为什么可以用栈区的,不会被释放掉吗, 还是说epoll_ctl的第四个参数在函数调用时重新拷贝了一个??? */
		epoll_event evt;
		evt.data.ptr = _pChannel;
		evt.events = EPOLLIN;
		int ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, _pChannel->GetFd(), &evt);
		if (ret != -1)
		{
			bRet = true;
		}
	}
	return bRet;
}

void ZinxKernel::Zinx_Del_Channel(Ichannel* _pChannel)
{
	if (_pChannel == nullptr)
	{
		return;
	}

	epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, _pChannel->GetFd(), NULL);
}

bool ZinxKernel::ZinxKernel_Init()
{
	int fd = epoll_create(1);
	if (fd == -1)
	{
		/* 错误处理 */

		// to do ...
	}

	m_epoll_fd = fd;

}

void ZinxKernel::ZinxKernel_Fini()
{
	if (m_epoll_fd != -1)
	{
		close(m_epoll_fd);
		m_epoll_fd = -1;
	}
}

bool ZinxKernel::Zinx_Set_OutChannel(Ichannel* _pChannel)
{
	if (_pChannel == nullptr)
	{
		return false;
	}

	epoll_event evt;
	evt.data.ptr = _pChannel;
	evt.events = EPOLLIN | EPOLLOUT;
	int ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
	if (ret == -1)
	{
		return false;
	}
	return true;
}

bool ZinxKernel::Zinx_Set_Restoration(Ichannel* _pChannel)
{
	if (_pChannel == nullptr)
	{
		return false;
	}

	epoll_event evt;
	evt.data.ptr = _pChannel;
	evt.events = EPOLLIN;
	int ret = epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, _pChannel->GetFd(), &evt);
	if (ret == -1)
	{
		return false;
	}
	return true;
}

void ZinxKernel::ZinxKernel_Exit()
{
	m_exit_status = true;
}

