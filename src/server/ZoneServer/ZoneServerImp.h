//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ZoneServerImp.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 地图服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __ZONE_SERVER_IMP_H__
#define __ZONE_SERVER_IMP_H__

#pragma once

#include "zonepacket.h"
#include "servermap.h"
#include "ServerConfig.h"
#include "CommonServer.h"
#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 地图服务器实现
/// - 主要任务 :
/// -# 处理客户端的游戏事务
class CZoneServer : public ICommonServer, public CPAKHandler
{
public:
	CZoneServer(void);
	virtual ~CZoneServer(void);
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
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;//
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;//
	// ...
	CEventHandler* GetUIHandler(void);
	CServerConfig* GetConfig(void);
	ICommonServer* GetShareGameServer(void);
	CFileLog&      GetFileLog(void);
	CNetworkPtr&   GetNetworkPtr(void);
private:
	CZoneServer(const CZoneServer&);
	CZoneServer& operator=(const CZoneServer&);
	/// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	/// 初始化配置
	bool  InitLoadConfig(void);

	/// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	/// 清除配置
	void  ExitUnloadConfig(void);

	/// 创建连接游戏服务器对象
	bool  StartConnectGameServer(void);

	/// 停止连接游戏服务器
	void  StopConnectGameServer(void);

	/// 向游戏服务器注册
	void  LinkGameServer(void);
	/// 向游戏服务器注销
	void  UnlinkGameServer(void);

	/// 定时检测连接游戏服务器对象是否有效
	bool  CheckConnectGameServer(void);

	/// 同步服务器信息给界面
	bool  SyncServerInfo(void);
private:
	Int                    m_nStatus;         ///< 服务器状态
	bool                   m_bGameCnnted;     ///< 是否连接游戏服务器完成
	bool                   m_bGameLinked;     ///< 是否向游戏服务器注册完成
	CEventHandler*         m_pUIHandler;      ///< 界面回调接口
	CServerConfig*         m_pConfig;         ///< 配置对象
	KeyRef                 m_krConnectGame;   ///< 内网, 连接游戏服务器
	ICommonServer*         m_pShareGameSvr;   ///< 同进程游戏服务器
	CFileLog               m_FileLog;         ///< 简单文本日志
	CNetworkPtr            m_NetworkPtr;      ///< 网络对象
	CCommonRoutinePtr      m_ZoneRoutine;     ///< 游戏事务处理 
	ZONE_ADDR              m_ServerInfo;      ///< 本服务器信息, 总进入地图人数-当前地图在线人数-地图维护的地图数量-地图维护的线程数量
};

INLINE CZoneServer::CZoneServer(void)
: m_nStatus(STATUSC_NONE)
, m_bGameCnnted(false)
, m_bGameLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectGame(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::ZoneServer)
{
}

INLINE CZoneServer::~CZoneServer(void)
{
}

INLINE CZoneServer::CZoneServer(const CZoneServer&)
: m_nStatus(STATUSC_NONE)
, m_bGameCnnted(false)
, m_bGameLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectGame(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::ZoneServer)
{
}

INLINE CZoneServer& CZoneServer::operator=(const CZoneServer&)
{
	return (*this);
}

INLINE bool CZoneServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CZoneServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE bool CZoneServer::OnUdpDispatch(const PacketPtr&, PUDP_PARAM)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	return false;
}

INLINE bool CZoneServer::OnTcpAccept(KeyRef, KeyRef)
{
	return false;
}

INLINE bool CZoneServer::OnUdpClose(KeyRef, LLong)
{
	return false;
}

INLINE CEventHandler* CZoneServer::GetUIHandler(void)
{
	return m_pUIHandler;
}

INLINE CServerConfig* CZoneServer::GetConfig(void)
{
	return m_pConfig;
}

INLINE ICommonServer* CZoneServer::GetShareGameServer(void)
{
	return m_pShareGameSvr;
}

INLINE CFileLog& CZoneServer::GetFileLog(void)
{
	return m_FileLog;
}

INLINE CNetworkPtr& CZoneServer::GetNetworkPtr(void)
{
	return m_NetworkPtr;
}

#endif // __ZONE_SERVER_IMP_H__
