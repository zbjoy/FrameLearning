#include <iostream>
#include <zinx.h>
#include "RoleProcess.h"
#include "IOChannel.h"

using namespace std;

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new StdInChannel());
	ZinxKernel::Zinx_Add_Channel(*new StdOutChannel());
	ZinxKernel::Zinx_Add_Role(*new Echo());
	ZinxKernel::Zinx_Add_Role(*new AddDatePreCtl());
	ZinxKernel::Zinx_Add_Role(*new InOutCtl());
	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(9998, new myTcpConnFact()));

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}