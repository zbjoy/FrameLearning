#pragma once
#include "zinx.h"

class TcpDataChannel : public Ichannel
{
	int m_fd;
public:
	TcpDataChannel(int _fd) : m_fd(_fd) {}
	virtual ~TcpDataChannel() {}
	// 通过 Ichannel 继承
	bool Init() override;
	bool ReadFd(std::string& _input) override;
	bool WriteFd(std::string& _output) override;
	void Fini() override;
	int GetFd() override;
	std::string GetChannelInfo() override;
	// AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class TcpDataChannelFact
{
public:
	virtual TcpDataChannel* CreateChannel(int _fd) = 0;
};

class TcpListenChannel : public Ichannel
{
	int m_server_fd;
	unsigned short m_port;
	TcpDataChannelFact* m_fact;
public:
	TcpListenChannel(unsigned short _port, TcpDataChannelFact* _pFact);
	~TcpListenChannel();
	// 通过 Ichannel 继承
	bool Init() override;
	bool ReadFd(std::string& _input) override;
	bool WriteFd(std::string& _output) override;
	void Fini() override;
	int GetFd() override;
	std::string GetChannelInfo() override;
	AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};


