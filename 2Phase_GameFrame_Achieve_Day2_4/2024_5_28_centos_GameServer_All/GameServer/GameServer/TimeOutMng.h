#pragma once
#include <zinx.h>
#include <vector>
#include <list>

class TimeOutProc
{
public:
	virtual int GetTime() = 0;
	virtual void Proc() = 0;
	int iCount = -1;
};

class TimeOutMng : public AZinxHandler
{
public:
	void Add_Task(TimeOutProc* _task);
	void Del_Task(TimeOutProc* _task);

	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override;
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override;

	static TimeOutMng* GetInstance()
	{
		return m_instance;
	}

private:
	TimeOutMng() {
		for (int i = 0; i < 10; i++)
		{
			std::list<TimeOutProc*> temp;
			m_timer_wheel.push_back(temp);
		}
	};
	~TimeOutMng() {};


private:
	std::vector<std::list<TimeOutProc*>> m_timer_wheel;
	static TimeOutMng* m_instance;
	int curIndex = 0;
};

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
	int m_timer_fd;
};
