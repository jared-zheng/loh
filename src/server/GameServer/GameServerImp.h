//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GameServerImp.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 游戏服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAME_SERVER_IMP_H__
#define __GAME_SERVER_IMP_H__

#pragma once

#include "gamedbpacket.h"
#include "gamepacket.h"
#include "zonepacket.h"
#include "gatepacket.h"
#include "loginpacket.h"
#include "servermap.h"
#include "serversort.h"
#include "ServerConfig.h"
#include "CommonServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 游戏服务器实现
/// - 主要任务 :
/// -# 维护网关服务器对游戏操作的请求的转发
/// -# 维护地图服务器对游戏操作回应的转发
/// -# 事务简单, 全部使用网络线程和定时线程来处理
class CGameServer : public ICommonServer, public CPAKHandler
{
public:
	/// 各类型服务器组统计信息索引号
	enum INFO_INDEX {
		INFOI_ZONE,
		INFOI_GATE,
		INFOI_MAX,
	};
public:
	CGameServer(void);
	virtual ~CGameServer(void);
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
private:
	CGameServer(const CGameServer&);
	CGameServer& operator=(const CGameServer&);
	/// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	/// 初始化配置
	bool  InitLoadConfig(void);

	/// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	/// 清除配置
	void  ExitUnloadConfig(void);

	/// 创建连接中心服务器对象
	bool  StartConnectCenterServer(void);
	/// 创建连接游戏DB服务器对象
	bool  StartConnectGameDBServer(void);
	/// 创建监听地图服务器对象
	bool  StartListenZoneServer(void);
	/// 创建监听网关服务器对象
	bool  StartListenGateServer(void);
	/// 创建UDP监听登陆服务器对象
	bool  StartUDPService(void);

	/// 停止连接中心服务器
	void  StopConnectCenterServer(void);
	/// 停止连接游戏DB服务器
	void  StopConnectGameDBServer(void);
	/// 停止监听地图服务器
	void  StopListenZoneServer(void);
	/// 停止监听网关服务器
	void  StopListenGateServer(void);
	/// 停止UDP监听登陆服务器
	void  StopUDPService(void);

	/// 处理中心服务器的信令包
	bool  DispatchCenterServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理游戏DB服务器的信令包
	bool  DispatchGameDBServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理地图服务器的信令包
	bool  DispatchZoneServer(const PacketPtr& PktPtr, KeyRef krSocket);
	/// 处理网关服务器的信令包
	bool  DispatchGateServer(const PacketPtr& PktPtr, KeyRef krSocket);

	/// 向中心服务器注册
	void  LinkCenterServer(void);
	/// 向中心服务器注销
	void  UnlinkCenterServer(void);
	/// 向游戏DB服务器注册
	void  LinkGameDBServer(void);
	/// 向游戏DB服务器注销
	void  UnlinkGameDBServer(void);

	/// 定时检测连接中心服务器对象是否有效
	bool  CheckConnectCenterServer(void);
	/// 定时检测连接游戏DB服务器对象是否有效
	bool  CheckConnectGameDBServer(void);
	/// 定时检测监听地图服务器对象是否有效
	bool  CheckListenZoneServer(void);
	/// 定时检测监听网关服务器对象是否有效
	bool  CheckListenGateServer(void);
	/// 定时检测UDP监听登陆服务器对象是否有效
	bool  CheckUDPService(void);

	/// 同步服务器信息给界面
	bool  SyncServersInfo(void);
	/// 同步游戏服务器信息给界面
	bool  SyncGameServerInfo(void);
	/// 同步地图服务器信息给界面
	bool  SyncZoneServerInfo(void);
	/// 同步网关服务器负载情况并同步给界面
	bool  SyncGateSortInfo(void);

	/// 同进程服务器处理-注册
	bool  OnShareLink(CEventBase& EventRef, LLong llParam);
	/// 同进程服务器处理-更新
	bool  OnShareUpdate(CEventBase& EventRef, LLong llParam);
	/// 网络服务器处理-注册
	bool  OnServerLink(CPAKLink* pLink, KeyRef krSocket);
	/// 网络服务器处理-更新
	bool  OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket);
	/// 网络服务器处理-注销
	bool  OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket);
	/// 注册服务器数据
	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerLink(CPAKLink* pLink, DataRef drServer, MAP_TYPE& MapRef);
	/// 更新服务器数据
	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer, MAP_TYPE& MapRef);
	/// 注销服务器数据
	template <typename MAP_TYPE, DATA_INDEX DataIndex, INFO_INDEX InfoIndex>
	bool  ServerUnlink(KeyRef krSocket, MAP_TYPE& MapRef);
	/// 选择合适的网关服务器来处理连接游戏请求
	bool  SelectGame(CPAKLoginSelectGame* pSelect, LLong llParam = 0);
	/// 注册的地图服务器对象和地图编号加入对应表
	bool  AddZoneServerIds(CStream& Stream, PINDEX index, DataRef drServer);
	/// 注销的地图服务器对象和地图编号从对应表删除
	bool  RemoveZoneServerIds(PINDEX index);
private:
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ZONE_ID
	typedef tagDATA_MAP<UInt, DataRef>   ZONE_ID_MAP, *PZONE_ID_MAP;
private:
	Int                 m_nStatus;          ///< 服务器状态
	bool                m_bCenterCnnted;	///< 是否连接中心服务器完成
	bool                m_bCenterLinked;	///< 是否向中心服务器注册完成
	bool                m_bGameDBCnnted;	///< 是否连接游戏DB服务器完成
	bool                m_bGameDBLinked;	///< 是否向游戏DB服务器注册完成
	CEventHandler*      m_pUIHandler;       ///< 界面回调接口
	CServerConfig*      m_pConfig;          ///< 配置对象
	KeyRef              m_krConnectCenter;  ///< 内网, 连接中心服务器
	KeyRef              m_krConnectGameDB;  ///< 内网, 连接游戏DB服务器
	KeyRef              m_krListenZone;     ///< 内网, 监听地图服务器
	KeyRef              m_krListenGate;     ///< 内网, 监听网关服务器
	KeyRef              m_krUDPService;     ///< 内网, 监听登陆服务器
	ICommonServer*      m_pShareCenterSvr;  ///< 同进程中心服务器
	ICommonServer*      m_pShareGameDBSvr;  ///< 同进程游戏DB服务器
	ICommonServer*      m_pShareZoneSvr;    ///< 同进程地图服务器
	ICommonServer*      m_pShareGateSvr;    ///< 同进程网关服务器
	CFileLog            m_FileLog;          ///< 简单文本日志
	CNetworkPtr         m_NetworkPtr;       ///< 网络对象
	GAME_ADDR           m_ServerInfo;       ///< 本服务器信息
	SERVER_INFO         m_OthersInfo[INFOI_MAX]; ///< 服务器组统计数据
	SVR_ZONE_INDEX_MAP  m_ZoneSvrMap;       ///< 地图服务器信息
	ZONE_ID_MAP         m_ZoneIdMap;        ///< 地图服务器对象和地图编号对应表
	SVR_GATE_MAP        m_GateSvrMap;       ///< 网关服务器信息
	RESULT_CHAIN        m_SortChain;        ///< 网关服务器排序结果 
};

INLINE CGameServer::CGameServer(void)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_bGameDBCnnted(false)
, m_bGameDBLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krConnectGameDB(nullptr)
, m_krListenZone(nullptr)
, m_krListenGate(nullptr)
, m_krUDPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_pShareGameDBSvr(nullptr)
, m_pShareZoneSvr(nullptr)
, m_pShareGateSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GameServer)
{
}

INLINE CGameServer::~CGameServer(void)
{
}

INLINE CGameServer::CGameServer(const CGameServer&)
: m_nStatus(STATUSC_NONE)
, m_bCenterCnnted(false)
, m_bCenterLinked(false)
, m_bGameDBCnnted(false)
, m_bGameDBLinked(false)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krConnectCenter(nullptr)
, m_krConnectGameDB(nullptr)
, m_krListenZone(nullptr)
, m_krListenGate(nullptr)
, m_krUDPService(nullptr)
, m_pShareCenterSvr(nullptr)
, m_pShareGameDBSvr(nullptr)
, m_pShareZoneSvr(nullptr)
, m_pShareGateSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::GameServer)
{
}

INLINE CGameServer& CGameServer::operator=(const CGameServer&)
{
	return (*this);
}

INLINE bool CGameServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGameServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameServer::INFO_INDEX InfoIndex>
INLINE bool CGameServer::ServerLink(CPAKLink* pLink, DataRef drServer, MAP_TYPE& MapRef)
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
		if ((DataIndex == DATA_INDEX_ZONE) && (index != nullptr)) {
			AddZoneServerIds(pLink->GetStream(), index, drServer);// 后面接的是地图服务器管理的地图区域数组
		}
	}
	if (index != nullptr) { // 2.更新界面
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&Pair), DataIndex);
		m_OthersInfo[InfoIndex].Incr(pLink->GetServerData());
		return true;
	}
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameServer::INFO_INDEX InfoIndex>
INLINE bool CGameServer::ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer, MAP_TYPE& MapRef)
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
		m_OthersInfo[InfoIndex].Diff(pUpdate->GetServerData(), Pair.Value);
		Pair.Value.Copy(pUpdate->GetServerData());
		m_pUIHandler->OnHandle(PAK_EVENT_UPDATE, reinterpret_cast<uintptr_t>(&Pair), DataIndex);// 更新界面
		return true;
	}
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CGameServer::INFO_INDEX InfoIndex>
INLINE bool CGameServer::ServerUnlink(KeyRef krSocket, MAP_TYPE& MapRef)
{
	// 2.更新界面
	m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, reinterpret_cast<uintptr_t>(krSocket), DataIndex);

	SERVER_DATA sd;
	{
		CSyncLockWaitScope scope(MapRef.GetLock());
		MAP_TYPE::SVR_MAP_PAIR* pPair = MapRef.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			sd = pPair->m_V;
			if (DataIndex == PAK_TYPE_ZONE) {
				RemoveZoneServerIds(reinterpret_cast<PINDEX>(pPair)); // 删除对应的地图id索引信息
			}
			MapRef.RemoveAt(reinterpret_cast<PINDEX>(pPair));
		}
	}
	if (sd.usStatus > STATUSU_NONE) {
		m_OthersInfo[InfoIndex].Decr(sd);
		return true;
	}
	return false;
}

#endif // __GAME_SERVER_IMP_H__
