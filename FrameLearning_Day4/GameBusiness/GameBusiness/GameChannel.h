#pragma once

#include <ZinxTCP.h>
#include "GameProtocol.h"

class GameChannel :
    public ZinxTcpData
{
public:
    GameChannel(int _fd) : ZinxTcpData(_fd) {};
    // 通过 ZinxTcpData 继承

    /* 每一个channel对应一个protocol */
    GameProtocol* m_proto = nullptr;
    
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;
};

class GameConnFact : public IZinxTcpConnFact
{
public:
    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;

};

