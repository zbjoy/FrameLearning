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

    char buf[8] = { 0 };
    /* 不写read的话它就会一直触发, 只有有read才会阻塞住 */
    if (8 != read(m_timer_fd, buf, 8))
    {
        std::cout << "TimerChannel::ReadFd(): read != 8" << std::endl;
        return false;
    }
    // std::cout << "过去了1秒" << std::endl;

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
    return "timerfd";
}

AZinxHandler* TimerChannel::GetInputNextStage(BytesMsg& _oInput)
{
    // std::cout << "一次返回" << std::endl;
    return TimeOutMng::GetInstance();
}

void TimeOutMng::Add_Task(TimeOutProc* _task)
{
    int timeNum = (_task->GetTime() + curIndex) % 10;
    _task->iCount = _task->GetTime() / 10;

    m_timer_wheel[timeNum].push_back(_task);
}

void TimeOutMng::Del_Task(TimeOutProc* _task)
{
    /* 删除时已经不知道跑到哪里去了, 只能直接遍历 */
    for (auto single : m_timer_wheel)
    {
        for (auto task : single)
        {
            if (task == _task)
            {
                single.remove(task);
            }
        }
    }
}

IZinxMsg* TimeOutMng::InternelHandle(IZinxMsg& _oInput)
{
    // std::cout << "(TimeOutMng::InternelHandle)" << std::endl;
    /* 处理传来的时间信号 */
    GET_REF2DATA(BytesMsg, _num, _oInput);
    int num = atoi(_num.szData.c_str());

    int num2 = 0;
    _num.szData.copy((char*)&num2, sizeof(int), 0);
    // std::cout << "atoi(_num.szData.c_str()): " << num << std::endl;
    // std::cout << "copy from char: " << num2 << std::endl;;
    // static int testNum = 0;
    // std::cout << "****************(nowTime: " << testNum++ << ")****************" << std::endl;

    // num2是对的

	while (num2-- > 0)
    {
        // for (auto single : m_timer_wheel[curIndex])
        for (auto it = m_timer_wheel[curIndex].begin(); it != m_timer_wheel[curIndex].end();)
        {
            if ((*it)->iCount == 0)
            {
                (*it)->Proc();
                /* 不可以这么玩, 这会导致遍历不下去下面的迭代器 */
                // Del_Task(single);
                std::list<TimeOutProc*>::iterator temp = it;
                it = m_timer_wheel[curIndex].erase(it);
                // Add_Task(*it);

                /* 因为it会变为下一个, 所以要提前看住 */
                Add_Task(*temp);
            }
            else
            {
                (*it)->iCount--;
                it++;
            }
        }
        curIndex = (curIndex + 1) % 10;
    }

    return nullptr;
}

AZinxHandler* TimeOutMng::GetNextHandler(IZinxMsg& _oNextMsg)
{
    return nullptr;
}
