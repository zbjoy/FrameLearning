#pragma once

class IZinxMsg
{
public:
	IZinxMsg() {};
	virtual ~IZinxMsg() {};
};

class IZinxHandler
{
public:
	IZinxHandler();
	virtual ~IZinxHandler();
	void Handle(IZinxMsg* _inputMsg);
	virtual IZinxMsg* Internel_Handle(IZinxMsg* _inputMsg) = 0;
	virtual IZinxHandler* GetNext(IZinxMsg* _next_input) = 0;
};

