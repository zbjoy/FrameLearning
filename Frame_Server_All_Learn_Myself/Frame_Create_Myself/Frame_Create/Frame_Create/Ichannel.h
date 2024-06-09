#pragma once
#include <string>
#include "ZinxKernel.h"

class Ichannel
{
	friend class ZinxKernel;
public:

	virtual bool Init() = 0;
	virtual void Fini() = 0;
	virtual bool ReadFd(std::string& _Input) = 0;
	virtual bool WriteFd(std::string& _Output) = 0;
	virtual int GetFd() = 0;

private:
	std::string m_buffer;
};

