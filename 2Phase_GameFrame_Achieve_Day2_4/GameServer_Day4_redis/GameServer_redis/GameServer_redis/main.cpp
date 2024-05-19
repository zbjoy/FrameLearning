#include <iostream>
#include <zinx.h>
#include <ZinxTCP.h>
#include "GameChannel.h"

using namespace std;

/* 这次延迟好像有点高 */

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

int main()
{
	// deamon_init();

	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*new ZinxTCPListen(8899, new GameConnFact()));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}