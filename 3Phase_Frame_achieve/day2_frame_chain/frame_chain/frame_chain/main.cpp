#include <iostream>
#include "ZinxKernel.h"
#include "StdioChannel.h"
#include "FIFOChannel.h"
#include "IZinxHandler.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

int main(void)
{
	// ZinxKernel::GetInstance().Add_Channel(new StdioChannel);
	StdinChannel* pIn = new StdinChannel;
	StdoutChannel* pOut = new StdoutChannel;
	IZinxHandler* pProc = new IZinxHandler;
	pProc->pNext = pOut;
	pIn->Set_OutChannel(pProc);
	ZinxKernel::GetInstance().Add_Channel(pIn);
	ZinxKernel::GetInstance().Add_Channel(pOut);

	FIFOChannel* pFIn = new FIFOChannel("input", true);
	FIFOChannel* pFOut = new FIFOChannel("output", false);
	pFIn->SetOutChannel(pFOut);

	ZinxKernel::GetInstance().Add_Channel(pFIn);
	ZinxKernel::GetInstance().Add_Channel(pFOut);

	ZinxKernel::GetInstance().run();

	return 0;
}