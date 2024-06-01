#include <iostream>
#include <zinx.h>
#include "GameChannel.h"

#include "RandomName.h"

// #include "AOIWorld.h"

using namespace std;

//void RandomName_test()
//{
//	RandomName randomName;
//	randomName.Init();
//	randomName.GetName();
//}

//class TestPlayer : public Player
//{
//public:
//	TestPlayer(string _name, float _x, float _y) : name(_name), x(_x), y(_y) {} ;
//	float x;
//	float y;
//	string name;
//	// 通过 Player 继承
//	float GetX() override
//	{
//		return x;
//	}
//	float GetY() override
//	{
//		return y;
//	}
//};
//
//void AOIWorld_Test()
//{
//	AOIWorld world(0, 400, 0, 400, 20, 20);
//	TestPlayer p1("p1", 17.2, 12.1);
//	TestPlayer p2("p2", 13.2, 16.1);
//	TestPlayer p3("p3", 33.7, 12.1);
//	TestPlayer p4("p4", 29.5, 7.2);
//	TestPlayer p5("p5", 100.2, 98.5);
//
//	world.Add_Player(&p1);
//	world.Add_Player(&p2);
//	world.Add_Player(&p3);
//	world.Add_Player(&p4);
//	world.Add_Player(&p5);
//
//	auto players = world.GetSrdPlayersPosition(&p2);
//	for (auto single : players)
//	{
//		auto p = dynamic_cast<TestPlayer*>(single);
//		cout << p->name << endl;
//	}
//}

int main()
{

	// AOIWorld_Test();
	// RandomName_test();

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}