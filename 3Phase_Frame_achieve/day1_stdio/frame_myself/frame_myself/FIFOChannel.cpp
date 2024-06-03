#include "FIFOChannel.h"
#include "ZinxKernel.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int FIFOChannel::GetFd()
{
    return m_fd;
}

std::string FIFOChannel::ReadFd(std::string _input)
{
    if (m_fd == -1)
    {
        m_fd = open(file.c_str(), O_RDONLY);// | O_NONBLOCK);
        std::cout << "尝试open O_RDONLY" << std::endl;
        if (m_fd == -1)
        {
            std::cout << "管道没打开, 读不进来东西" << std::endl;
        }
    }
    // std::cout << "有东西读入" << std::endl;
    char buf[1024] = { 0 };
    int num = read(m_fd, buf, 1024);

    if (pHandler->m_fd == -1)
    {
        std::cout << "尝试打开写管道" << std::endl;
        pHandler->m_fd = open(pHandler->file.c_str(), O_WRONLY | O_NONBLOCK);
        if (pHandler->m_fd == -1)
        {
            std::cout << "管道没打开, 写不进去东西" << std::endl;
        }
        else
        {
            std::cout << "管道打开成功, 可以写入数据了" << std::endl;
            ZinxKernel::GetInstance().Zinx_Add_Channel(pHandler);
            pHandler->data_sendout(std::string(buf, num));
        }
    }
    return std::string(buf, num);
}

void FIFOChannel::WriteFd(std::string _output)
{
    if (m_fd == -1)
    {
        std::cout << "尝试打开写管道" << std::endl;
        m_fd = open(file.c_str(), O_WRONLY);
        if (m_fd == -1)
        {
            std::cout << "管道没打开, 写不进去东西" << std::endl;
        }
    }

    // std::cout << "写管道尝试写进去东西" << std::endl;
    write(m_fd, _output.c_str(), _output.size());
}

void FIFOChannel::data_process(std::string _output)
{
    pHandler->data_sendout(_output);
}

bool FIFOChannel::Init()
{
    int flag = O_RDONLY;
    if (!bIsRead)
    {
        flag = O_WRONLY;
    }
    m_fd = -1;
    /* 用非阻塞的方式打开, 管道好难用 */
    m_fd = open(file.c_str(), flag | O_NONBLOCK);
    if (m_fd == -1)
    {
        if (flag == O_WRONLY)
        std::cout << "打开写管道失败" << std::endl;
        if (flag == O_RDONLY)
        std::cout << "打开读管道失败" << std::endl;

        return false;
    }
    return true;
}

void FIFOChannel::Fini()
{
    if (m_fd != -1)
    {
        close(m_fd);
    }
}
