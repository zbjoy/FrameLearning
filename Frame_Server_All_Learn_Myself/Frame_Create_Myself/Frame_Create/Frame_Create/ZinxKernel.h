#pragma once

class Ichannel;
class ZinxKernel
{
public:
	static void Zinx_Run();

	static bool Zinx_Add_Channel(Ichannel* _pChannel);
	static void Zinx_Del_Channel(Ichannel* _pChannel);

	static bool ZinxKernel_Init();
	static void ZinxKernel_Fini();

	static bool Zinx_Set_OutChannel(Ichannel* _pChannel);
	static bool Zinx_Set_Restoration(Ichannel* _pChannel);

	static void ZinxKernel_Exit();

private:

	static int m_epoll_fd;
	static bool m_exit_status;
};

