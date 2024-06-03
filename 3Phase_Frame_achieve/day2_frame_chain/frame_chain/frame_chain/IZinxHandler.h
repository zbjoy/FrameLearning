#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "Ichannel.h"
#include "IMessage.h"

class IZinxHandler
{
public:
	Ichannel* pNext = nullptr;
	IZinxHandler() {};
	~IZinxHandler() {};

	void Handler(IMessage* _input);

	virtual IMessage* data_handle(IMessage* _input) = 0;
	virtual IZinxHandler* get_next() = 0;

};

