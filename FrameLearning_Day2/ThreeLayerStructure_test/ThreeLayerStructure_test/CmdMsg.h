#pragma once
#include <zinx.h>
#include <iostream>

using namespace std;

class CmdMsg :
    public UserData
{
public:
    string szUserData;
    bool isOpen = true;
    bool isCmd = false;
    bool isExit = false;
};

