#include <iostream>
#include "TcpDataChannel.h"

using namespace std;

class Echo : public AZinxHandler
{
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, _b, _oInput);
		cout << "---------" << endl;
		cout << _b.szData << endl;
		return nullptr;
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}
} echo;

class tcpDataTest : public TcpDataChannel
{
public:
	tcpDataTest(int _fd) : TcpDataChannel(_fd) {}
	// 通过 TcpDataChannel 继承
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return &echo;
	}
};

class tcpTest : public TcpDataChannelFact
{
public:
	// 通过 TcpDataChannelFact 继承
	TcpDataChannel* CreateChannel(int _fd) override
	{
		
		return new tcpDataTest(_fd);
	}
};

int main(void)
{
	ZinxKernel::ZinxKernelInit();
	ZinxKernel::Zinx_Add_Channel(*(new TcpListenChannel(8899, new tcpTest())));

	ZinxKernel::Zinx_Run();
	ZinxKernel::ZinxKernelFini();
	return 0;
}