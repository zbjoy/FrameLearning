#include <iostream>
#include <algorithm>
#include "ZinxKernel.h"
#include "StdioChannel.h"
#include "FIFOChannel.h"
#include "IZinxHandler.h"

using namespace std;

ZinxKernel ZinxKernel::zinxKernel;

class TestMsg : public IMessage
{
public:
	string data;
};

class Next_Letter : public IZinxHandler
{
public:
	// 通过 IZinxHandler 继承
	IMessage* data_handle(IMessage* _input) override
	{
		auto pMsg = dynamic_cast<TestMsg*>(_input);
		printf("%c\n", pMsg->data[0] + 1);
		return nullptr;
	}
	IZinxHandler* get_next() override
	{
		return nullptr;
	}
} step3;

class ShowAscii : public IZinxHandler
{
public:
	// 通过 IZinxHandler 继承
	IMessage* data_handle(IMessage* _input) override
	{
		auto pMsg = dynamic_cast<TestMsg*>(_input);
		printf("%d\n", pMsg->data[0]);
		TestMsg* pRet = new TestMsg();
		pRet->data = pMsg->data;
		return pRet;
	}
	IZinxHandler* get_next() override
	{
		return &step3;
	}
} step2;

class Upper : public IZinxHandler
{
public:
	// 通过 IZinxHandler 继承
	IMessage* data_handle(IMessage* _input) override
	{
		auto pMsg = dynamic_cast<TestMsg*>(_input);

		//string temp("a");
		string temp;
		 temp.resize(1);
		
		transform(pMsg->data.begin(), pMsg->data.end(), temp.begin(), ::toupper);

		printf("%c\n", temp[0]);
		TestMsg* pRet = new TestMsg;
		pRet->data = temp;
		return pRet;
	}
	IZinxHandler* get_next() override
	{
		return &step2;
	}
} step1;


void test_chain()
{
	string temp;
	cin >> temp;
	TestMsg* msg = new TestMsg;
	msg->data = temp;
	step1.Handler(msg);
}

int main(void)
{
	// test_chain();
	// ZinxKernel::GetInstance().Add_Channel(new StdioChannel);
	StdinChannel* pIn = new StdinChannel;
	StdoutChannel* pOut = new StdoutChannel;
	// IZinxHandler* pProc = new IZinxHandler;
	// pProc->pNext = pOut;
	// pIn->Set_OutChannel(pProc);
	ZinxKernel::GetInstance().Add_Channel(pIn);
	ZinxKernel::GetInstance().Add_Channel(pOut);

	FIFOChannel* pFIn = new FIFOChannel("input", true);
	FIFOChannel* pFOut = new FIFOChannel("output", false);
	pFIn->SetOutChannel(pFOut);

	ZinxKernel::GetInstance().Add_Channel(pFIn);
	ZinxKernel::GetInstance().Add_Channel(pFOut);

	ZinxKernel::GetInstance().run();

	return 0;
}