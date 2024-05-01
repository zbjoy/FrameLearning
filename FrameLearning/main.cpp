#include <zinx.h>
#include <iostream>

using namespace std;

class TestOut : public Ichannel {
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

class Echo : public AZinxHandler {
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		ZinxKernel::Zinx_SendOut(oBytes.szData, *poOut);
		return nullptr;
	}

	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return nullptr;
	}
} *poEcho = new Echo();

class AddHandler : public AZinxHandler {
public:
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		time_t tmp;
		time(&tmp);
		char* str = ctime(&tmp);
		str[strlen(str) - 1] = 0; // 去掉末尾的\n
		string szNew = string(str) + " : " + oBytes.szData;
		BytesMsg* pRet = new BytesMsg(oBytes);
		pRet->szData = szNew;
		return pRet;
	}
	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		return poEcho;
	}
} *poAdd = new AddHandler();

class CmdHandler : public AZinxHandler {
public:
	enum State
	{
		CLOSE,
		OPEN,
		DATE,
	} status = OPEN;
	// 通过 AZinxHandler 继承
	IZinxMsg* InternelHandle(IZinxMsg& _oInput) override
	{
		GET_REF2DATA(BytesMsg, oBytes, _oInput);
		if (oBytes.szData == "exit")
		{
			ZinxKernel::Zinx_Exit();
		}
		else if (oBytes.szData == "open")
		{
			ZinxKernel::Zinx_Add_Channel(*poOut);
			status = OPEN;
			return nullptr;
		}
		else if (oBytes.szData == "close")
		{
			ZinxKernel::Zinx_Del_Channel(*poOut);
			status = CLOSE;
			/* 这样返回的话再次 open 时不会把之前 close 的输入值发送给 poEcho */
			return nullptr;
			/* 如果这样返回就会发送*/
			// return new BytesMsg(oBytes);
		}

		if (status == OPEN || status == DATE)
		{
			if (oBytes.szData == "date")
			{
				status = DATE;
				return nullptr;
			}
			else if (oBytes.szData == "closedate")
			{
				status = OPEN;
				return nullptr;
			}
		}

		if (status == CLOSE)
		{
			return nullptr;
		}
		else
		{
			return new BytesMsg(oBytes);
		}
	}

	AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) override
	{
		if (status == DATE)
		{
			/* 如果为DATE, 返回poADD*/
			return poAdd;
		}
		else
		{
			return poEcho;
		}
	}
} *poCmd = new CmdHandler();

class TestIn :public Ichannel {
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
		return "TestIn";
	}
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override
	{
		return poCmd;
	}
} *poIn = new TestIn();

int main()
{
	ZinxKernel::ZinxKernelInit();

	ZinxKernel::Zinx_Add_Channel(*poIn);
	ZinxKernel::Zinx_Add_Channel(*poOut);

	ZinxKernel::Zinx_Run();

	ZinxKernel::ZinxKernelFini();
	return 0;
}