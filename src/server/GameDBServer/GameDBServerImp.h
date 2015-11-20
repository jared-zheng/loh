//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GameDBServerImp.h                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏DB服务器管理实现                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAMEDB_SERVER_IMP_H__
#define __GAMEDB_SERVER_IMP_H__

#pragma once

#include "gamedbpacket.h"
#include "gamepacket.h"
#include "gatepacket.h"
#include "servermap.h"
#include "ServerConfig.h"
#include "CommonServer.h"
#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBServer : 游戏DB服务器
// 主要任务 :
// 1. 角色相关的数据库操作
// 2. 将热门账号的角色启动的时候缓存到内存
// 3. 事务和数据库相关, 需要专门的事务处理队列来进行
class CGameDBServer : public ICommonServer, public CPAKHandler
{
public:
	enum INFO_INDEX 
	{
		INFOI_GAMEDB,
		INFOI_GAME,
		INFOI_GATE,
		INFOI_MAX,
	};
public:
	CGameDBServer(void);
	virtual ~CGameDBServer(void);
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
	ICommonServer* GetShareGameServer(void);
	ICommonServer* GetShareGateServer(void);
	CFileLog&      GetFileLog(void);
	CNetworkPtr&   GetNetworkPtr(void);
private:
	CGameDBServer(const CGameDBServer&);
	CGameDBServer& operator=(const CGameDBServer&);
	// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	// 初始化配置
	bool  InitLoadConfig(void);

	// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	// 
	void  ExitUnloadConfig(void);

	// 运行创建监听游戏服务器连接的连接对象
	bool  StartListenGameServer(void);
	// 运行创建监听网关服务器连接的连接对象
	bool  StartListenGateServer(void);

	// 停止监听游戏服务器连接
	void  StopListenGameServer(void);
	// 停止监听网关服务器连接
	void  StopListenGateServer(void);

	// 定时检测监听游戏服务器连接的连接对象是否有效
	bool  CheckListenGameServer(void);
	// 定时检测监听网关服务器连接的连接对象是否有效
	bool  CheckListenGateServer(void);

	// 同步服务器信息给界面
	bool  SyncServerInfo(void);

	// 同进程服务器处理
	bool  OnShareLink(CEventBase& EventRef, LLong llParam);
	bool  OnShareUpdate(CEventBase& EventRef, LLong llParam);
	// 服务器处理
	bool  OnServerLink(CPAKLink* pLink, KeyRef krSocket);
	bool  OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket);
	bool  OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket);

	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerLink(CPAKLink* pLink, DataRef drServer, MAP_TYPE& MapRef);

	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer, MAP_TYPE& MapRef);

	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerUnlink(KeyRef krSocket, MAP_TYPE& MapRef);

private:
	Int                    m_nStatus;         // 服务器状态
	CEventHandler*         m_pUIHandler;      // 界面回调接口
	CServerConfig*         m_pConfig;         // 配置对象
	KeyRef                 m_krListenGame;    // 内网, 监听游戏服务器
	KeyRef                 m_krListenGate;    // 内网, 监听网关服务器
	ICommonServer*         m_pShareGameSvr;
	ICommonServer*         m_pShareGateSvr;
	CFileLog               m_FileLog;         // 简单文本日志
	CNetworkPtr            m_NetworkPtr;      // 网络对象
	CCommonRoutinePtr      m_GameDBRoutine;   // 
	SERVER_INFO            m_ServerInfo[INFOI_MAX]; // gamedb, game, gate
	SVR_GAME_MAP           m_GameSvrMap;      // 游戏服务器信息
	SVR_GATE_MAP           m_GateSvrMap;      // 网关服务器信息  
};

INLINE CGameDBServer::CGameDBServer(void)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenGame(nullptr)
, m_krListenGate(nullptr)
, m_pShareGameSvr(nullptr)
, m_pShareGateSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GameDBServer)
{
}

INLINE CGameDBServer::~CGameDBServer(void)
{
}

INLINE CGameDBServer::CGameDBServer(const CGameDBServer&)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenGame(nullptr)
, m_krListenGate(nullptr)
, m_pShareGameSvr(nullptr)
, m_pShareGateSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GameDBServer)
{
}

INLINE CGameDBServer& CGameDBServer::operator=(const CGameDBServer&)
{
	return (*this);
}

INLINE bool CGameDBServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGameDBServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE bool CGameDBServer::OnUdpDispatch(const PacketPtr&, PUDP_PARAM)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	return false;
}

INLINE bool CGameDBServer::OnTcpConnect(UInt, KeyRef)
{
	return false;
}

INLINE bool CGameDBServer::OnUdpClose(KeyRef, LLong)
{
	return false;
}

INLINE CEventHandler* CGameDBServer::GetUIHandler(void)
{
	return m_pUIHandler;
}

INLINE CServerConfig* CGameDBServer::GetConfig(void)
{
	return m_pConfig;
}

INLINE ICommonServer* CGameDBServer::GetShareGameServer(void)
{
	return m_pShareGameSvr;
}

INLINE ICommonServer* CGameDBServer::GetShareGateServer(void)
{
	return m_pShareGateSvr;
}

INLINE CFileLog& CGameDBServer::GetFileLog(void)
{
	return m_FileLog;
}

INLINE CNetworkPtr& CGameDBServer::GetNetworkPtr(void)
{
	return m_NetworkPtr;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameDBServer::INFO_INDEX InfoIndex>
INLINE bool CGameDBServer::ServerLink(CPAKLink* pLink, DataRef drServer, MAP_TYPE& MapRef)
{
	PINDEX index = nullptr;
	// 1.获取信息
	MAP_TYPE::SVR_PAIR Pair;
	Pair.drKey = drServer;
	Pair.Value.Copy(pLink->GetServerData());
	assert(Pair.Value.usStatus == STATUSU_LINK);
	assert(pLink->CheckStream());
	Pair.Value.Addr(pLink->GetStream());
	{
		CSyncLockWaitScope scope(MapRef.GetLock());
		assert(MapRef.Find(drServer) == nullptr);
		index = MapRef.Add(drServer, Pair.Value);
	}
	if (index != nullptr) { // 2.更新界面
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&Pair), DataIndex);
		m_ServerInfo[InfoIndex].Incr(pLink->GetServerData());
		return true;
	}
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameDBServer::INFO_INDEX InfoIndex>
INLINE bool CGameDBServer::ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer, MAP_TYPE& MapRef)
{
	MAP_TYPE::SVR_PAIR Pair;
	Pair.drKey = drServer;
	{
		CSyncLockWaitScope scope(MapRef.GetLock());
		MAP_TYPE::SVR_MAP_PAIR* pPair = MapRef.Find(drServer);
		if ((pPair != nullptr) &&
			(pPair->m_V.usStatus > STATUSU_NONE) &&
			(pPair->m_V.usStatus < STATUSU_UNLINK)) {
			Pair.Value.Copy(pPair->m_V);
			pPair->m_V.Copy(pUpdate->GetServerData());
		}
	}
	if (Pair.Value.usStatus > STATUSU_NONE) {
		m_ServerInfo[InfoIndex].Diff(pUpdate->GetServerData(), Pair.Value);
		Pair.Value.Copy(pUpdate->GetServerData());
		m_pUIHandler->OnHandle(PAK_EVENT_UPDATE, reinterpret_cast<uintptr_t>(&Pair), DataIndex);// 更新界面
		return true;
	}
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameDBServer::INFO_INDEX InfoIndex>
INLINE bool CGameDBServer::ServerUnlink(KeyRef krSocket, MAP_TYPE& MapRef)
{
	// 2.更新界面
	m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, reinterpret_cast<uintptr_t>(krSocket), DataIndex);

	SERVER_DATA sd = { 0 };
	{
		CSyncLockWaitScope scope(MapRef.GetLock());
		MAP_TYPE::SVR_MAP_PAIR* pPair = MapRef.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			sd = pPair->m_V;
			MapRef.RemoveAt(reinterpret_cast<PINDEX>(pPair));
		}
	}
	if (sd.usStatus > STATUSU_NONE) {
		m_ServerInfo[InfoIndex].Decr(sd);
		return true;
	}
	return false;
}
#endif // __GAMEDB_SERVER_IMP_H__
