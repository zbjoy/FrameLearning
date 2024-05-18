#include <zinx.h>
#include <vector>
#include <list>

class TimerChannel : public Ichannel
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
	int timer_fd;
};

class TimerProc
{
public:

	virtual void Proc() = 0;
	virtual int GetTimeSec() = 0;

	int iCount = -1;
};

class TimerMng : public AZinxHandler
{
public:
	
	static TimerMng* GetInstance()
	{
		return m_instance;
	}

	bool Add_Task(TimerProc* _timerProc);
	bool Del_Task(TimerProc* _timerProc);

private:
	static TimerMng* m_instance;
	TimerMng() 
	{
		iCount = 0;
		maxQueueNum = 10;

		for (int i = 0; i < maxQueueNum; i++)
		{
			std::list<TimerProc*> temp;
			m_time_wheel.push_back(temp);
		}
	};
	~TimerMng() {};

private:
	int iCount;
	int maxQueueNum;

	std::vector<std::list<TimerProc*>> m_time_wheel;

	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;
};