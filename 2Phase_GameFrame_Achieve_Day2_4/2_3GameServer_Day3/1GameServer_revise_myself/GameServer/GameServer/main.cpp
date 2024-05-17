#include <iostream>
#include <zinx.h>
#include "GameChannel.h"

/*
	bug: 当有两个玩家Tom5(先进入游戏), Tom6(后进入游戏) 在两个玩家位置较远的时候, Tom6发的消息Tom5可以看到, 但是Tom5发的Tom6看不到
*/

using namespace std;

int main(void)
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}