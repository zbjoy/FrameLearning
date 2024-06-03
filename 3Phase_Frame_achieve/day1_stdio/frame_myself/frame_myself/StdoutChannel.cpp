#include "StdoutChannel.h"
#include <unistd.h>

int StdoutChannel::GetFd()
{
    return STDOUT_FILENO;
}

std::string StdoutChannel::ReadFd(std::string _input)
{
    return std::string();
}

void StdoutChannel::WriteFd(std::string _output)
{
    char buf[_output.size()];
    _output.copy(buf, _output.size());
    write(GetFd(), buf, _output.size());
}

void StdoutChannel::data_process(std::string _output)
{
}

bool StdoutChannel::Init()
{
    return true;
}

void StdoutChannel::Fini()
{
}
