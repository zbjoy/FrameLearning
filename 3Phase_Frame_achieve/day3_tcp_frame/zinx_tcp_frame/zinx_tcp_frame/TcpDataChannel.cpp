#include "TcpDataChannel.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

bool TcpDataChannel::Init()
{
	return true;
}

bool TcpDataChannel::ReadFd(std::string& _input)
{
	char szBuff[128] = { 0 };
	int irecvLen = -1;
	bool bRet = false;

	while (true)
	{
		irecvLen = recv(m_fd, szBuff, sizeof(szBuff), MSG_DONTWAIT);
		std::cout << "接收到消息" << std::endl;
		if (irecvLen == 0)
		{
			/* 链接断开 */
			std::cout << "连接断开" << std::endl;
			SetChannelClose();
			break;
		}
		else if (irecvLen == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
			{
				break;
			}
			SetChannelClose();
			break;
		}
		else
		{
			_input.append(szBuff, irecvLen);
			bRet = true;
		}
	}

	return bRet;
}

bool TcpDataChannel::WriteFd(std::string& _output)
{
	send(m_fd, _output.data(), _output.size(), 0);
	return true;
}

void TcpDataChannel::Fini()
{
	if (m_fd > 0)
	{
		close(m_fd);
		m_fd = -1;
	}
}

int TcpDataChannel::GetFd()
{
	return m_fd;
}

std::string TcpDataChannel::GetChannelInfo()
{
	return "TcpDataChannel";
}


TcpListenChannel::TcpListenChannel(unsigned short _port, TcpDataChannelFact* _pFact) : m_port(_port), m_fact(_pFact)
{
}

TcpListenChannel::~TcpListenChannel()
{
	if (nullptr != m_fact)
	{
		delete m_fact;
	}
}

bool TcpListenChannel::Init()
{
	bool bRet = false;
	m_server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (0 <= m_server_fd)
	{
		sockaddr_in addr;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(m_port);
		addr.sin_family = AF_INET;

		// 设置重复绑定(端口复用)
		int iflag = 1;
		setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &iflag, sizeof(iflag));



		int ret = bind(m_server_fd, (sockaddr*)&addr, sizeof(addr));
		if (ret == 0)
		{
			if (0 == listen(m_server_fd, 1000))
			{
				bRet = true;
			}
			else
			{
				perror(__FILE__":""__FUNC__"":""bind()");
			}
		}
		else
		{
			perror(__FILE__":""__FUNC__"":""bind()");
		}

	}
	else
	{
		perror(__FILE__":""__FUNC__"":""bind()");
	}
	return bRet;
}

bool TcpListenChannel::ReadFd(std::string& _input)
{
	bool bRet = false;

	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	int client_fd = accept(m_server_fd, (sockaddr*)&addr, &addrlen);
	std::cout << "接收到一个链接" << std::endl;
	if (0 <= client_fd)
	{
		auto pDataChannel = m_fact->CreateChannel(client_fd);
		bRet = ZinxKernel::Zinx_Add_Channel(*pDataChannel);
	}
	else
	{
		perror(__FILE__":""__FUNC__"":""accept");
	}

	return bRet;
}

bool TcpListenChannel::WriteFd(std::string& _output)
{
	return false;
}

void TcpListenChannel::Fini()
{
	if (0 <= m_server_fd)
	{
		close(m_server_fd);
	}
}

int TcpListenChannel::GetFd()
{
	return m_server_fd;
}

std::string TcpListenChannel::GetChannelInfo()
{
	return "TcpDataListen";
}

AZinxHandler* TcpListenChannel::GetInputNextStage(BytesMsg& _oInput)
{
	return nullptr;
}

