#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameRole.h"
#include "GameMsg.h"

UserData* GameProtocol::raw2request(std::string _szInput)
{
    MultiMsg* pRetMsg = new MultiMsg();
	// szLast.append(_szInput.data()); // 这样是错的, 为什么???
	szLast.append(_szInput);
	// szLast.append(_szInput.c_str());

    while (true)
    {
        if (szLast.size() < sizeof(int))
        {
            break;
        }

        int length = 0;
        int pid = 0;

        length |= szLast[0] << 0;
        length |= szLast[1] << 8;
        length |= szLast[2] << 16;
        length |= szLast[3] << 24;

        if (szLast.size() < length + 8)
        {
            /* 不够 length + id + content */
            std::cout << "消息不足" << std::endl;
            break;
        }

        pid |= szLast[4] << 0;
        pid |= szLast[5] << 8;
        pid |= szLast[6] << 16;
        pid |= szLast[7] << 24;

        GameMsg* temp = new GameMsg((GameMsg::MSG_TYPE)pid, szLast.substr(8, length));
        std::cout << temp->pMsg->Utf8DebugString();

        pRetMsg->m_msg_list.push_back(temp);

        szLast.erase(0, length + 8);
    }

    return pRetMsg;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    /* 接收GameMsg类型的消息 */
    GET_REF2DATA(GameMsg, gameMsg, _oUserData);

    std::string* pRetStr = new std::string();

    int pid = gameMsg.enMsgType;
    std::string content = gameMsg.serialize();
    int length = content.size();

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
