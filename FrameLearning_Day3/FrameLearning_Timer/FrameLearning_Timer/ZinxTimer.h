#pragma once
#include <zinx.h>


class ZinxTimerChannel : public Ichannel
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

    int iTimerFd;
};


class output_hello :
    public AZinxHandler
{
public:

    // 通过 AZinxHandler 继承
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
}; // *pout_hello = new output_hello();

class TimerOutProc
{
public:
    virtual void Proc() = 0;
    virtual int GetTimeSec() = 0;

    int iCount = -1;
};

class TimerOutMng : public AZinxHandler
{
    std::list<TimerOutProc*> m_task_list;

    static TimerOutMng single;
public:

    // 通过 AZinxHandler 继承
    IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
    AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

    void AddTask(TimerOutProc* _ptask);
    void DelTask(TimerOutProc* _ptask);

    static TimerOutMng& getInstance()
    {
        return single;
    }
};

