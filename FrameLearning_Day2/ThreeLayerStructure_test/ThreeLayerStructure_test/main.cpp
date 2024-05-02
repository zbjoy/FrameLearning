#include <iostream>
#include <zinx.h>
#include "StdInOutChannel.h"
#include "EchoRole.h"

using namespace std;

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new StdInChannel()));
	ZinxKernel::Zinx_Add_Channel(*(new StdOutChannel()));
	ZinxKernel::Zinx_Add_Role(*(new EchoRole()));

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}