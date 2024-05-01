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

class ExitFramework : public AZinxHandler
{
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		if (oBytes.szData == "exit")
		{
			ZinxKernel::Zinx_Exit();
			return nullptr;
		}
		return new BytesMsg(oBytes);
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return poEcho;
	}
} *poExit = new ExitFramework();

class AddHandler : public AZinxHandler
{
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		time_t tmp;
		time(&tmp);
		char* p = ctime(&tmp);
		p[strlen(p) - 1] = 0; //去掉末尾的换行符
		string szNew = string(p) + " : " + oBytes.szData;
		BytesMsg *pret = new BytesMsg(oBytes);
		pret->szData = szNew;
		return pret;
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return poEcho;
	}
} *poAdd = new AddHandler;

class CmdHandler : public AZinxHandler
{
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		if (oBytes.szData == "open")
		{
			ZinxKernel::Zinx_Add_Channel(*poOut);
		}
		else if (oBytes.szData == "close")
		{
			ZinxKernel::Zinx_Del_Channel(*poOut);
		}
		else if (oBytes.szData == "date")
		{
			status = 1;
		}
		else if (oBytes.szData == "closedate")
		{
			status = 0;
		}

		return new BytesMsg(oBytes);
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		if (status == 1)
		{
			return poAdd;
		}
		return poExit;
	}

	int status = 0;

} *poCmd = new CmdHandler();


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
		return poCmd;
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