#pragma once
#include <zinx.h>
#include <vector>
#include <list>

class TimerChannel :
    public Ichannel
{
public:
    // 通过 Ichannel 继承
    bool Init() override;
    bool ReadFd(std::string& _input) override;
    bool WriteFd(std::string& _output) override;
    void Fini() override;
    int GetFd() override;
    std::string GetChannelInfo() override;
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

private:
    int m_timer_fd;
};

class TimerProc
{
public:
    virtual void Proc() = 0;
    virtual int GetTimerSec() = 0;

    int iCount = -1;
};

class TimerMng : public AZinxHandler
{
public:
    // 通过 AZinxHandler 继承
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

    void Add_Task(TimerProc* _task);
    void Del_Task(TimerProc* _task);

    static TimerMng* GetInstance()
    {
        return m_instance;
    }

private:
    TimerMng(int _maxQueueNum = 10) : maxQueueNum(_maxQueueNum)
    {
        for (int i = 0; i < maxQueueNum; i++)
        {
			std::list<TimerProc*> temp;
			m_timer_wheel.push_back(temp);
        }
    }
    ~TimerMng() {};

    int maxQueueNum;
    int iCount = 0;
    static TimerMng* m_instance;
    std::vector<std::list<TimerProc*>> m_timer_wheel;
};

