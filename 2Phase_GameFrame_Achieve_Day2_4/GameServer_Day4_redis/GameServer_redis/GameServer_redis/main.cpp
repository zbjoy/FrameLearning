#include <iostream>
#include <zinx.h>
#include <ZinxTCP.h>
#include "GameChannel.h"
#include "RandomName.h"
#include "TimerChannel.h"
#include <hiredis/hiredis.h>

using namespace std;

/* 这次延迟好像有点高 */

extern class RandomName randomName;

void deamon_init()
{
	int pid = fork();

	if (pid < 0)
	{
		exit(-1);
	}

	if (pid > 0)
	{
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
			exit(-1);
		}

		if (pid2 > 0)
		{
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

//class testSayHello : public TimerProc
//{
//public:
//	// 通过 TimerProc 继承
//	void Proc() override
//	{
//		cout << "Hello World!!" << endl;
//	}
//	int GetTimerSec() override
//	{
//		return 2;
//	}
//};

int main()
{
	deamon_init();
	randomName.LoadFile();

	// redisContext* redisConnect(const char* ip, int port);
	// redisConnect("127.0.0.1", 8899);
	// auto content = redisConnect("127.0.0.1", 8899);
	redisContext* c = redisConnect("127.0.0.1", 6379);
	if (c == NULL || c->err) {
		if (c) {
			printf("Error: %s\n", c->errstr);
			// handle error
		}
		else {
			printf("Can't allocate redis context\n");
		}
	}

	// void* redisCommand(redisContext * c, const char* format, ...);
	// auto reply = redisCommand(c, "set haha %s", "abc");
	// void freeReplyObject(void* reply);
	// freeReplyObject(reply);
	// redisFree(c);


	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));
	ZinxKernel::Zinx_Add_Channel(*new TimerChannel);

	// TimerMng::GetInstance()->Add_Task(new testSayHello);

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}