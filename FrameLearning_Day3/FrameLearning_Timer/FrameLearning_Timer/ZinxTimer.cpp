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

output_hello* pout_hello = new output_hello();

AZinxHandler* ZinxTimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    return pout_hello;
}

