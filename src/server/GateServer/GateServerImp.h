//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GateServerImp.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 网关服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GATE_SERVER_IMP_H__
#define __GATE_SERVER_IMP_H__

#pragma once

#include "gamedbpacket.h"
#include "gamepacket.h"
#include "gatepacket.h"
#include "loginpacket.h"
#include "servermap.h"
#include "serversort.h"
#include "ServerConfig.h"
#include "CommonServer.h"
#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateServer : 网关服务器
// 主要任务 :
// 1. 维护与客户端的连接
// 2. 需要专门的事务处理队列来进行, 高并发!!!
class CGateServer : public ICommonServer, public CPAKHandler
{
public:
	CGateServer(void);
	virtual ~CGateServer(void);
	// CTRefCount
	//virtual Long   AddRef(void) OVERRIDE;
	virtual Long   Release(void) OVERRIDE;
	// CComponent
	// Command to set param value
	virtual UInt   Command(PCXStr pszCMD, uintptr_t utParam) OVERRIDE;
	virtual UInt   Update(void) OVERRIDE;

	virtual UInt   Init(CEventHandler& EventHandlerRef) OVERRIDE;
	virtual void   Exit(void) OVERRIDE;

	virtual bool   Start(void) OVERRIDE;
	virtual bool   Pause(bool bPause = true) OVERRIDE;
	virtual void   Stop(void) OVERRIDE;

	virtual bool   OnShareRoutine(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) OVERRIDE;//
	virtual bool   OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) OVERRIDE;
	virtual bool   OnShareRoutine(Int nEvent, CStream& Stream, LLong llParam = 0) OVERRIDE;//
	// CPAKHandler
	virtual bool   OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp) OVERRIDE;
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	// ...
	CEventHandler* GetUIHandler(void);
	CServerConfig* GetConfig(void);
	ICommonServer* GetShareGameDBServer(void);
	ICommonServer* GetShareGameServer(void);
	KeyRef         GetConnectGameDB(void);
	KeyRef         GetConnectGame(void);
	KeyRef         GetUDPService(void);
	CFileLog&      GetFileLog(void);
	CNetworkPtr&   GetNetworkPtr(void);
	GATE_ADDR&     GetGateAddr(void);
private:
	CGateServer(const CGateServer&);
	CGateServer& operator=(const CGateServer&);
	// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	// 初始化配置
	bool  InitLoadConfig(void);

	// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	// 
	void  ExitUnloadConfig(void);

	// 运行创建连接游戏DB服务器的连接对象
	bool  StartConnectGameDBServer(void);
	// 运行创建连接游戏服务器的连接对象
	bool  StartConnectGameServer(void);
	// 运行创建UDP监听登陆服务器校验连接对象
	bool  StartUDPService(void);
	// 运行创建TCP监听客户端连接对象
	bool  StartTCPService(void);

	// 停止连接游戏DB服务器
	void  StopConnectGameDBServer(void);
	// 停止连接游戏服务器
	void  StopConnectGameServer(void);
	// 停止UDP监听登陆服务器
	void  StopUDPService(void);
	// 停止TCP监听客户端
	void  StopTCPService(void);

	// 处理游戏DB服务器的信令包
	bool  DispatchGameDBServer(const PacketPtr& PktPtr, KeyRef krSocket);
	// 处理游戏服务器的信令包
	bool  DispatchGameServer(const PacketPtr& PktPtr, KeyRef krSocket);

	// 连接游戏DB服务器
	void  LinkGameDBServer(void);
	// 断连游戏DB服务器
	void  UnlinkGameDBServer(void);
	// 连接游戏服务器
	void  LinkGameServer(void);
	// 断连游戏服务器
	void  UnlinkGameServer(void);

	// 定时检测连接游戏DB服务器的连接对象是否有效
	bool  CheckConnectGameDBServer(void);
	// 定时检测连接游戏服务器的连接对象是否有效
	bool  CheckConnectGameServer(void);
	// 定时检测UDP监听登陆服务器的连接对象是否有效
	bool  CheckUDPService(void);
	// 定时检测TCP监听客户端的连接对象是否有效
	bool  CheckTCPService(void);

	// 同步服务器信息给界面
	bool  SyncServerInfo(void);
private:
	Int                 m_nStatus;          // 服务器状态
	bool                m_bGameDBCnnted;
	bool                m_bGameDBLinked;
	bool                m_bGameCnnted;
	bool                m_bGameLinked;
	CEventHandler*      m_pUIHandler;       // 界面回调接口
	CServerConfig*      m_pConfig;          // 配置对象
	KeyRef              m_krConnectGameDB;  // 内网, 连接游戏DB服务器
	KeyRef              m_krConnectGame;    // 内网, 连接游戏服务器
	KeyRef              m_krUDPService;     // 内网, 监听登陆服务器
	KeyRef              m_krTCPService;     // 外网, 监听客户端
	ICommonServer*      m_pShareGameDBSvr;
	ICommonServer*      m_pShareGameSvr;
	CFileLog            m_FileLog;          // 简单文本日志
	CNetworkPtr         m_NetworkPtr;       // 网络对象
	CCommonRoutinePtr   m_GateRoutine;      //
	GATE_ADDR           m_ServerInfo;       // 本服务器信息 
};

INLINE CGateServer::CGateServer(void)
: m_nStatus(STATUSC_NONE)
, m_bGameDBCnnted(false)
, m_bGameDBLinked(false)
, m_bGameCnnted(false)
, m_bGameLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectGameDB(nullptr)
, m_krConnectGame(nullptr)
, m_krUDPService(nullptr)
, m_krTCPService(nullptr)
, m_pShareGameDBSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GateServer)
{
}

INLINE CGateServer::~CGateServer(void)
{
}

INLINE CGateServer::CGateServer(const CGateServer&)
: m_nStatus(STATUSC_NONE)
, m_bGameDBCnnted(false)
, m_bGameDBLinked(false)
, m_bGameCnnted(false)
, m_bGameLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectGameDB(nullptr)
, m_krConnectGame(nullptr)
, m_krUDPService(nullptr)
, m_krTCPService(nullptr)
, m_pShareGameDBSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GateServer)
{
}

INLINE CGateServer& CGateServer::operator=(const CGateServer&)
{
	return (*this);
}

INLINE Long CGateServer::Release(void)
{
	Long lRef = m_Counter.Reset();
	if (lRef == 0) {
		MDELETE this;
	}
	return lRef;
}

INLINE bool CGateServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGateServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE CEventHandler* CGateServer::GetUIHandler(void)
{
	return m_pUIHandler;
}

INLINE CServerConfig* CGateServer::GetConfig(void)
{
	return m_pConfig;
}

INLINE ICommonServer* CGateServer::GetShareGameDBServer(void)
{
	return m_pShareGameDBSvr;
}

INLINE ICommonServer* CGateServer::GetShareGameServer(void)
{
	return m_pShareGameSvr;
}

INLINE KeyRef CGateServer::GetConnectGameDB(void)
{
	return m_krConnectGameDB;
}

INLINE KeyRef CGateServer::GetConnectGame(void)
{
	return m_krConnectGame;
}

INLINE KeyRef CGateServer::GetUDPService(void)
{
	return m_krUDPService;
}

INLINE CFileLog& CGateServer::GetFileLog(void)
{
	return m_FileLog;
}

INLINE CNetworkPtr& CGateServer::GetNetworkPtr(void)
{
	return m_NetworkPtr;
}

INLINE GATE_ADDR& CGateServer::GetGateAddr(void)
{
	return m_ServerInfo;
}

#endif // __GATE_SERVER_IMP_H__
