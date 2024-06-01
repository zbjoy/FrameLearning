#include "TimeOutMng.h"
#include <sys/timerfd.h>

TimeOutMng* TimeOutMng::m_instance = new TimeOutMng();

bool TimerChannel::Init()
{
    std::cout << "开始创建 timerfd " << std::endl;
    int timerFd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timerFd == -1)
    {
        perror("TimerChannel::Init() timerfd_create():");
        return false;
    }

    itimerspec peroid {
        {1, 0}, {1, 0}
    };
    // peroid.it_interval.tv_sec = 1;
    // peroid.it_value.tv_sec = 1;
    int ret = timerfd_settime(timerFd, 0, &peroid, NULL);
    if (ret == -1)
    {
        perror("TimerChannel::Init() timerfd_settime():");
        return false;
    }

    std::cout << "timer_fd 创建成功" << std::endl;
	m_timer_fd = timerFd;
    return true;
}

bool TimerChannel::ReadFd(std::string& _input)
{
    //std::cout << "一次读取(ReadFd)" << std::endl;
    //int count;
    //char buf[8] = { 0 };
    ///* 必须有这个不然就狂出发timer_fd */
    //if (sizeof(buf) != read(m_timer_fd, buf, sizeof(buf)))
    //{
    //    return false;
    //}

    //_input.assign(buf, sizeof(buf));
    //std::cout << "((((((((((((((" << std::endl;
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
    return "timerfd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    // std::cout << "一次返回" << std::endl;
    return TimeOutMng::GetInstance();
}

IZinxMsg* TimeOutMng::InternelHandle(IZinxMsg& _oInput)
{
    std::cout << "(TimeOutMng::InternelHandle)" << std::endl;
    return nullptr;
}

AZinxHandler* TimeOutMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}
