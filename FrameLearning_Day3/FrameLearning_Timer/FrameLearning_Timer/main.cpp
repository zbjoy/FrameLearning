#include <iostream>
#include <zinx.h>
#include "RoleProcess.h"
#include "IOChannel.h"
#include "ZinxTimer.h"

using namespace std;

class timerhello : public TimerOutProc
{
public:
	// 通过 TimerOutProc 继承
	void Proc() override
	{
		Ichannel* pChannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "hello world";
		ZinxKernel::Zinx_SendOut(output, *pChannel);
	}
	int GetTimeSec() override
	{
		return 3;
	}
};

class timerbye : public TimerOutProc
{
public:
	// 通过 TimerOutProc 继承
	void Proc() override
	{
		Ichannel* pChannel = ZinxKernel::Zinx_GetChannel_ByInfo("stdout");
		std::string output = "bye bye";
		ZinxKernel::Zinx_SendOut(output, *pChannel);
	}
	int GetTimeSec() override
	{
		return 5;
	}
};


int main()
{
	ZinxKernel::ZinxKernelInit();

	TimerOutMng::getInstance().AddTask(new timerhello());
	TimerOutMng::getInstance().AddTask(new timerbye());

	ZinxKernel::Zinx_Add_Channel(*new StdInChannel());
	ZinxKernel::Zinx_Add_Channel(*new StdOutChannel());
	ZinxKernel::Zinx_Add_Role(*new Echo());
	ZinxKernel::Zinx_Add_Role(*new AddDatePreCtl());
	ZinxKernel::Zinx_Add_Role(*new InOutCtl());
	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(9998, new myTcpConnFact()));
	ZinxKernel::Zinx_Add_Channel(*new ZinxTimerChannel());

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}