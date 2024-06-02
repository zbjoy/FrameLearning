#include <iostream>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

int main(void)
{
	StdinChannel* pIn = new StdinChannel;
	StdoutChannel* pOut = new StdoutChannel;
	pIn->SetOut(pOut);
	ZinxKernel::GetInstance().Zinx_Add_Channel(pIn);
	ZinxKernel::GetInstance().Zinx_Add_Channel(pOut);
	ZinxKernel::GetInstance().ZinxKernel_Run();
	return 0;
}