#pragma once
#include <iostream>

class Ichannel
{
public:
	virtual bool Init() = 0;
	virtual void Fini() = 0;
	virtual int GetFd() = 0;
	virtual std::string ReadFd() = 0;
	virtual void WriteFd(std::string _output) = 0;
	virtual void data_process(std::string _input) = 0;
	void flushBuffer()
	{
		WriteFd(m_buffer);
		m_buffer.clear();
	}

	void data_sendout(std::string _output);

private:
	std::string m_buffer;
};

