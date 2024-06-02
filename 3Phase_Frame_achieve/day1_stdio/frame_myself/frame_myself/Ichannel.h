#pragma once
#include <iostream>
#include <sys/epoll.h>

class Ichannel
{
public:
	virtual int GetFd() = 0;
	virtual std::string ReadFd(std::string _input) = 0;
	virtual void WriteFd(std::string _output) = 0;
	virtual void data_process(std::string _output) = 0;
	void flush_buffer()
	{
		WriteFd(m_buffer);
		m_buffer.clear();
	}

	void data_sendout(std::string _output);


private:
	std::string m_buffer;
};

