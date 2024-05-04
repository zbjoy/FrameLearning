#include "GameProtocol.h"
#include "GameMsg.h"
#include <iostream>

using namespace std;

UserData* GameProtocol::raw2request(std::string _szInput)
{
    cout << "MyDebug: " << _szInput << endl;

	MultiMsg* pRet = new MultiMsg();
	//UserData* pRet = NULL;
	// GameMsg* pRet = new GameMsg();
	/* 接受并回显 */
	szLast.append(_szInput);
    
    while (1)
    {

		if (szLast.size() < 8)
		{
			break;
		}

		int m_length = 0;

		m_length |= szLast[0] << 0;
		m_length |= szLast[1] << 8;
		m_length |= szLast[2] << 16;
		m_length |= szLast[3] << 24;

		int id = 0;

		id |= szLast[4] << 0;
		id |= szLast[5] << 8;
		id |= szLast[6] << 16;
		id |= szLast[7] << 24;

		cout << "MyDebug_inWhile: " << endl;
		cout << "m_length: " << m_length << endl;
		cout << "id: " << id << endl;

		if (szLast.size() - 8 < m_length)
		{
			break;
		}

		/* 构造一条用户请求 */
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, m_length));
		pRet->m_msg_list.push_back(pMsg);

		/* 弹出已经处理成功的报文 */
		szLast.erase(0, 8 + m_length);

	}

	for (auto single : pRet->m_msg_list)
	{
		cout << "google std debug:" << endl;
		cout << single->pMsg->Utf8DebugString() << endl;
	}
	return pRet;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
    return nullptr;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
    return nullptr;
}

Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
    return nullptr;
}
