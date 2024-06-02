#include "FIFOChannel.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int FIFOChannel::GetFd()
{
    return m_fd;
}

std::string FIFOChannel::ReadFd()
{
    char buf[1024];
    int bytes_count = read(m_fd, buf, 1024);
    if (bytes_count > 0)
    {
        return std::string(buf, bytes_count);
    }

    return std::string();
}

void FIFOChannel::WriteFd(std::string _output)
{
    char buf[_output.size()];
    _output.copy(buf, _output.size(), 0);

    write(m_fd, buf, _output.size());
}

void FIFOChannel::data_process(std::string _input)
{
    pOut->data_sendout(_input);
}

bool FIFOChannel::Init()
{
    bool bRet = false;

    int flag = O_RDONLY;
    if (!bIsRead)
    {
        flag = O_WRONLY;
    }
    m_fd = open(fifoname.c_str(), flag);
    if (m_fd != -1)
    {
        bRet = true;
    }

    return bRet;
}

void FIFOChannel::Fini()
{
    if (m_fd != -1)
    {
        close(m_fd);
    }
}
