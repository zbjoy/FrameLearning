#include "GameProtocol.h"
#include "GameMsg.h"
#include "GameChannel.h"
#include "GameRole.h"
#include <iostream>
#include "msg.pb.h"

using namespace std;

UserData* GameProtocol::raw2request(std::string _szInput)
{
    // cout << "MyDebug: " << _szInput << endl;
    cout << "MyDebug: " << endl;

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

		/* 如果是大端字节序是不是就应该右移为高位??? */
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
			/* 测试代码 */
			cout << "当前长度为" << m_length << " ";
			cout << "szLast.size() : " << szLast.size() << endl;
			cout << "所发字节不全, 暂存下次再试" << endl;
			break;
		}

		/* 构造一条用户请求 */
		GameMsg* pMsg = new GameMsg((GameMsg::MSG_TYPE)id, szLast.substr(8, m_length));
		pRet->m_msg_list.push_back(pMsg);

		/* 弹出已经处理成功的报文 */
		cout << "处理成功, 弹出" << 8 + m_length << "大小" << endl;
		szLast.erase(0, 8 + m_length);

	}

	/* 测试代码 */
	
		for (auto single : pRet->m_msg_list)
		{
			cout << "google std debug:" << endl;
			cout << single->pMsg->Utf8DebugString() << endl;
		}
	

	/* 测试 */
	/*
		pb::Talk* pmsg = new pb::Talk();
		pmsg->set_content("hello");
		GameMsg* pGameMsg = new GameMsg(GameMsg::MSG_TYPE_CHAT_CONTENT, pmsg);
		ZinxKernel::Zinx_SendOut(*pGameMsg, *this);
	*/
	return pRet;
}

std::string* GameProtocol::response2raw(UserData& _oUserData)
{
	/*  根本看不懂  */
	int iLength = 0;
	int id = 0;
	std::string MsgContent;

	GET_REF2DATA(GameMsg, oOutput, _oUserData);
	id = oOutput.enMsgType;
	MsgContent = oOutput.serialize();
	iLength = MsgContent.size();

	auto pret = new std::string();

	/* 好像不 & 0xFF 也对 */
	pret->push_back((iLength >> 0) & 0xFF);
	pret->push_back((iLength >> 8) & 0xFF);
	pret->push_back((iLength >> 16) & 0xFF);
	pret->push_back((iLength >> 24) & 0xFF);

	pret->push_back((id >> 0) & 0xFF);
	pret->push_back((id >> 8) & 0xFF);
	pret->push_back((id >> 16) & 0xFF);
	pret->push_back((id >> 24) & 0xFF);
	//pret[1] = (iLength >> 8) & 0xFF;
	//pret[2] = (iLength >> 16) & 0xFF;
	//pret[3] = (iLength >> 24) & 0xFF;

	//pret[4] = (id >> 0) & 0xFF;
	//pret[5] = (id>> 8) & 0xFF;
	//pret[6] = (id >> 16) & 0xFF;
	//pret[7] = (id >> 24) & 0xFF;

	pret->append(MsgContent);
	return pret;
}

Irole* GameProtocol::GetMsgProcessor(UserDataMsg& _oUserDataMsg)
{
	return m_role;
}

/* 返回数据发送的通道 */
Ichannel* GameProtocol::GetMsgSender(BytesMsg& _oBytes)
{
	/* 要包含头文件: "GameChannel.h" */
	return m_channel;
}
