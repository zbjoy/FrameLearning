#pragma once
#include <string>
#include "Ichannel.h"

class StdoutChannel : public Ichannel
{
public:
	// 通过 Ichannel 继承
	int GetFd() override;
	std::string ReadFd() override;
	void WriteFd(std::string _output) override;
	void data_process(std::string _input) override;

	// 通过 Ichannel 继承
	bool Init() override;
	void Fini() override;
};

class StdinChannel : public Ichannel
{
	StdoutChannel* pOut = nullptr;
public:
	void Set_OutChannel(StdoutChannel* _pOut);
	StdoutChannel* Get_OutChannel()
	{
		return pOut;
	}
	// 通过 Ichannel 继承
	int GetFd() override;
	std::string ReadFd() override;
	void WriteFd(std::string _output) override;
	void data_process(std::string _input) override;

	// 通过 Ichannel 继承
	bool Init() override;
	void Fini() override;
};
