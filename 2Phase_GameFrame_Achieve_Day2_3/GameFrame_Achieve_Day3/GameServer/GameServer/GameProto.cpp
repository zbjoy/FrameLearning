#include "GameProto.h"
#include "GameMsg.h"

UserData* GameProto::raw2request(std::string _szInput)
{
	/* 滑动窗口 */
    szLast.append(_szInput);
	MultiMsg* pMsg = new MultiMsg();

    while (true)
    {
		int iLength = 0;
		int iPid = 0;
		if (szLast.size() < 8)
		{
			break;
		}

		/* 调试代码 */
		std::cout << "szLast.size() : " << szLast.size() << std::endl;

		//iLength |= _szInput[0] << 0;
		//iLength |= _szInput[1] << 8;
		//iLength |= _szInput[2] << 16;
		//iLength |= _szInput[3] << 24;
		iLength |= szLast[0] << 0;
		iLength |= szLast[1] << 8;
		iLength |= szLast[2] << 16;
		iLength |= szLast[3] << 24;

		if (iLength > szLast.size() - 8)
		{
			/* 调试代码 */
			std::cout << "长度不够" << std::endl;
			std::cout << "iLength为: " << iLength << std::endl;
			std::cout << "当前长度为(szLast.size() - 8): " << szLast.size() - 8 << std::endl;
			break;
		}

		//iPid |= _szInput[4] << 0;
		//iPid |= _szInput[5] << 8;
		//iPid |= _szInput[6] << 16;
		//iPid |= _szInput[7] << 24;
		iPid |= szLast[4] << 0;
		iPid |= szLast[5] << 8;
		iPid |= szLast[6] << 16;
		iPid |= szLast[7] << 24;

		/* 调试代码 */
		std::cout << "添加一个" << std::endl;
		std::string strstream = szLast.substr(8, iLength);
		szLast.erase(0, 8 + iLength);
		GameMsg* pGameMsg = new GameMsg((GameMsg::MSG_TYPE)iPid, strstream);
		pMsg->m_msgs.push_back(pGameMsg);
	}
    // GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)iPid, strstream);

	/* 调试代码 */
	for (auto single : pMsg->m_msgs)
	{
		std::cout << "enMsgType: " << single->enMsgType << std::endl;
		std::cout << single->pMsg->Utf8DebugString();
	}
	return pMsg;
}

std::string* GameProto::response2raw(UserData& _oUserData)
{
    return nullptr;
}

Irole* GameProto::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return nullptr;
}

Ichannel* GameProto::GetMsgSender(BytesMsg& _oBytes)
{
    return nullptr;
}
