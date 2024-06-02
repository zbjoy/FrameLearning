#include "StdioChannel.h"
#include <iostream>

int StdioChannel::GetFd()
{
    return 0;
}

void StdioChannel::ReadFd()
{
    std::cin >> content;
}

void StdioChannel::WriteFd()
{
    std::cout << content << std::endl;
}
