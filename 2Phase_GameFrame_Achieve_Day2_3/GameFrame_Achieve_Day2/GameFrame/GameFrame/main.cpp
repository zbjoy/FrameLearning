#include <iostream>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "GameMsg.h"
#include "msg.pb.h"
#include "AOIWorld.h"

using namespace std;

/* 测试代码 */
class myPlayer : public Player
{
public:
	float x = 0;
	float y = 0;
	string name;

	myPlayer(float _x, float _y, string _name) : x(_x), y(_y), name(_name) {};

	// 通过 Player 继承
	float GetX() override
	{
		return x;
	}

	float GetY() override
	{
		return y;
	}

};

int main()
{
	/* 测试代码 */
	
		pb::SyncPid* pMsg = new pb::SyncPid();
		pMsg->set_pid(1);
		pMsg->set_username("test");

		GameMsg gameMsg(GameMsg::MSG_TYPE_LOGIN_ID_NAME, pMsg);
		auto ret = gameMsg.serialize();

		for (char ch : ret)
		{
			printf("%02X ", ch);
		}
		fflush(stdout);
		// puts("");

		// 测试的序列化后的数据 内容为 1 "hello user" 
		char buf[] = { 0x08, 0x01, 0x12, 0x0A,0x68,0x65,0x6C, 0x6C, 0x6F, 0x20, 0x75, 0x73, 0x65, 0x72 };
		GameMsg temp(GameMsg::MSG_TYPE_LOGIN_ID_NAME, string(buf, sizeof(buf)));
		cout << dynamic_cast<pb::SyncPid*>(temp.pMsg)->pid() << endl;
		cout << dynamic_cast<pb::SyncPid*>(temp.pMsg)->username() << endl;
		

	/* 测试代码 */
		AOIWorld w(20, 200, 50, 230, 6, 6);
		myPlayer p1(60.2, 107.2, "玩家1");
		myPlayer p2(91.5, 118.3, "玩家2");
		myPlayer p3(147.6, 133.6, "玩家3");
		w.Add_Player(&p1);
		w.Add_Player(&p2);
		w.Add_Player(&p3);

		list<Player*> tmp = w.GetSrdPlayer(&p1);

		for (auto single : tmp)
		{
			cout << "玩家姓名: " << (dynamic_cast<myPlayer*>(single))->name << endl;
		}


	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}