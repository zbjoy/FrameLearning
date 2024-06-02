#include "StdioChannel.h"
#include <iostream>

void StdinChannel::Set_OutChannel(StdoutChannel* _pOut)
{
	pOut = _pOut;
}

int StdinChannel::GetFd()
{
	return 0;
}

std::string StdinChannel::ReadFd()
{
	std::string temp;
	std::cin >> temp;
	return temp;
}

void StdinChannel::WriteFd(std::string _output)
{
	// std::cout << _output << std::endl;
}

void StdinChannel::data_process(std::string _input)
{
	// 将数据回显到标准输出 --> 调用标准输出通道对象的sendout函数
	pOut->data_sendout(_input);
}

bool StdinChannel::Init()
{
	return true;
}

void StdinChannel::Fini()
{
}

int StdoutChannel::GetFd()
{
	return 1;
}

std::string StdoutChannel::ReadFd()
{
	return std::string();
}

void StdoutChannel::WriteFd(std::string _output)
{
	std::cout << _output << std::endl;
}

void StdoutChannel::data_process(std::string _input)
{
}

bool StdoutChannel::Init()
{
	return true;
}

void StdoutChannel::Fini()
{
}
