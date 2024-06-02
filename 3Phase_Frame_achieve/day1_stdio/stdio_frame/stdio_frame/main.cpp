#include <iostream>
#include "ZinxKernel.h"
#include "StdioChannel.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

int main(void)
{
	ZinxKernel::GetInstance().Add_Channel(new StdioChannel);
	ZinxKernel::GetInstance().run();
	return 0;
}