#include "GameProtocol.h"
#include "GameChannel.h"
#include "GameMsg.h"
#include "GameRole.h"

GameProtocol::~GameProtocol()
{
    ZinxKernel::Zinx_Del_Role(*m_role);
    delete m_role;
}

UserData* GameProtocol::raw2request(std::string _szInput)
{
    // GameMsg* pRet = nullptr;
    MultiMsg* pRet = new MultiMsg();
    /* 使用滑动窗口接受消息 */
    szLast.append(_szInput);

    while (true)
    {
		if (szLast.size() < 8)
		{
            break;
		}

		int length = 0;
		int id = 0;
        
        length |= szLast[0] << 0;
        length |= szLast[1] << 8;
        length |= szLast[2] << 16;
        length |= szLast[3] << 24;

        if (szLast.size() - 8 < length)
        {
            /* 调试代码 */
            std::cout << "消息不足" << std::endl;
            break;
        }

        id |= szLast[4] << 0;
        id |= szLast[5] << 8;
        id |= szLast[6] << 16;
        id |= szLast[7] << 24;

        GameMsg* tmp = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, length));
        pRet->m_msg_list.push_back(tmp);

        szLast.erase(0, 8 + length);
    }
    
    return pRet;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    GET_REF2DATA(GameMsg, gameMsg, _oUserData);

    int length = 0;
    int id = 0;

    std::string MsgContent = gameMsg.serialize();
    id = gameMsg.enMsgType;
    length = MsgContent.size();

    /* 调试代码 */
    std::cout << "GameProtocol::response2row()" << std::endl;
    std::cout << "length: " << length << std::endl;
    std::cout << "id: " << id << std::endl;
    std::cout << gameMsg.pMsg->Utf8DebugString() << std::endl;

    std::string* strRet = new std::string();

    strRet->push_back((length >> 0) & 0xFF);
    strRet->push_back((length >> 8) & 0xFF);
    strRet->push_back((length >> 16) & 0xFF);
    strRet->push_back((length >> 24) & 0xFF);

    strRet->push_back((id >> 0) & 0xFF);
    strRet->push_back((id >> 8) & 0xFF);
    strRet->push_back((id >> 16) & 0xFF);
    strRet->push_back((id >> 24) & 0xFF);

    strRet->append(MsgContent);

    return strRet;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return m_role;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return m_channel;
}
