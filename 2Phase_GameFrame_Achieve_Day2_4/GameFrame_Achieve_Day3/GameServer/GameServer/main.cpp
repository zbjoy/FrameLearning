#include <iostream>
#include <zinx.h>
#include "GameChannel.h"
#include "msg.pb.h"

using namespace std;

void test()
{
	pb::SyncPid s;
	s.set_pid(1);
	s.set_username("test");

	string temp;
	s.SerializeToString(&temp);

	for (char ch : temp)
	{
		printf("%02X ", ch);
	}
	cout << endl;
}

int main()
{
	/* 调试代码 */
	test();

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}