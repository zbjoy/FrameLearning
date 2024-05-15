#include <iostream>
#include <zinx.h>
#include "ZinxTimer.h"

using namespace std;

int main(void)
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*(new ZinxTimer()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}