#include "ZinxTimer.h"
#include <sys/timerfd.h>



IZinxMsg* output_hello::InternelHandle(IZinxMsg& _oInput)
{
    Ichannel* pChannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
    std::string output = "hello world\n";
    ZinxKernel::Zinx_SendOut(output, *pChannel);
    return nullptr;
}

AZinxHandler* output_hello::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}

bool ZinxTimerChannel::Init()
{
    bool ret = false;
    /* 创建一个定时器 */
    int iFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (iTimerFd >= 0)
    {
		struct itimerspec period = {
			{1, 0}, {1, 0}
		};
        if (timerfd_settime(iFd, 0, &period, NULL) == 0)
        {
            ret = true;
            iTimerFd = iFd;
        }
    }
    return ret;
}

bool ZinxTimerChannel::ReadFd(std::string& _input)
{
    bool ret = false;
    /* 读取超时的次数 */
    char buf[8] = { 0 };
    if (sizeof(buf) == read(iTimerFd, buf, sizeof(buf)))
    {
        ret = true;
        _input.assign(buf, sizeof(buf));
    }
    return ret;
}

bool ZinxTimerChannel::WriteFd(std::string& _output)
{
    return false;
}

void ZinxTimerChannel::Fini()
{
    /* 关闭定时器的文件描述符 */
    close(iTimerFd);
    iTimerFd = -1;
}

int ZinxTimerChannel::GetFd()
{
    return iTimerFd;
}

std::string ZinxTimerChannel::GetChannelInfo()
{
    return "TimerFd";
}

// output_hello* pout_hello = new output_hello();
TimerOutMng TimerOutMng::single;

AZinxHandler* ZinxTimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return &TimerOutMng::getInstance();
}


TimerOutMng::TimerOutMng()
{
    for (int i = 0; i < 10; i++)
    {
        std::list<TimerOutProc*> temp;
        m_timer_wheel.push_back(temp);
    }
}

IZinxMsg* TimerOutMng::InternelHandle(IZinxMsg& _oInput)
{
    int iTimerOutCount = 0;
    GET_REF2DATA(BytesMsg, oOutput, _oInput);
    oOutput.szData.copy((char*)&iTimerOutCount, sizeof(iTimerOutCount), 0);

    while (iTimerOutCount-- > 0)
    {
		iCount++;
		iCount = iCount % 10;
		std::list<TimerOutProc*> m_cache;
		for (auto itr = m_timer_wheel[iCount].begin(); itr != m_timer_wheel[iCount].end(); )
		{
			if ((*itr)->iCount <= 0)
			{
				m_cache.push_back(*itr);
				// (*itr)->Proc();
				auto ptmp = *itr;
				itr = m_timer_wheel[iCount].erase(itr);
				AddTask(ptmp);
			}
			else
			{
				(*itr)->iCount--;
				itr++;
			}
		}

		for (auto task : m_cache)
		{
			task->Proc();
		}
	}
	return nullptr;
}

AZinxHandler* TimerOutMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}

void TimerOutMng::AddTask(TimerOutProc* _ptask)
{
    int index = (_ptask->GetTimeSec() + iCount) % 10;
    _ptask->iCount = _ptask->GetTimeSec() / 10;
    m_timer_wheel[index].push_back(_ptask);
}

void TimerOutMng::DelTask(TimerOutProc* _ptask)
{
    for (std::vector<std::list<TimerOutProc*>>::iterator itr = m_timer_wheel.begin(); itr != m_timer_wheel.end();)
    {
        for (auto p : (*itr))
        {
            if (p == _ptask)
            {
                // itr->remove(p);
                (*itr).remove(p);
                return;
            }
        }
        itr++;
    }
}
