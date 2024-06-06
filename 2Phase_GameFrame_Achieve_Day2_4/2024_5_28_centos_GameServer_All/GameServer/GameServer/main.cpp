#include <iostream>
#include <zinx.h>
#include "GameChannel.h"

#include "RandomName.h"
#include <hiredis/hiredis.h>

#include "TimeOutMng.h"
// #include "AOIWorld.h"

using namespace std;

//void RandomName_test()
//{
//	RandomName randomName;
//	randomName.Init();
//	string name1 = randomName.GetName();
//	string n2 = randomName.GetName();
//	cout << "name1: " << name1 << endl;
//	cout << "n2: " << n2 << endl;
//
//	randomName.ReleaseName(name1);
//	randomName.ReleaseName(n2);
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

class SayHello : public TimeOutProc
{
public:
	// 通过 TimeOutProc 继承
	int GetTime() override
	{
		return 13;
	}
	void Proc() override
	{
		cout << "Hello" << endl;
	}
} *hello = new SayHello;

class SayNo : public TimeOutProc
{
public:
	// 通过 TimeOutProc 继承
	int GetTime() override
	{
		return 9;
	}
	void Proc() override
	{
		cout << "No" << endl;
	}
} *no = new SayNo;

void Timer_test()
{
	TimeOutMng::GetInstance()->Add_Task(hello);
	TimeOutMng::GetInstance()->Add_Task(no);

}



// class RandomName;
extern RandomName randomName;

void daemon_init();
int main()
{

	// AOIWorld_Test();
	// RandomName_test();
	// Timer_test();
	/* 初始化redis连接 */
	//redisContext* c = redisConnect("127.0.0.1", 6379);
	//if (c == NULL || c->err) {
	//	if (c) {
	//		printf("Error: %s\n", c->errstr);
	//		// handle error
	//	}
	//	else {
	//		printf("Can't allocate redis context\n");
	//	}
	//}

	///* redis connect test */
	//void* reply = redisCommand(c, "lpush name tom");
	//freeReplyObject(reply);
	//cout << "已经添加tom" << endl;
	daemon_init();

	randomName.Init();

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));
	ZinxKernel::Zinx_Add_Channel(*new TimerChannel());

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();

	//redisFree(c);
	return 0;
}

void daemon_init()
{
	pid_t pid;
	int i;

	pid = fork();
	if (pid < 0)
	{
		cout << "Error Fork" << endl;
		exit(1);
	}
	else if (pid > 0)
	{
		// 父进程
		exit(0);
	}

	pid = setsid();
	
	int nil = open("/dev/null", O_RDWR);
	dup2(nil, STDIN_FILENO);
	dup2(nil, STDOUT_FILENO);
	dup2(nil, STDERR_FILENO);
	close(nil);

	while (true)
	{
		int pid2 = fork();

		if (pid2 < 0)
		{
			exit(1);
		}

		if (pid2 > 0)
		{
			// 父进程等待
			int status;
			wait(&status);

			if (status == 0)
			{
				exit(0);
			}
		}
		else
		{
			break;
		}
	}

}