#include "StdinChannel.h"
#include <unistd.h>

int StdinChannel::GetFd()
{
    return STDIN_FILENO;
}

std::string StdinChannel::ReadFd(std::string _input)
{
    // std::cin >> _input;
    char buf[1024];
    _input.copy(buf, 1024, 0);
    int num = read(GetFd(), buf, 1024);
    _input.assign(buf, num);
    return _input;
}

void StdinChannel::WriteFd(std::string _output)
{
}

void StdinChannel::data_process(std::string _output)
{
    pOut->data_sendout(_output);
}

bool StdinChannel::Init()
{
    return true;
}

void StdinChannel::Fini()
{
}
