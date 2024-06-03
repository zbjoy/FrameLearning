#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "Ichannel.h"

class IZinxHandler
{
public:
	Ichannel* pNext = nullptr;
	IZinxHandler() {};
	~IZinxHandler() {};

	void handle(std::string _input)
	{
		std::string output = Upper(_input);
		pNext->data_sendout(output);
	}

	std::string Upper(std::string _input)
	{
		
		if (_input[0] <= 'z' && _input[0] >= 'a')
		{
			std::transform(_input.begin(), _input.end(), _input.begin(), ::toupper);
		}
		return _input;
	}


};

