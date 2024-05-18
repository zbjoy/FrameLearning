#include "ZinxTimer.h"
#include <sys/timerfd.h>

TimerMng* TimerMng::m_instance = new TimerMng();

bool TimerChannel::Init()
{
	/* 创建一个时间定时器 */
	int timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
	if (timerFd == -1)
	{
		perror("TimerChannel::Init()  timerfd_create(): ");
		return false;
	}

	itimerspec peroid{
		{1, 0}, {1, 0}
	};
	
	int ret = timerfd_settime(timerFd, 0, &peroid, NULL);
	if (ret == -1)
	{
		perror("TimerChannel::Init()  timerfd_settime(): ");
		return false;
	}

	timer_fd = timerFd;

	return true;
}

bool TimerChannel::ReadFd(std::string& _input)
{
	bool bRet = false;
	char buf[8] = { 0 };
	if (sizeof(buf) == read(timer_fd, buf, sizeof(buf)))
	{
		_input.assign(buf, sizeof(buf));
		bRet = true;
	}
	return bRet;
}

bool TimerChannel::WriteFd(std::string& _output)
{
	return false;
}

void TimerChannel::Fini()
{
	close(timer_fd);
	timer_fd = -1;
}

int TimerChannel::GetFd()
{
	return timer_fd;
}

std::string TimerChannel::GetChannelInfo()
{
	return "TimerFd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return TimerMng::GetInstance();
}

bool TimerMng::Add_Task(TimerProc* _timerProc)
{
	int count = _timerProc->GetTimeSec();
	_timerProc->iCount = count / maxQueueNum;
	int index = (count + iCount) % maxQueueNum;
	m_time_wheel[index].push_back(_timerProc);
	return true;
}

bool TimerMng::Del_Task(TimerProc* _timerProc)
{
	for (auto single : m_time_wheel)
	{
		for (auto proc : single)
		{
			if (proc == _timerProc)
			{
				single.remove(proc);
				return true;
			}
		}
	}
	return false;
}

IZinxMsg* TimerMng::InternelHandle(IZinxMsg& _oInput)
{
	iCount = (iCount + 1) % maxQueueNum;
	int trigger_count = 0;
	GET_REF2DATA(BytesMsg, oOutput, _oInput);
	oOutput.szData.copy((char*)&trigger_count, sizeof(trigger_count), 0);
	
	while (trigger_count--)
	{
		std::vector<TimerProc*> tempCache;
		for (std::list<TimerProc*>::iterator itr = m_time_wheel[iCount].begin(); itr != m_time_wheel[iCount].end();)
		{
			if ((*itr)->iCount == 0)
			{
				tempCache.push_back(*itr);
				auto temp = *itr;
				itr = m_time_wheel[iCount].erase(itr);
				Add_Task(temp);
			}
			else
			{
				(*itr)->iCount--;
				itr++;
			}
		}

		for (auto single : tempCache)
		{
			single->Proc();
		}
	}

	return nullptr;
}

AZinxHandler* TimerMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
	return nullptr;
}
