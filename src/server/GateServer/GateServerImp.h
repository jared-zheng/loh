//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GateServerImp.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器实现                                     //
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
/// 网关服务器实现
/// - 主要任务 :
/// -# 维护与客户端的连接
/// -# 需要专门的事务处理队列来进行, 高并发!!!
/// -# 可以让网关连接聊天服务器
class CGateServer : public ICommonServer, public CPAKHandler
{
public:
	CGateServer(void);
	virtual ~CGateServer(void);
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
	Int            GetGameId(void);
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
	/// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	/// 初始化配置
	bool  InitLoadConfig(void);

	/// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	/// 清除配置
	void  ExitUnloadConfig(void);

	/// 创建连接游戏DB服务器对象
	bool  StartConnectGameDBServer(void);
	/// 创建连接游戏服务器对象
	bool  StartConnectGameServer(void);
	/// 创建UDP监听登陆服务器对象
	bool  StartUDPService(void);
	/// 创建TCP监听客户端对象
	bool  StartTCPService(void);

	/// 停止连接游戏DB服务器
	void  StopConnectGameDBServer(void);
	/// 停止连接游戏服务器
	void  StopConnectGameServer(void);
	/// 停止UDP监听登陆服务器
	void  StopUDPService(void);
	/// 停止TCP监听客户端
	void  StopTCPService(void);

	/// 处理游戏DB服务器的信令包
	bool  DispatchGameDBServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理游戏服务器的信令包
	bool  DispatchGameServer(const PacketPtr& PktPtr, KeyRef krSocket);

	/// 向游戏DB服务器注册
	void  LinkGameDBServer(void);
	/// 向游戏DB服务器注销
	void  UnlinkGameDBServer(void);
	/// 向游戏服务器注册
	void  LinkGameServer(void);
	/// 向游戏服务器注销
	void  UnlinkGameServer(void);

	/// 定时检测连接游戏DB服务器对象是否有效
	bool  CheckConnectGameDBServer(void);
	/// 定时检测连接游戏服务器对象是否有效
	bool  CheckConnectGameServer(void);
	/// 定时检测UDP监听登陆服务器对象是否有效
	bool  CheckUDPService(void);
	/// 定时检测TCP监听客户端对象是否有效
	bool  CheckTCPService(void);

	// 同步服务器信息给界面
	bool  SyncServerInfo(void);
private:
	Int                 m_nStatus;          ///< 服务器状态
	Int                 m_nGameId;          ///< 连接的游戏服务器Id
	bool                m_bGameDBCnnted;    ///< 是否连接游戏DB服务器完成
	bool                m_bGameDBLinked;    ///< 是否向游戏DB服务器注册完成
	bool                m_bGameCnnted;      ///< 是否连接游戏服务器完成
	bool                m_bGameLinked;      ///< 是否向游戏服务器注册完成
	CEventHandler*      m_pUIHandler;       ///< 界面回调接口
	CServerConfig*      m_pConfig;          ///< 配置对象
	KeyRef              m_krConnectGameDB;  ///< 内网, 连接游戏DB服务器
	KeyRef              m_krConnectGame;    ///< 内网, 连接游戏服务器
	KeyRef              m_krUDPService;     ///< 内网, 监听登陆服务器
	KeyRef              m_krTCPService;     ///< 外网, 监听客户端
	ICommonServer*      m_pShareGameDBSvr;  ///< 同进程游戏DB服务器
	ICommonServer*      m_pShareGameSvr;    ///< 同进程游戏服务器
	CFileLog            m_FileLog;          ///< 简单文本日志
	CNetworkPtr         m_NetworkPtr;       ///< 网络对象
	CCommonRoutinePtr   m_GateRoutine;      ///< 游戏事务处理 
	GATE_ADDR           m_ServerInfo;       ///< 本服务器信息 
};

INLINE CGateServer::CGateServer(void)
: m_nStatus(STATUSC_NONE)
, m_nGameId(0)
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
, m_nGameId(0)
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

INLINE bool CGateServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGateServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE Int CGateServer::GetGameId(void)
{
	return m_nGameId;
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
