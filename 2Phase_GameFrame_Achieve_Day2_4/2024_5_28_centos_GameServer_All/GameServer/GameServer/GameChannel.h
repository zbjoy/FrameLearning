#pragma once
#include <zinx.h>
#include <ZinxTCP.h>

class GameProtocol;
class GameChannel :
    public ZinxTcpData
{
public:
    GameChannel(int _fd) : ZinxTcpData(_fd) {};
    // 通过 ZinxTcpData 继承
    AZinxHandler* GetInputNextStage(BytesMsg& _oInput) override;

public:
    GameProtocol* m_protocol;
};


class GameConnFact : public IZinxTcpConnFact
{
public:
    // 通过 IZinxTcpConnFact 继承
    ZinxTcpData* CreateTcpDataChannel(int _fd) override;
};
