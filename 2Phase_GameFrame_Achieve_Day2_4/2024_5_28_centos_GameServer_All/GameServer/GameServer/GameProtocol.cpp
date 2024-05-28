#include "GameRole.h"
#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"

UserData* GameProtocol::raw2request(std::string _szInput)
{
    /* 解析接收到的消息 */
    MultiMsg* pRetMsg = new MultiMsg();

    /* 用滑动窗口的方式接收数据 */
    // 数据类型: length + pid + data

    szLast.append(_szInput);

    while (true)
    {
        if (szLast.size() < sizeof(int))
        {
            break;
        }

        int length = 0;

        // 有一个整形提升
        length |= szLast[0] << 0;
        length |= szLast[1] << 8;
        length |= szLast[2] << 16;
        length |= szLast[3] << 24;

        if (length > szLast.size() - 8)
        {
            break;
        }

        int pid = 0;

        pid |= szLast[4] << 0;
        pid |= szLast[5] << 8;
        pid |= szLast[6] << 16;
        pid |= szLast[7] << 24;

        GameMsg* tempMsg = new GameMsg((GameMsg::MSG_TYPE)pid, szLast.substr(8, length));
        pRetMsg->m_msg_list.push_back(tempMsg);
        szLast.erase(0, length + 8);

        std::cout << "(protocol层)接收到消息: (google debug)" << std::endl;
        std::cout << tempMsg->pMsg->Utf8DebugString() << std::endl;
    }

    return pRetMsg;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    GET_REF2DATA(GameMsg, _gameMsg, _oUserData);

    std::string content = _gameMsg.serialize();
    int length = content.size();
    int pid = _gameMsg.enMsgType;

    std::string* pRetStr = new std::string();

    pRetStr->push_back(length >> 0);
    pRetStr->push_back(length >> 8);
    pRetStr->push_back(length >> 16);
    pRetStr->push_back(length >> 24);

    pRetStr->push_back(pid >> 0);
    pRetStr->push_back(pid >> 8);
    pRetStr->push_back(pid >> 16);
    pRetStr->push_back(pid >> 24);

    pRetStr->append(content);

    return pRetStr;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return m_role;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return m_channel;
}
