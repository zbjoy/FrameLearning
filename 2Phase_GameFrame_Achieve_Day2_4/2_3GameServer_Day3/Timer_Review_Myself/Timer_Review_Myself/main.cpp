#include <iostream>
#include <zinx.h>
#include "ZinxTimer.h"

using namespace std;

class SayHello : public TimerProc
{
public:
	// 通过 TimerProc 继承
	void Proc() override
	{
		cout << "Hello" << endl;
	}
	int GetTimeSec() override
	{
		return 2;
	}
};

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new TimerChannel());

	TimerMng::GetInstance()->Add_Task(new SayHello);

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}