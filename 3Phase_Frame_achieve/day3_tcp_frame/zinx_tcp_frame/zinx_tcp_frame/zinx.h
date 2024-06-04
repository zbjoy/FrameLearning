#pragma once

#ifndef _ZINX_H_
#define _ZINX_H_

#include <iostream>
#include <string>
#include <list>

/* 定义动态类型转换后的引用类型, 失败返回nullptr */
#define GET_REF2DATA(type, ref, orig) type * pref = dynamic_cast<type*>(&orig); if (nullptr == pref) {return nullptr;} type& ref = dynamic_cast<type&>(orig)

/* 所有hadnler实例之间传递的消息父类 */
class IZinxMsg
{
public:
	IZinxMsg() {}
	virtual ~IZinxMsg() {}
};

/* 处理handler抽象类, 重写处理信息方法和获取下一个处理者方法 */
class AZinxHandler
{
public:
	AZinxHandler() {}
	virtual ~AZinxHandler() {}
	void Handle(IZinxMsg& _oInput);

protected:
	/* 信息处理函数, 开发者重写该函数实现信息处理, 当有需要一个环节继续处理时, 应该创建新的信息对象(堆对象) 并返回指针 */
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput) = 0;

	/* 获取下一个处理环节函数, 开发者重写该函数, 可以指定当前的处理环节结束后下一个环节是什么, 通过参数信息对象, 可以针对不同情况分别处理 */
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg) = 0;

};

/* 系统信息类, 只包含当前信息的方向 */
class SysIOReadyMsg : public IZinxMsg
{
public:
	enum IO_DIC
	{
		IN, OUT
	} m_emIODic;

	SysIOReadyMsg(IO_DIC _dic) : m_emIODic(_dic) {}
};

/* 字节流信息, 包含string封装的字节流, string封装的通道信息和父类中的方向 */
class BytesMsg : public SysIOReadyMsg
{
public:
	BytesMsg(SysIOReadyMsg& _base) : SysIOReadyMsg(_base.m_emIODic) {}
	std::string szInfo;
	std::string szData;
};

/* 纯用户数据信息, 需要开发者继承后添加业务相关字段 */
class UserData
{
public:
	UserData() {}
	virtual ~UserData() {}
};

/* 用户信息类(对纯用户数据类的封装), 继承自字节流类, 拥有一个纯用户数据对象 */
class UserDataMsg : public BytesMsg
{
	UserDataMsg(BytesMsg& _base) : BytesMsg(_base) {}
	UserData* poUserData = nullptr;
	virtual ~UserDataMsg()
	{
		if (poUserData != nullptr)
		{
			delete poUserData;
		}
	}
};

/*角色类，派生自基础处理者类，提供初始化，去初始化，处理信息，设置、清除下一个处理者的虚函数。
角色类是用于处理业务数据的，开发者应该将一个纯业务流程由一个或多个角色类的子类处理
角色对象应该被添加到zinxkernel中*/

/* ------------------ */
// to do
/* ------------------ */



class Ichannel;
/* 协议类, 派生自基础处理者类, 提供基于通信协议的原始数据和业务数据之间的转换
	一般的, 协议对象会被指定为通道对象的下一个处理环节*/

/* ------------------ */
// to do
/* ------------------ */



/* 通道类, 派生自基础处理类, 提供基于系统调用的数据收发功能
	一般的, 用户应该根据处理的文件(信息源)不同而创建通道类的子类或选用合适的实用类(已经提供的通道类子类)来完成系统级的文件IO*/
class Ichannel : public AZinxHandler
{
public:
	Ichannel() {}
	virtual ~Ichannel() {}

	/* 通道初始化函数, 一般的, 开发者应该重写这个函数实现打开文件和一定的参数配置 
		该函数会在通道对象添加到ZinxKernel时被调用*/
	virtual bool Init() = 0;

	/* 读取数据, 开发者应该根据目标文件不同, 重写这个函数, 以实现将fd中的数据读取到参数_string中
		该函数会在数据源所对应的文件有数据到达时调用*/
	virtual bool ReadFd(std::string& _input) = 0;

	/* 写出数据, 开发者应该将写出的操作通过重载该函数实现
		该函数会在调用ZinxKernel::Sendout函数后被调用(通常不是直接调用而是通过IO多路复用的反压机制调用)*/
	virtual bool WriteFd(std::string& _output) = 0;

	/* 通道去初始化, 开发者应该在该函数回收相关资源
		该函数会在通道对象从ZinxKernel中摘除时调用*/
	virtual void Fini() = 0;

	/* 获取文件描述符函数, 开发者应该在该函数返回当前关心的文件,
		一般的, 开发者应该在派生类中定义属性用来记录数据来记录当前IO所用的文件, 在此函数中只是返回该属性*/
	virtual int GetFd() = 0;

	void FlushOut();
	bool HasOutput()
	{
		return false == m_WriteBuffer.empty();
	}

	std::string Conver2Printable(std::string& _szData);

	/* 设置通道关闭函数, 若在数据处理或业务处理时, 判断当前应该被关闭时, 可以调用该函数 */
	void SetChannelClose() { m_NeedClose = true; }
	bool ChannelNeedClose() { return m_NeedClose; }

	/*获取通道信息函数，开发者可以在该函数中返回跟通道相关的一些特征字符串，方便后续查找和过滤*/
	virtual std::string GetChannelInfo() = 0;

protected:
	/* 获取下一个处理环节, 开发者应该重写该函数, 指定下一个处理环节 
		一般的, 开发者应该在该函数返回一个协议对象, 用来处理读取到的字节流*/
	virtual AZinxHandler* GetInputNextStage(BytesMsg& _oInput) = 0;

	

private:
	virtual IZinxMsg* InternelHandle(IZinxMsg& _oInput);
	virtual AZinxHandler* GetNextHandler(IZinxMsg& _oNextMsg);
	std::list<std::string> m_WriteBuffer;
	bool m_NeedClose = false;

};

/* zinx并发核心, 整个框架既是一个单例, 提供若干静态方法进行操作 */
class ZinxKernel
{
	static void Zinx_SetChannelOut(Ichannel& _oChannel);
	static void Zinx_ClearChannelOut(Ichannel& _oChannel);
	friend class Ichannel;

public:
	/* 初始化, 每个要用到zinx框架的进程应该调用且只需调用一次该函数 */
	static bool ZinxKernelInit();

	/* 去初始化, 在进程退出前建议调用该函数, 回收相关资源 */
	static void ZinxKernelFini();

	/* 添加通道对象, 要求对象为堆对象, 通道对象只有添加到ZinxKernel后才能被并发处理 */
	static bool Zinx_Add_Channel(Ichannel& _oChannel);

	/* 摘除通道对象, 该函数不会释放通道对象, 需要手动释放 */
	static void Zinx_Del_Channel(Ichannel& _oChannel);

	/* 通过通道的信息获取通道对象, 如果多个通道的信息字符串相同则获得第一个通道 */
	static Ichannel* Zinx_GetChannel_ByInfo(std::string _szInfo);

	/* 添加协议对象, 要求对象为堆对象, 该函数仅有内存管理的作用, 建议开发者将协议对象添加到ZinxKernel存储 */
	// static bool Zinx_Add_Proto(Iprotocol& _oProto);

	/*摘除协议对象，该函数不会释放协议对象，需要调用者手动释放*/
	// static void Zinx_Del_Proto(Iprotocol& _oProto);

	/*添加角色对象，要求对象为堆对象，添加到zinxkernel后的角色对象才能被调用初始化函数*/
	// static bool Zinx_Add_Role(Irole& _oRole);

	/*获取角色对象列表，获取当前zinxkernel中所有的角色对象*/
	// static std::list<Irole*>& Zinx_GetAllRole();

	/*摘除角色对象，该函数不会释放角色对象，需要调用者手动释放*/
	// static void Zinx_Del_Role(Irole& _oRole);

	/* 运行该框架, 该函数运行后会一直循环处理IO数据, 直到Zinx_Exit被调用 */
	static void Zinx_Run();

	/* 向外发送数据, 将参数1指定的用户数据通过参数2指定的协议对象发出 */
	// static void Zinx_Sendout(UserData& _oUserData, Iprotocol& _oProto);

	/* 向外发送数据, 将参数1指定的字节流通过参数2指定的通道发出 */
	static void Zinx_Sendout(std::string& szBytes, Ichannel& _oChannel);
private:
	ZinxKernel();
	~ZinxKernel();
	bool Add_Channel(Ichannel& _oChannel);
	void Del_Channel(Ichannel& _oChannel);
	// bool Add_Proto(Iprotocol& _oProto);
	// void Del_Proto(Iprotocol& _oProto);
	// bool Add_Role(Irole& _oRole);
	// void Del_Role(Irole& _oRole);

	void Run();
	void Sendout(UserData& _oUserData);
	std::list<Ichannel*> m_ChannelList;
	// std::list<Iprotocol*> m_ProtoList;
	// std::list<Irole*> m_RoleList
	int iEpollFd = -1;
	static ZinxKernel* poZinxKernel;
	bool m_need_exit = false;

public:
	/* 停止框架 */
	static void Zinx_Exit();
};

#endif
