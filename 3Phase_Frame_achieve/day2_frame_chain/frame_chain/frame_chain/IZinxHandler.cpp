#include "IZinxHandler.h"

void IZinxHandler::Handler(IMessage* _input)
{
	/* 处理数据 */
	IMessage* pMsg = data_handle(_input);

	if (pMsg != nullptr)
	{
		/* 调用下一个handler */
		IZinxHandler* pNext = get_next();

		if (pNext != nullptr)
		{
			pNext->Handler(pMsg);
		}

		if (pMsg != nullptr)
		{
			delete pMsg;
			pMsg = nullptr;
		}
	}
}
