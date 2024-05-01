#include <zinx.h>
#include <iostream>

using namespace std;

class TestOut : public Ichannel
{
public:
	// 通过 Ichannel 继承
	bool Init() override
	{
		return true;
	}
	bool ReadFd(std::string& _input) override
	{
		return false;
	}
	bool WriteFd(std::string& _output) override
	{
		cout << _output << endl;
		return true;
	}
	void Fini() override
	{
	}
	int GetFd() override
	{
		return STDOUT_FILENO;
	}
	std::string GetChannelInfo() override
	{
		return "TestOut";
	}
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return nullptr;
	}
} *poOut = new TestOut();

class Echo : public AZinxHandler
{
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		//cout << oBytes.szData;
		ZinxKernel::Zinx_SendOut(oBytes.szData, *poOut);
		return nullptr;
	}

	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}

} *poEcho = new Echo();

class TestStdin : public Ichannel
{
public:
	// 通过 Ichannel 继承
	bool Init() override
	{
		return true;
	}
	bool ReadFd(std::string& _input) override
	{
		cin >> _input;
		return true;
	}
	bool WriteFd(std::string& _output) override
	{
		return false;
	}
	void Fini() override
	{
	}
	int GetFd() override
	{
		return STDIN_FILENO;
	}
	std::string GetChannelInfo() override
	{
		return "TestStdin";
	}
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return poEcho;
	}
} *poIn = new TestStdin();

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*poIn);
	ZinxKernel::Zinx_Add_Channel(*poOut);
	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}