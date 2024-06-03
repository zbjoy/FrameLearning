#include "IZinxHandler.h"

IZinxHandler::IZinxHandler()
{
}

IZinxHandler::~IZinxHandler()
{
}

void IZinxHandler::Handle(IZinxMsg* _inputMsg)
{
	auto pMsg = Internel_Handle(_inputMsg);
	if (pMsg != nullptr)
	{
		IZinxHandler* pNextHandler = GetNext(pMsg);
		if (pNextHandler != nullptr)
		{
			pNextHandler->Handle(pMsg);
		}
		delete pMsg;
	}
}
