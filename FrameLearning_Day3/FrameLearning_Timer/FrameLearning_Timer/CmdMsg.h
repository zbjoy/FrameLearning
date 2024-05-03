#pragma once
#include <zinx.h>

enum STATE
{
    OPEN,
    CLOSE,
    EXIT,
    DATE,
    CLOSEDATE
};

class CmdMsg :
    public UserData
{
public:
    std::string szData;

    bool isCmd = false;
    STATE mCmd;

};

