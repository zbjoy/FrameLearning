#include "GameProto.h"
#include "GameMsg.h"
#include "GameChannel.h"
#include "GameRole.h"
#include "msg.pb.h"

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
		std::cout << "iPid is : " << iPid << std::endl;
		std::string strstream = szLast.substr(8, iLength);
		GameMsg* pGameMsg = new GameMsg((GameMsg::MSG_TYPE)iPid, strstream);
		pMsg->m_msgs.push_back(pGameMsg);
		szLast.erase(0, 8 + iLength);
	}
    // GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)iPid, strstream);

	/* 调试代码 */
	//for (auto single : pMsg->m_msgs)
	//{
	//	std::cout << "enMsgType: " << single->enMsgType << std::endl;
	//	std::cout << single->pMsg->Utf8DebugString();
	//}
	return pMsg;
}

std::string* GameProto::response2raw(UserData& _oUserData)
{
	GET_REF2DATA(GameMsg, gameMsg, _oUserData);

	/* 调试代码 */
	std::cout << "gameMsg.enMsgType is " << gameMsg.enMsgType << std::endl;
	std::cout << "google debug:" << std::endl;
	std::cout << gameMsg.pMsg->Utf8DebugString() << std::endl;
	std::string strRet = gameMsg.serialize();

	std::string* pRet = new std::string();

	int iLength = 0;
	int iPid = 0;
	iLength = strRet.size();
	iPid = gameMsg.enMsgType;
	//int iLength = strRet.size();
	//int iPid = gameMsg.enMsgType;
	std::cout << "my end debug iLength : " << iLength << std::endl;
	std::cout << "my end debug iPid : " << iPid << std::endl;

	pRet->push_back((iLength >> 0) & 0xFF);
	pRet->push_back((iLength >> 8) & 0xFF);
	pRet->push_back((iLength >> 16) & 0xFF);
	pRet->push_back((iLength >> 24) & 0xFF);

	pRet->push_back((iPid >> 0) && 0xFF);
	pRet->push_back((iPid >> 8) && 0xFF);
	pRet->push_back((iPid >> 16) && 0xFF);
	pRet->push_back((iPid >> 24) && 0xFF);

	pRet->append(strRet);
	
	// return &strRet;
	return pRet;
}

Irole* GameProto::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return m_role;
}

Ichannel* GameProto::GetMsgSender(BytesMsg& _oBytes)
{
	std::string temp = _oBytes.szData;
	int id = 0;
	int length = 0;
	length |= temp[0] << 0;
	length |= temp[1] << 8;
	length |= temp[2] << 16;
	length |= temp[3] << 24;

	id |= temp[4] << 0;
	id |= temp[5] << 8;
	id |= temp[6] << 16;
	id |= temp[3] << 24;

	// google::protobuf::Message* pMsg;

	// pMsg->ParseFromString(temp.substr(8, temp.size() - 8));
	// std::cout << "google haha: " << pMsg->Utf8DebugString();


	std::cout << "length : " << length << std::endl;
	std::cout << "id : " << id << std::endl;
	std::cout << "向channel发送消息" << std::endl;
	return m_channel;
}
