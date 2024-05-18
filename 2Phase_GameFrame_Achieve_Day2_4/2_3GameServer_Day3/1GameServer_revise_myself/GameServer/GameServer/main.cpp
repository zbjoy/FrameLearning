#include <iostream>
#include <zinx.h>
#include "GameChannel.h"
#include "ZinxTimer.h"

/*
	bug(2024_5_17): 当有两个玩家Tom5(先进入游戏), Tom6(后进入游戏) 在两个玩家位置较远的时候, Tom6发的消息Tom5可以看到, 但是Tom5发的Tom6看不到
	debug(2024_5_18): 应该是AOIWorld中因为grid的错误导致, 在修改后好像正常了(测试了三次, 第一次还是不正常, 但之后正常了, 所以应该是第一次没有在编译完成前就把.out启动的原因)

	修改了google::protobuf中的message.h的205行: 将~Message()改为了虚析构
	好像和google的源码无关
*/

using namespace std;

int main(void)
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));
	ZinxKernel::Zinx_Add_Channel(*new ZinxTimerChannel());

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}