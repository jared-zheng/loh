//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CenterServerImp.h                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 中心服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __CENTER_SERVER_IMP_H__
#define __CENTER_SERVER_IMP_H__

#pragma once

#include "centerpacket.h"
#include "servermap.h"
#include "ServerConfig.h"
#include "CommonServer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 中心服务器实现
/// - 主要任务 :
///   -# 维护在线选择, 登陆和游戏服务器信息
///   -# 定时同步登陆服务器信息给选择服务器, 定时同步游戏服务器信息给登陆服务器
///   -# 事务简单, 全部使用网络线程和定时线程来处理
///   -# 从服务器扩展配置文件中读取数据库信息, 定时写入统计数据
class CCenterServer : public ICommonServer, public CPAKHandler
{
public:
	/// 各类型服务器组统计信息索引号
	enum INFO_INDEX
	{
		INFOI_CENTER,
		INFOI_SELECT,
		INFOI_LOGIN,
		INFOI_GAME,
		INFOI_MAX,
	};
public:
	CCenterServer(void);
	virtual ~CCenterServer(void);
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
private:
	CCenterServer(const CCenterServer&);
	CCenterServer& operator=(const CCenterServer&);
	/// 获取共享的配置对象和网络对象
	bool  InitLoadShare(void);
	/// 初始化配置
	bool  InitLoadConfig(void);

	/// 清除共享的配置对象和网络对象
	void  ExitUnloadShare(void);
	/// 清除配置
	void  ExitUnloadConfig(void);

	/// 创建监听选择, 登陆和游戏服务器对象
	bool  StartListenServers(void);
	/// 创建监听服务器对象
	bool  StartListenServer(KeyRef& krListen, const CStringKey& strAddr, UShort usPort);

	/// 停止监听服务器
	void  StopListenServers(void);

	/// 定时检测监听服务器对象是否有效
	bool  CheckListenServers(void);
	/// 定时检测监听选择服务器对象是否有效
	bool  CheckListenSelectServer(void);
	/// 定时检测监听登陆服务器对象是否有效
	bool  CheckListenLoginServer(void);
	/// 定时检测监听游戏服务器对象是否有效
	bool  CheckListenGameServer(void);

	/// 同步服务器信息
	bool  SyncServersInfo(void);
	/// 同步中心服务器信息给界面
	bool  SyncCenterServerInfo(void);
	/// 同步选择服务器信息给界面
	bool  SyncSelectServerInfo(void);
	/// 同步登陆服务器信息给所有选择服务器和界面
	bool  SyncLoginServerInfo(void);
	/// 同步游戏服务器信息给所有登陆服务器和界面
	bool  SyncGameServerInfo(void);
	/// 同步登陆服务器特殊信息给单个选择服务器
	bool  SendLoginServerInfo(KeyRef krSocket = nullptr);
	/// 同步游戏服务器特殊信息给单个登陆服务器
	bool  SendGameServerInfo(KeyRef krSocket = nullptr);

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

private:
	Int                    m_nStatus;         ///< 服务器状态
	CEventHandler*         m_pUIHandler;      ///< 界面回调接口
	CServerConfig*         m_pConfig;         ///< 配置对象
	KeyRef                 m_krListenSelect;  ///< 内网, 监听选择服务器
	KeyRef                 m_krListenLogin;   ///< 内网, 监听登陆服务器
	KeyRef                 m_krListenGame;    ///< 外网, 监听游戏服务器
	ICommonServer*         m_pShareSelectSvr; ///< 同进程选择服务器
	ICommonServer*         m_pShareLoginSvr;  ///< 同进程登陆服务器
	ICommonServer*         m_pShareGameSvr;   ///< 同进程游戏服务器
	CFileLog               m_FileLog;         ///< 简单文本日志
	CNetworkPtr            m_NetworkPtr;      ///< 网络对象
	SERVER_INFO            m_ServerInfo[INFOI_MAX];  ///< 服务器组统计数据
	SVR_SELECT_MAP         m_SelectSvrMap;   ///< 选择服务器信息
	SVR_LOGIN_MAP          m_LoginSvrMap;    ///< 登陆服务器信息
	SVR_GAME_MAP           m_GameSvrMap;     ///< 游戏服务器信息
};

INLINE CCenterServer::CCenterServer(void)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenSelect(nullptr)
, m_krListenLogin(nullptr)
, m_krListenGame(nullptr)
, m_pShareSelectSvr(nullptr)
, m_pShareLoginSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::CenterServer)
{
}

INLINE CCenterServer::~CCenterServer(void)
{
}

INLINE CCenterServer::CCenterServer(const CCenterServer&)
: m_nStatus(STATUSC_NONE)
, m_pUIHandler(nullptr)
, m_pConfig(nullptr)
, m_krListenSelect(nullptr)
, m_krListenLogin(nullptr)
, m_krListenGame(nullptr)
, m_pShareSelectSvr(nullptr)
, m_pShareLoginSvr(nullptr)
, m_pShareGameSvr(nullptr)
, m_FileLog(true, LOGL_ALL, nullptr, CServerConfig::CenterServer)
{
}

INLINE CCenterServer& CCenterServer::operator=(const CCenterServer&)
{
	return (*this);
}

INLINE bool CCenterServer::OnShareRoutine(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CCenterServer::OnShareRoutine(Int, CStream&, LLong)
{
	return false;
}

INLINE bool CCenterServer::OnUdpDispatch(const PacketPtr&, PUDP_PARAM)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	return false;
}

INLINE bool CCenterServer::OnTcpConnect(UInt, KeyRef)
{
	return false;
}

INLINE bool CCenterServer::OnUdpClose(KeyRef, LLong)
{
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CCenterServer::INFO_INDEX InfoIndex>
INLINE bool CCenterServer::ServerLink(CPAKLink* pLink, DataRef drServer, MAP_TYPE& MapRef)
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
		MAP_TYPE::SVR_MAP_PAIR* pPair = MapRef.Find(drServer);
		if (pPair == nullptr) {
			index = MapRef.Add(drServer, Pair.Value);
		}
		else if (pPair->m_V.usStatus == STATUSU_UNLINK) { // 注销的信息未同步到其他服务器
			pPair->m_V = Pair.Value;
			index = reinterpret_cast<PINDEX>(pPair);
		}
	}
	if (index != nullptr) { // 2.更新界面
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&Pair), DataIndex);
		m_ServerInfo[InfoIndex].Incr(pLink->GetServerData());
		return true;
	}
	return false;
}

template <typename MAP_TYPE, DATA_INDEX DataIndex, CCenterServer::INFO_INDEX InfoIndex>
INLINE bool CCenterServer::ServerUpdate(CPAKUpdate* pUpdate, DataRef drServer, MAP_TYPE& MapRef)
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

template <typename MAP_TYPE, DATA_INDEX DataIndex, CCenterServer::INFO_INDEX InfoIndex>
INLINE bool CCenterServer::ServerUnlink(KeyRef krSocket, MAP_TYPE& MapRef)
{
	// 2.更新界面
	m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, reinterpret_cast<uintptr_t>(krSocket), DataIndex);

	SERVER_DATA sd;
	{
		CSyncLockWaitScope scope(MapRef.GetLock());
		MAP_TYPE::SVR_MAP_PAIR* pPair = MapRef.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			sd = pPair->m_V;
			// 3.修改状态
			if ((DataIndex == DATA_INDEX_SELECT) || (pPair->m_V.usStatus <= STATUSU_LINK)) {
				MapRef.RemoveAt(reinterpret_cast<PINDEX>(pPair));// 登陆服务器从注册到注销, 信息还未同步到其他服务器, 直接作为无状态信息删除
			}
			else if ((pPair->m_V.usStatus == STATUSU_OKAY) || (pPair->m_V.usStatus == STATUSU_SYNC)) {
				pPair->m_V.usStatus = STATUSU_UNLINK; // 需要同步给其他服务器以后再删除
			}
		}
	}
	if (sd.usStatus > STATUSU_NONE) {
		m_ServerInfo[InfoIndex].Decr(sd);
		return true;
	}
	return false;
}

#endif // __CENTER_SERVER_IMP_H__
