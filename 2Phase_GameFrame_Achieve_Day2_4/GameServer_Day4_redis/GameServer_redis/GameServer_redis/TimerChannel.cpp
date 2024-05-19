#include "TimerChannel.h"
#include <sys/timerfd.h>

TimerMng* TimerMng::m_instance = new TimerMng;

bool TimerChannel::Init()
{
    int timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerFd == -1)
    {
        perror("timerfd_create");
        return false;
    }

    itimerspec peroid{
        {1, 0}, {1, 0}
    };

    int ret = timerfd_settime(timerFd, 0, &peroid, NULL);
    if (ret == -1)
    {
        perror("timerfd_settime");
        return false;
    }

    m_timer_fd = timerFd;
    return true;
}

bool TimerChannel::ReadFd(std::string& _input)
{
    int count;
    char buf[8] = { 0 };
    if (sizeof(buf) != read(m_timer_fd, buf, sizeof(buf)))
    {
        return false;
    }

    _input.assign(buf, sizeof(buf));
    return true;
}

bool TimerChannel::WriteFd(std::string& _output)
{
    return false;
}

void TimerChannel::Fini()
{
    close(m_timer_fd);
}

int TimerChannel::GetFd()
{
    return m_timer_fd;
}

std::string TimerChannel::GetChannelInfo()
{
    return "TimerFd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return TimerMng::GetInstance();
}

IZinxMsg* TimerMng::InternelHandle(IZinxMsg& _oInput)
{
    GET_REF2DATA(BytesMsg, bytesMsg, _oInput);
    // int timerCount = atoi(bytesMsg.szData.c_str());
    // std::cout << "timerCount: " << timerCount << std::endl;

    int timerCount = 0;
    bytesMsg.szData.copy((char*)&timerCount, sizeof(timerCount), 0);
    std::cout << "timerCount: " << timerCount << std::endl;

    while (timerCount-- > 0)
    {
        iCount = (iCount + 1) % maxQueueNum;

        for (std::list<TimerProc*>::iterator itr = m_timer_wheel[iCount].begin(); itr != m_timer_wheel[iCount].end();)
        {
            if ((*itr)->iCount <= 0)
            {
                (*itr)->Proc();
                auto temp = *itr;
                itr = m_timer_wheel[iCount].erase(itr);
                Add_Task(temp);
            }
            else
            {
                (*itr)->iCount--;
                itr++;
            }
        }
    }
    return nullptr;
}

AZinxHandler* TimerMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}

void TimerMng::Add_Task(TimerProc* _task)
{
    int index = (_task->GetTimerSec() + iCount) % maxQueueNum;
    _task->iCount = _task->GetTimerSec() / maxQueueNum;
    m_timer_wheel[index].push_back(_task);
}

void TimerMng::Del_Task(TimerProc* _task)
{
    for (std::vector<std::list<TimerProc*>>::iterator itr = m_timer_wheel.begin(); itr != m_timer_wheel.end(); itr++)
    {
        for (auto single : (*itr))
        {
            if (single == _task)
            {
                (*itr).remove(single);
                return;
            }
        }
    }
}
