#include "GameProtocol.h"
#include "GameRole.h"
#include "GameChannel.h"
#include "GameMsg.h"

UserData* GameProtocol::raw2request(std::string _szInput)
{
    /* 创建一个数据包存所有数据 */
    MultiMsg* pRetMsg = new MultiMsg();
    /* 处理数据 */
    szData.append(_szInput);

    while (true)
    {
		if (szData.size() < 8)
		{
			/* 不足八个字节就不用想, 直接发个空给Role层就好了 */
            break;
		}

        int length = 0;
        int pid = 0;

        for (int i = 0; i < sizeof(int); i++)
        {
            /* 对与char类型进行了整形提升 */
            length |= (szData[i] << (i * 8));
        }

        if (length < szData.size() - 8)
        {
            std::cout << "消息不足" << std::endl;
            break;
        }

        for (int i = sizeof(int); i < 2 * sizeof(int); i++)
        {
            pid |= (szData[i] << ((i - sizeof(int)) * 8));
        }

        GameMsg* temp = new GameMsg((GameMsg::MSG_TYPE)pid, szData.substr(8, length));
        pRetMsg->m_msg_list.push_back(temp);

        /* 弹出已经取出的 */
        szData.erase(0, 8 + length);
	}

    return pRetMsg;
}

/* 取过来的是GameMsg */
std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    GET_REF2DATA(GameMsg, gameMsg, _oUserData);
    std::string content = gameMsg.serialize();

    int length = content.size();
    int id = gameMsg.enMsgType;

    /* 可以不 new 吗 */
    // std::string* pRet = new std::string();
    // std::string retStr;
    std::string* pRetStr = new std::string();
    /* 可以用 push_back 吗 */

    //retStr.push_back(length >> 0);
    //retStr.push_back(length >> 8);
    //retStr.push_back(length >> 16);
    //retStr.push_back(length >> 24);
    pRetStr->push_back(length >> 0);
    pRetStr->push_back(length >> 8);
    pRetStr->push_back(length >> 16);
    pRetStr->push_back(length >> 24);

    //retStr.push_back(id >> 0);
    //retStr.push_back(id >> 8);
    //retStr.push_back(id >> 16);
    //retStr.push_back(id >> 24);
    pRetStr->push_back(id >> 0);
    pRetStr->push_back(id >> 8);
    pRetStr->push_back(id >> 16);
    pRetStr->push_back(id >> 24);

    // retStr.append(content);
    pRetStr->append(content);

    /* 感觉这样就完蛋了 */
    // return &retStr;
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
