#include <iostream>
#include "ZinxKernel.h"
#include "StdinChannel.h"
#include "StdoutChannel.h"
#include "FIFOChannel.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

void fifo_test()
{
	//FIFOChannel readfifo("input");
	FIFOChannel* readfifo = new FIFOChannel("input");
	//FIFOChannel writefifo("output", false);
	FIFOChannel* writefifo = new FIFOChannel("output", false);
	//readfifo.pHandler = &writefifo;
	readfifo->pHandler = writefifo;
	//ZinxKernel::GetInstance().Zinx_Add_Channel(&readfifo);
	ZinxKernel::GetInstance().Zinx_Add_Channel(readfifo);
	//ZinxKernel::GetInstance().Zinx_Add_Channel(&writefifo);
	ZinxKernel::GetInstance().Zinx_Add_Channel(writefifo);
}

int main(void)
{
	fifo_test();
	StdinChannel* pIn = new StdinChannel;
	StdoutChannel* pOut = new StdoutChannel;
	pIn->SetOut(pOut);
	ZinxKernel::GetInstance().Zinx_Add_Channel(pIn);
	ZinxKernel::GetInstance().Zinx_Add_Channel(pOut);
	ZinxKernel::GetInstance().ZinxKernel_Run();
	return 0;
}