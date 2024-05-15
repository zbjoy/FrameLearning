#include "ZinxTimer.h"
#include <sys/timerfd.h>

bool ZinxTimer::Init()
{
    bool bRet = false;
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer_fd != -1)
    {
        struct itimerspec peroid {
            {1, 0}, {0, 0}
        };
        if (timerfd_settime(timer_fd, 0, &peroid, NULL) == 0)
        {
			Timer_Fd = timer_fd;
			bRet = true;
        }
    }
    return bRet;
}

bool ZinxTimer::ReadFd(std::string& _input)
{
    int iCount = 0;
    char buf[8] = { 0 };
    if (sizeof(buf) == read(Timer_Fd, buf, sizeof(buf)))
    {
        _input.assign(buf, sizeof(buf));
        return true;
    }
    return false;
}

bool ZinxTimer::WriteFd(std::string& _output)
{
    return false;
}

void ZinxTimer::Fini()
{
}

int ZinxTimer::GetFd()
{
    return Timer_Fd;
}

std::string ZinxTimer::GetChannelInfo()
{
    return "TimerFd";
}

AZinxHandler* ZinxTimer::GetInputNextStage(BytesMsg& _oInput)
{
    return nullptr;
}
