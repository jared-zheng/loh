//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ZoneServerImp.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 地图服务器管理实现                                 //
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
// CZoneServer : 地图服务器
// 主要任务 :
// 1. 处理客户端的游戏业务
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
	virtual bool   OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp) OVERRIDE;
	virtual bool   OnTcpAccept(KeyRef krAccept, KeyRef krListen) OVERRIDE;//
	virtual bool   OnTcpConnect(UInt uError, KeyRef krConnect) OVERRIDE;
	virtual bool   OnTcpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
	virtual bool   OnUdpClose(KeyRef krSocket, LLong llLiveData) OVERRIDE;
private:
	CZoneServer(const CZoneServer&);
	CZoneServer& operator=(const CZoneServer&);
	// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	// 初始化配置
	bool  InitLoadConfig(void);

	// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	// 
	void  ExitUnloadConfig(void);

	// 运行创建连接游戏服务器的连接对象
	bool  StartConnectGameServer(void);

	// 停止连接游戏服务器
	void  StopConnectGameServer(void);
	// 连接游戏服务器
	void  LinkGameServer(void);
	// 断连游戏服务器
	void  UnlinkGameServer(void);

	// 定时检测连接游戏服务器的连接对象是否有效
	bool  CheckConnectGameServer(void);

	// 同步服务器信息给界面
	bool  SyncServerInfo(void);
private:
	Int                    m_nStatus;         // 服务器状态
	bool                   m_bGameCnnted;
	bool                   m_bGameLinked;
	CEventHandler*         m_pUIHandler;      // 界面回调接口
	CServerConfig*         m_pConfig;         // 配置对象
	KeyRef                 m_krConnectGame;    // 内网, 连接游戏服务器
	ICommonServer*         m_pShareGameSvr;
	CFileLog               m_FileLog;         // 简单文本日志
	CNetworkPtr            m_NetworkPtr;      // 网络对象
	ZONE_ADDR              m_ServerInfo;      // 本服务器信息
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

INLINE bool CZoneServer::OnTcpAccept(KeyRef, KeyRef)
{
	return false;
}

#endif // __ZONE_SERVER_IMP_H__
