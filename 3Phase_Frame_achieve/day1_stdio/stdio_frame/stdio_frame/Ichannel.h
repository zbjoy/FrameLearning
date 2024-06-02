#pragma once


class Ichannel
{
public:
	virtual int GetFd() = 0;
	virtual void ReadFd() = 0;
	virtual void WriteFd() = 0;
	void flushBuffer()
	{

	}
};

