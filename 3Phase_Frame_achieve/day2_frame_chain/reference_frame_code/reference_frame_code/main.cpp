#include <iostream>
#include <algorithm>
#include "Ichannel.h"
#include "ZinxKernel.h"

using namespace std;

class StdOut : public Ichannel
{
public:
	// 通过 Ichannel 继承
	bool Init() override
	{
		return true;
	}
	void Fini() override
	{
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
	int GetFd() override
	{
		return 1;
	}
	IZinxHandler* GetInputNextStage(ByteMsg* _byte) override
	{
		return nullptr;
	}
} stdOut;

class Upper : public IZinxHandler
{
public:
	// 通过 IZinxHandler 继承
	IZinxMsg* Internel_Handle(IZinxMsg* _inputMsg) override
	{
		auto pMsg = dynamic_cast<ByteMsg*>(_inputMsg);
		string s = pMsg->content;
		if (s[0] <= 'z' && s[0] >= 'a')
		{
			transform(s.begin(), s.end(), s.begin(), ::toupper);
			sysIODicMsg dic(sysIODicMsg::IO_OUT);
			ByteMsg* ret = new ByteMsg(s, dic);
			return ret;
		}
		return nullptr;
	}
	IZinxHandler* GetNext(IZinxMsg* _next_input) override
	{
		return &stdOut;
	}
} upper;

class StdIn : public Ichannel
{
public:
	// 通过 Ichannel 继承
	bool Init() override
	{
		return true;
	}
	void Fini() override
	{
	}
	bool ReadFd(std::string& _input) override
	{
		cin >> _input;
		ZinxKernel::Zinx_Sendout(_input, &stdOut);
		return true;
	}
	bool WriteFd(std::string& _output) override
	{
		return false;
	}
	int GetFd() override
	{
		return 0;
	}
	IZinxHandler* GetInputNextStage(ByteMsg* _byte) override
	{
		return &upper;
	}
} stdIn;


int main(void)
{
	
	ZinxKernel::GetInstance().Zinx_Add_Channel(&stdIn);
	ZinxKernel::GetInstance().Zinx_Add_Channel(&stdOut);
	ZinxKernel::GetInstance().Zinx_Run();
	return 0;
}