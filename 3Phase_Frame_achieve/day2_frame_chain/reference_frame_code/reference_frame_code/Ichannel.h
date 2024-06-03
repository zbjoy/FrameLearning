#pragma once
#include <iostream>
#include "IZinxHandler.h"

class sysIODicMsg : public IZinxMsg
{
public:
	enum IO_DIC {
		IO_IN, IO_OUT
	} m_DIC;

	sysIODicMsg(IO_DIC _dic) : m_DIC(_dic) {}
};

class ByteMsg : public sysIODicMsg
{
public:
	std::string content;
	ByteMsg(std::string _content, sysIODicMsg& _iodic) : content(_content), sysIODicMsg(_iodic.m_DIC) {}
};

class Ichannel : public IZinxHandler
{
public:
	Ichannel();
	virtual ~Ichannel();
	virtual bool Init() = 0;
	virtual void Fini() = 0;
	virtual bool ReadFd(std::string& _input) = 0;

	virtual bool WriteFd(std::string& _output) = 0;
	virtual int GetFd() = 0;

	void flushout();
	virtual IZinxHandler* GetInputNextStage(ByteMsg* _byte) = 0;

	// 通过 IZinxHandler 继承
	IZinxMsg* Internel_Handle(IZinxMsg* _inputMsg) override;
	IZinxHandler* GetNext(IZinxMsg* _next_input) override;

private:
	std::string m_buffer;

};

