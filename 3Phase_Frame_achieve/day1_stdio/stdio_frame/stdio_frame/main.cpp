#include <iostream>
#include "ZinxKernel.h"
#include "StdioChannel.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

int main(void)
{
	// ZinxKernel::GetInstance().Add_Channel(new StdioChannel);
	StdinChannel* pIn = new StdinChannel;
	StdoutChannel* pOut = new StdoutChannel;
	pIn->Set_OutChannel(pOut);
	ZinxKernel::GetInstance().Add_Channel(pIn);
	ZinxKernel::GetInstance().Add_Channel(pOut);
	ZinxKernel::GetInstance().run();
	return 0;
}