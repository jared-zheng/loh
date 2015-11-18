//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : LoginDBServerImp.h                           //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 登陆DB服务器管理实现                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __LOGINDB_SERVER_IMP_H__
#define __LOGINDB_SERVER_IMP_H__

#pragma once

#include "logindbpacket.h"
#include "loginpacket.h"
#include "servermap.h"
#include "ServerConfig.h"
#include "CommonServer.h"
#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginDBServer : 登陆DB服务器
// 主要任务 :
// 1. 账号相关的数据库操作
// 2. 将热门账号启动的时候缓存到内存
// 3. 事务和数据库相关, 需要专门的事务处理队列来进行
class CLoginDBServer : public ICommonServer, public CPAKHandler
{
public:
	CLoginDBServer(void);
	virtual ~CLoginDBServer(void);
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
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;//
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;//
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;//
	// ...
	CEventHandler* GetUIHandler(void);
	CServerConfig* GetConfig(void);
	ICommonServer* GetShareServer(void);
	CFileLog&      GetFileLog(void);
	CNetworkPtr&   GetNetworkPtr(void);
private:
	CLoginDBServer(const CLoginDBServer&);
	CLoginDBServer& operator=(const CLoginDBServer&);
	// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	// 初始化配置
	bool  InitLoadConfig(void);

	// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	// 
	void  ExitUnloadConfig(void);

	// 运行创建监听登陆服务器连接的连接对象
	bool  StartListenLoginServer(void);

	// 停止监听登陆服务器连接
	void  StopListenLoginServer(void);

	// 定时检测监听登陆服务器连接的连接对象是否有效
	bool  CheckListenLoginServer(void);

	// 同步服务器信息给界面
	bool  SyncServerInfo(void);

	// 同进程服务器处理
	bool  OnShareLink(CEventBase& EventRef, LLong llParam);
	bool  OnShareUpdate(CEventBase& EventRef, LLong llParam);
	// 服务器处理
	bool  OnServerLink(CPAKLink* pLink, KeyRef krSocket);
	bool  OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket);
	bool  OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket);

	bool  ServerLink(CPAKLink* pLink, DataRef drServer);
	bool  ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer);
	bool  ServerUnlink(KeyRef krSocket);
private:
	Int                    m_nStatus;         // 服务器状态
	CEventHandler*         m_pUIHandler;      // 界面回调接口
	CServerConfig*         m_pConfig;         // 配置对象
	KeyRef                 m_krListenLogin;   // 内网, 监听登陆服务器
	ICommonServer*         m_pShareLoginSvr;
	CFileLog               m_FileLog;         // 简单文本日志
	CNetworkPtr            m_NetworkPtr;      // 网络对象
	CCommonRoutinePtr      m_LoginDBRoutine;  // 
	SERVER_INFO            m_ServerInfo;      // 本服务器信息
	SVR_LOGIN_MAP          m_LoginSvrMap;     // 登陆服务器信息   
};

INLINE CLoginDBServer::CLoginDBServer(void)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenLogin(nullptr)
, m_pShareLoginSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::LoginDBServer)
{
}

INLINE CLoginDBServer::~CLoginDBServer(void)
{
}

INLINE CLoginDBServer::CLoginDBServer(const CLoginDBServer&)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenLogin(nullptr)
, m_pShareLoginSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::LoginDBServer)
{
}

INLINE CLoginDBServer& CLoginDBServer::operator=(const CLoginDBServer&)
{
	return (*this);
}

INLINE Long CLoginDBServer::Release(void)
{
	Long lRef = m_Counter.Reset();
	if (lRef == 0) {
		MDELETE this;
	}
	return lRef;
}

INLINE bool CLoginDBServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CLoginDBServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE bool CLoginDBServer::OnUdpDispatch(const PacketPtr&, PUDP_PARAM)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	return false;
}

INLINE bool CLoginDBServer::OnTcpConnect(UInt, KeyRef)
{
	return false;
}

INLINE bool CLoginDBServer::OnUdpClose(KeyRef, LLong)
{
	return false;
}

INLINE CEventHandler* CLoginDBServer::GetUIHandler(void)
{
	return m_pUIHandler;
}

INLINE CServerConfig* CLoginDBServer::GetConfig(void)
{
	return m_pConfig;
}

INLINE ICommonServer* CLoginDBServer::GetShareServer(void)
{
	return m_pShareLoginSvr;
}

INLINE CFileLog& CLoginDBServer::GetFileLog(void)
{ 
	return m_FileLog;
}

INLINE CNetworkPtr& CLoginDBServer::GetNetworkPtr(void)
{
	return m_NetworkPtr;
}

INLINE bool CLoginDBServer::ServerLink(CPAKLink* pLink, DataRef drServer)
{
	PINDEX index = nullptr;
	// 1.获取信息
	SVR_LOGIN_MAP::SVR_PAIR Pair;
	Pair.drKey = drServer;
	Pair.Value.Copy(pLink->GetServerData());
	assert(Pair.Value.usStatus == STATUSU_LINK);
	assert(pLink->CheckStream());
	Pair.Value.Addr(pLink->GetStream());
	{
		CSyncLockWaitScope scope(m_LoginSvrMap.GetLock());
		assert(m_LoginSvrMap.Find(drServer) == nullptr);
		index = m_LoginSvrMap.Add(drServer, Pair.Value);
	}
	if (index != nullptr) { // 2.更新界面
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&Pair), DATA_INDEX_LOGIN);
		return true;
	}
	return false;
}

INLINE bool CLoginDBServer::ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer)
{
	SVR_LOGIN_MAP::SVR_PAIR Pair;
	Pair.drKey = drServer;
	Pair.Value.Copy(pUpdate->GetServerData());
	m_pUIHandler->OnHandle(PAK_EVENT_UPDATE, reinterpret_cast<uintptr_t>(&Pair), DATA_INDEX_LOGIN);// 更新界面
	{
		CSyncLockWaitScope scope(m_LoginSvrMap.GetLock());
		SVR_LOGIN_MAP::SVR_MAP_PAIR* pPair = m_LoginSvrMap.Find(drServer);
		if ((pPair != nullptr) &&
			(pPair->m_V.usStatus > STATUSU_NONE) &&
			(pPair->m_V.usStatus < STATUSU_UNLINK)) {
			pPair->m_V.Copy(pUpdate->GetServerData());
			return true;
		}
	}
	return false;
}

INLINE bool CLoginDBServer::ServerUnlink(KeyRef krSocket)
{
	// 1.更新界面
	m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, reinterpret_cast<uintptr_t>(krSocket), DATA_INDEX_LOGIN);
	{
		CSyncLockWaitScope scope(m_LoginSvrMap.GetLock());
		return m_LoginSvrMap.Remove((DataRef)krSocket);
	}
}

#endif // __LOGINDB_SERVER_IMP_H__
