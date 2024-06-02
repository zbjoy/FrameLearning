#pragma once
#include "Ichannel.h"

class FIFOChannel : public Ichannel
{
	std::string fifoname;
	bool bIsRead = true;

	int m_fd;
	FIFOChannel* pOut = NULL;
public:
	FIFOChannel(std::string _fifoname, bool _bIsRead) : fifoname(_fifoname), bIsRead(_bIsRead) {};
	~FIFOChannel() {};

	void SetOutChannel(FIFOChannel* _pOut)
	{
		pOut = _pOut;
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

