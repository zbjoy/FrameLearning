#include <iostream>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"

using namespace std;

int main()
{
	pb::SyncPid* pMsg = new pb::SyncPid();
	pMsg->set_pid(1);
	pMsg->set_username("hello user");

	GameMsg gameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
	auto ret = gameMsg.serialize();

	for (char ch : ret)
	{
		printf("%02X ", ch);
	}
	fflush(stdout);
	// puts("");

	char buf[] = { 0x08, 0x01, 0x12, 0x0A,0x68,0x65,0x6C, 0x6C, 0x6F, 0x20, 0x75, 0x73, 0x65, 0x72 };
	GameMsg temp(GameMsg::MSG_TYPE_LOGIN_ID_NAME, string(buf, sizeof(buf)));
	cout << dynamic_cast<pb::SyncPid*>(temp.pMsg)->pid() << endl;
	cout << dynamic_cast<pb::SyncPid*>(temp.pMsg)->username() << endl;

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}