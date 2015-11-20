//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginServerImp.cpp                           //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 登陆服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoginServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginServer
UInt CLoginServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CLoginServer::Update(void)
{
	CheckSessionTimeout();
	CheckConnectCenterServer();
	CheckConnectLoginDBServer();
	CheckUDPService();
	CheckTCPService();
	SyncServerInfo();
	SyncGameServerInfo();
	return 0;
}
//--------------------------------------
UInt CLoginServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}
// 获取共享的配置对象和网络对象
bool CLoginServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_DEFAULT_CONFIG, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_LOGIN);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[登陆服务器]从同进程共享数据回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[登陆服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件
	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[登陆服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[登陆服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	return true;
}
// 初始化配置
bool CLoginServer::InitLoadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krConnectLoginDB == nullptr);
	assert(m_krUDPService == nullptr);
	assert(m_krTCPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);
	assert(m_pShareLoginDBSvr == nullptr);
	assert(m_pShareGameSvr == nullptr);

	assert(m_GameSvrMap.Size() == 0);
	assert(m_SessionMap.Size() == 0);

	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_LOGIN);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_LOGIN);
	LOGV_INFO(m_FileLog, TF("[登陆服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);
	if (m_pConfig->GetNetworkAttr() & ATTR_IPV6) {
		m_ServerInfo.NetAddr[LOGINI_TCP].usAttr     = ATTR_IPV6;
		m_ServerInfo.NetAddr[LOGINI_UDP].usAttr     = ATTR_IPV6;
		m_ServerInfo.NetAddr[LOGINI_LOGINDB].usAttr = ATTR_IPV6;
	}
	UShort     usPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, 0, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[LOGINI_TCP]); // client tcp

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, CServerConfig::CFG_DEFAULT_GAME, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[LOGINI_UDP]); // game udp

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGINDB, 0, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[LOGINI_LOGINDB]); // logindb
	return true;
}
//--------------------------------------
void CLoginServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务退出处理完成!"));
	}
}
// 清除共享的配置对象和网络对象
void CLoginServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CLoginServer::ExitUnloadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krConnectLoginDB == nullptr);
	assert(m_krUDPService == nullptr);
	assert(m_krTCPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);
	assert(m_pShareLoginDBSvr == nullptr);
	assert(m_pShareGameSvr == nullptr);

	assert(m_GameSvrMap.Size() == 0);
	assert(m_SessionMap.Size() == 0);
}
//--------------------------------------
bool CLoginServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务启动开始!"));

		if ((StartUDPService() == false) || // 提前开启获取地址信息
			(StartConnectCenterServer() == false)  ||
			(StartConnectLoginDBServer() == false) ||
			(StartTCPService() == false)) {
			return false;
		}

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务启动完成!"));
	}
	return true;
}
// 运行创建连接中心服务器的连接对象
bool CLoginServer::StartConnectCenterServer(void)
{
	// 登陆和中心在不同进程,  需要连接内网中心服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) == 0) {
		if (m_krConnectCenter == nullptr) {
			UShort      usPort = 0;
			CStringKey  strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, CServerConfig::CFG_DEFAULT_CENTER, strAddr, usPort);
			m_krConnectCenter = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectCenter != nullptr) {
			if (m_bCenterCnnted == false) {
				LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务器和中心服务器在不同进程, 创建连接中心服务器Socket成功"));
				UShort      usPort = 0;
				CStringKey  strAddr;
				m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, usPort);
				if (m_NetworkPtr->Connect(m_krConnectCenter, usPort, *strAddr) == false) {
					LOGV_ERROR(m_FileLog, TF("[登陆服务器]连接中心服务器[%s]:%d请求失败"), *strAddr, usPort);
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[登陆服务器]连接中心服务器[%s]:%d请求完成"), *strAddr, usPort);
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[登陆服务器]登陆服务器和中心服务器在不同进程, 创建连接中心服务器Socket失败"));
			return false;
		}
	}
	else if (m_bCenterCnnted == false) {
		assert(m_pShareCenterSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::CenterServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[登陆服务器]从动态变量表获取同进程中心服务器对象指针无效"));
			return false;
		}
		m_pShareCenterSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bCenterCnnted = true;

		LOG_INFO(m_FileLog, TF("[登陆服务器]同进程直接连接中心服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CLoginLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CBufReadStream brs((PByte)m_ServerInfo.NetAddr, sizeof(m_ServerInfo.NetAddr));
		Link.SetStream(brs);

		m_bCenterLinked = m_pShareCenterSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<LLong>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_CENTER);
		return m_bCenterLinked;
	}
	return true;
}
// 运行创建连接登陆DB服务器的连接对象
bool CLoginServer::StartConnectLoginDBServer(void)
{
	// 登陆和登陆DB在不同进程,  需要连接内网登陆DB服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_LOGINDB) == 0) {
		if (m_krConnectLoginDB == nullptr) {
			UShort      usPort = 0;
			CStringKey  strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGIN, CServerConfig::CFG_DEFAULT_LOGINDB, strAddr, usPort);
			m_krConnectLoginDB = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectLoginDB != nullptr) {
			if (m_bLoginDBCnnted == false) {
				LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务器和登陆DB服务器在不同进程, 创建连接登陆DB服务器Socket成功"));
				if (m_NetworkPtr->Connect(m_krConnectLoginDB, m_ServerInfo.NetAddr[LOGINI_LOGINDB]) == false) {
					LOGV_ERROR(m_FileLog, TF("[登陆服务器]连接登陆DB服务器请求失败"));
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[登陆服务器]连接登陆DB服务器请求完成"));
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[登陆服务器]登陆服务器和登陆DB服务器在不同进程, 创建连接中心服务器Socket失败"));
			return false;
		}
	}
	else if (m_bLoginDBCnnted == false) {
		assert(m_pShareLoginDBSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::LoginDBServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[登陆服务器]从动态变量表获取同进程登陆DB服务器对象指针无效"));
			return false;
		}
		m_pShareLoginDBSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bLoginDBCnnted = true;

		LOG_INFO(m_FileLog, TF("[登陆服务器]同进程直接连接登陆DB服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CLoginLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CBufReadStream brs((PByte)m_ServerInfo.NetAddr, sizeof(m_ServerInfo.NetAddr));
		Link.SetStream(brs);

		m_bLoginDBLinked = m_pShareLoginDBSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<LLong>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_LOGINDB);
		return m_bLoginDBLinked;
	}
	return true;
}
// 运行创建UDP监听游戏服务器校验连接对象
bool CLoginServer::StartUDPService(void)
{
	if (m_krUDPService == nullptr) {
		m_krUDPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[LOGINI_UDP], SOCKET_UDP);
		if (m_krUDPService != nullptr) {
			LOGV_INFO(m_FileLog, TF("[登陆服务器]创建UDP监听游戏服务器成功"));
			if (m_ServerInfo.NetAddr[LOGINI_UDP].usPort == 0) {
				m_NetworkPtr->GetAddr(m_krUDPService, m_ServerInfo.NetAddr[LOGINI_UDP], false);
			}
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(m_ServerInfo.NetAddr + LOGINI_UDP), DATA_INDEX_LOGIN);
		}
		else {
			LOGV_ERROR(m_FileLog, TF("[登陆服务器]创建UDP监听游戏服务器失败"));
			return false;
		}
	}
	if (((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAME) != 0) &&
		(m_pShareGameSvr == nullptr)) {
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::GameServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[登陆服务器]从动态变量表获取同进程游戏服务器对象指针无效"));
			return false;
		}
		m_pShareGameSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
	}
	return true;
}

// 运行创建监听客户端连接对象
bool CLoginServer::StartTCPService(void)
{
	if (m_krTCPService != nullptr) {
		LOGV_INFO(m_FileLog, TF("[登陆服务器]创建监听客户端的连接已经存在"));
		return true;
	}
	bool bRet = true;
	m_krTCPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[LOGINI_TCP]);
	if (m_krTCPService != nullptr) {
		bRet = m_NetworkPtr->Listen(m_krTCPService);
		LOGV_INFO(m_FileLog, TF("[登陆服务器]创建监听客户端的连接成功, %s"), bRet ? TF("监听连接成功") : TF("监听连接失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[登陆服务器]创建监听客户端的连接失败"));
		bRet = false;
	}
	return bRet;
}
//--------------------------------------
bool CLoginServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[登陆服务器]暂停服务"));
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[登陆服务器]允许服务"));
		return true;
	}
	return false;
}
//--------------------------------------
void CLoginServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务停止开始!"));

		StopUDPService();
		StopTCPService();
		StopConnectLoginDBServer();
		StopConnectCenterServer();

		m_ServerInfo.Zero();
		m_GameInfo.Reset();
		m_GameSvrMap.RemoveAll();
		m_SessionMap.RemoveAll();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[登陆服务器]登陆服务停止完成!"));
	}
}
// 停止连接中心服务器
void CLoginServer::StopConnectCenterServer(void)
{
	if (m_krConnectCenter != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectCenter, false);
		m_krConnectCenter = nullptr;
		LOG_INFO(m_FileLog, TF("[登陆服务器]销毁连接中心服务器的连接成功"));
	}
	m_bCenterCnnted   = false;
	m_bCenterLinked   = false;
	m_pShareCenterSvr = nullptr;
}
// 停止连接登陆DB服务器
void CLoginServer::StopConnectLoginDBServer(void)
{
	if (m_krConnectLoginDB != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectLoginDB, false);
		m_krConnectLoginDB = nullptr;
		LOG_INFO(m_FileLog, TF("[登陆服务器]销毁连接登陆DB服务器的连接成功"));
	}
	m_bLoginDBCnnted   = false;
	m_bLoginDBLinked   = false;
	m_pShareLoginDBSvr = nullptr;
}
// 停止UDP监听游戏服务器
void CLoginServer::StopUDPService(void)
{
	if (m_krUDPService != nullptr) {
		m_NetworkPtr->Destroy(m_krUDPService, false);
		m_krUDPService = nullptr;
		LOG_INFO(m_FileLog, TF("[登陆服务器]销毁UDP服务的连接成功"));
	}
	m_pShareGameSvr = nullptr;
}
// 停止监听客户端连接
void CLoginServer::StopTCPService(void)
{
	if (m_krTCPService != nullptr) {
		m_NetworkPtr->Destroy(m_krTCPService, false);
		m_krTCPService = nullptr;
		LOG_INFO(m_FileLog, TF("[登陆服务器]销毁监听客户端的连接成功"));
	}
}
//--------------------------------------
bool CLoginServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong, CEventQueue::EVENT_TYPE)
{
	assert((m_pConfig->GetLoadServers() & (CServerConfig::CFG_DEFAULT_LOGINDB|CServerConfig::CFG_DEFAULT_GAME)) != 0);
	switch (nEvent) {
	// logindb
	case PAK_EVENT_LOGIN_LINKACK:
		{
			GameLinkAck(static_cast<CPAKSessionAck*>(&EventRef));
		}
		break;
	case PAK_EVENT_LOGIN_UNLINKACK:
		{
			GameUnlinkAck(reinterpret_cast<CPAKSessionAck*>(&EventRef));
		}
		break;
	// game or gate
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			CPAKSessionAck* pAck = reinterpret_cast<CPAKSessionAck*>(&EventRef);
			assert((pAck->GetType() == PAK_TYPE_GAME) || (pAck->GetType() == PAK_TYPE_GATE));
			SelectGameAck(pAck);
		}
		break;
	case PAK_EVENT_LOGIN_LINK_GAME:
		{
			CPAKLoginLinkGame* pLink = reinterpret_cast<CPAKLoginLinkGame*>(&EventRef);
			assert(pLink->GetType() == PAK_TYPE_GATE);
			LinkGameAck(pLink);
		}
		break;
	case PAK_EVENT_LOGIN_QUEUE_PLAY:
		{
			CPAKSessionAck* pAck = reinterpret_cast<CPAKSessionAck*>(&EventRef);
			assert(pAck->GetType() == PAK_TYPE_GATE);
			PlayGameAck(pAck);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return true;
}

bool CLoginServer::OnShareRoutine(Int nEvent, CStream& Stream, LLong)
{
	assert((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) != 0);
	if (nEvent == PAK_EVENT_SYNC) {
		LOGV_INFO(m_FileLog, TF("[登陆服务器]同进程同步游戏服务器信息"));
		CStreamScopePtr StreamPtr;
		if (m_NetworkPtr->ReferBuffer(StreamPtr, Stream)) { // 同进程的Stream是写模式, 创建一个读模式的引用
			CPAKSync Sync;
			Sync.Serialize(*StreamPtr);

			m_GameInfo.Copy(Sync.GetServerData());
			SyncGameServerInfo(*StreamPtr);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[登陆服务器]同进程同步游戏服务器信息错误"));
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
		assert(0);
	}
	return true;
}
//--------------------------------------
bool CLoginServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	if (PktPtr->GetEvent() != PAK_EVENT_LIVE) {
		switch (PktPtr->GetType()) {
		case PAK_TYPE_CLIENT_LOGIN:
			{
				bRet = DispatchGameService(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_LOGINDB:
			{
				bRet = DispatchLoginDBServer(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_CENTER:
			{
				bRet = DispatchCenterServer(PktPtr, pTcp->krSocket);
			}
			break;
		default:
			{
				LOGV_WARN(m_FileLog, TF("[登陆服务器]%p连接信令包类型[%d]不正确"), pTcp->krSocket, PktPtr->GetType());
				bRet = false;
			}
		}
	}
	else {
		CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_LOGIN> LiveAck;
		LiveAck.AdjustSize();
		bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
	}
	return bRet;
}
// 处理中心服务器的信令包
bool CLoginServer::DispatchCenterServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bCenterLinked = true;

			NET_ADDR NetAddr;
			m_NetworkPtr->GetAddr(krSocket, NetAddr, false);
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&NetAddr), DATA_INDEX_CENTER);
			LOG_INFO(m_FileLog, TF("[登陆服务器]收到中心服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[登陆服务器]收到中心服务器更新回复包"));
		}
		break;
	case PAK_EVENT_SYNC:
		{
			DEV_INFO(TF("[登陆服务器]中心服务器同步游戏服务器信息"));
			CPAKSync* pSync = static_cast<CPAKSync*>(PktPtr.Get());
			m_GameInfo.Copy(pSync->GetServerData());

			if (pSync->CheckStream()) {
				SyncGameServerInfo(pSync->GetStream());
			}
			// 发送同步回执包
			CPAKSimple<PAK_EVENT_SYNCACK, PAK_TYPE_LOGIN> SyncAck;
			SyncAck.AdjustSize();
			m_NetworkPtr->Send(krSocket, SyncAck);
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bCenterLinked = false;
			LOG_INFO(m_FileLog, TF("[登陆服务器]收到中心服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆服务器]%p连接信息无法识别的中心服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
// 处理登陆DB服务器的信令包
bool CLoginServer::DispatchLoginDBServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bLoginDBLinked = true;

			NET_ADDR NetAddr;
			m_NetworkPtr->GetAddr(krSocket, NetAddr, false);
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&NetAddr), DATA_INDEX_LOGINDB);
			LOG_INFO(m_FileLog, TF("[登陆服务器]收到登陆DB服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[登陆服务器]收到登陆DB服务器更新回复包"));
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bLoginDBLinked = false;
			LOG_INFO(m_FileLog, TF("[登陆服务器]收到登陆DB服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LOGIN_LINKACK:
		{
			GameLinkAck(static_cast<CPAKSessionAck*>(PktPtr.Get()));
		}
		break;
	case PAK_EVENT_LOGIN_UNLINKACK:
		{
			GameUnlinkAck(static_cast<CPAKSessionAck*>(PktPtr.Get()));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆服务器]%p连接信息无法识别的登陆DB服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
// 处理游戏客户端的信令包
bool CLoginServer::DispatchGameService(const PacketPtr& PktPtr, KeyRef krSocket)
{
	bool bRet = false;
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LOGIN_LINK:
		{
			bRet = OnClientLink(static_cast<CPAKLoginLink*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_LOGIN_UNLINK:
		{
			bRet = OnClientUnlink(static_cast<CPAKLoginUnlink*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			bRet = OnClientSelectGame(static_cast<CPAKLoginSelectGame*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_LINK:
		{	// 客户端需要主动连接来避免网络层ACK断开
			m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_CLIENT_READY);
			CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_LOGIN> LinkAck;
			LinkAck.AdjustSize();
			bRet = m_NetworkPtr->Send(krSocket, LinkAck);
		}
		break;
	case PAK_EVENT_UNLINK: // 客户端主动断开连接
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆服务器]%p连接信息无法识别的客户端信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return bRet;
}

bool CLoginServer::OnClientLink(CPAKLoginLink* pLink, KeyRef krSocket)
{
	UInt uAck = LOGIN_ERROR_SESSION;

	SESSION_DATA sd;
	sd.llTimeout = CPlatform::GetRunningTime();
	sd.nStatus   = SESSION_DATA::SESSION_STATUS_LINK;
	{
		CSyncLockWaitScope scope(m_SessionMap.GetLock());
		SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)krSocket);
		if (pPair == nullptr) {
			m_SessionMap.Add((DataRef)krSocket, sd);
			uAck = DATAD_OKAY;
		}
		else if (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_NONE) { // 登陆失败
			pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_LINK;
			pPair->m_V.llTimeout = sd.llTimeout;
			uAck = DATAD_OKAY;
		}
	}
	if (uAck == DATAD_OKAY) {
		pLink->SetType(PAK_TYPE_LOGIN);
		pLink->SetSessionId((LLong)krSocket);
		// 发送给登陆DB进行登陆处理, 超时关闭当前连接
		if (m_pShareLoginDBSvr == nullptr) {
			m_NetworkPtr->Send(m_krConnectLoginDB, *pLink);
		}
		else {
			m_pShareLoginDBSvr->OnShareRoutine(PAK_EVENT_LOGIN_LINK, *pLink);
		}
	}
	else {
		CPAKAck LinkAck(PAK_EVENT_LOGIN_LINKACK, (UInt)PAK_TYPE_LOGIN);
		LinkAck.SetAck(LOGIN_ERROR_SESSION);
		LinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, LinkAck, SEND_CLOSE);
	}
	return true;
}

bool CLoginServer::OnClientUnlink(CPAKLoginUnlink* pUnlink, KeyRef krSocket)
{
	UInt uAck = LOGIN_ERROR_SESSION;
	{
		CSyncLockWaitScope scope(m_SessionMap.GetLock());
		SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			// session &  UNLINK > status >= LINKACK
			if ( (pUnlink->GetSessionId() == reinterpret_cast<LLong>(pPair))  &&
				 (pPair->m_V.nStatus >= SESSION_DATA::SESSION_STATUS_LINKACK) &&
				 (pPair->m_V.nStatus < SESSION_DATA::SESSION_STATUS_UNLINK) ) {

				pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_UNLINK;
				pPair->m_V.llTimeout = CPlatform::GetRunningTime();

				pUnlink->SetType(PAK_TYPE_LOGIN);
				pUnlink->SetSessionId((LLong)krSocket);
				pUnlink->SetUserId(pPair->m_V.llUserId);
				pUnlink->SetTime(pPair->m_V.llOnline);
				pUnlink->SetGameId(pPair->m_V.nGameId);
				pUnlink->SetCltLast(pPair->m_V.CltLast);

				uAck = DATAD_OKAY;
			}
		}
	}
	if (uAck == DATAD_OKAY) { // 发送给登陆DB进行登陆处理, 超时关闭当前连接
		if (m_pShareLoginDBSvr == nullptr) {
			m_NetworkPtr->Send(m_krConnectLoginDB, *pUnlink);
		}
		else {
			m_pShareLoginDBSvr->OnShareRoutine(PAK_EVENT_LOGIN_UNLINK, *pUnlink);
		}
	}
	else {
		CPAKAck UnlinkAck(PAK_EVENT_LOGIN_UNLINKACK, (UInt)PAK_TYPE_LOGIN);
		UnlinkAck.SetAck(uAck);
		UnlinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UnlinkAck, SEND_CLOSE);
	}
	return true;
}

bool CLoginServer::OnClientSelectGame(CPAKLoginSelectGame* pSelect, KeyRef krSocket)
{
	UInt  uAck    = LOGIN_ERROR_SESSION;
	LLong llIndex = pSelect->GetAuthCode();
	{
		CSyncLockWaitScope scope(m_SessionMap.GetLock());
		SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			// session &  status == LINKACK
			if ((pSelect->GetSessionId() == reinterpret_cast<LLong>(pPair)) &&
				(pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_LINKACK)) {

				pPair->m_V.nStatus     = SESSION_DATA::SESSION_STATUS_SELECT;
				pPair->m_V.llTimeout   = CPlatform::GetRunningTime();
				pPair->m_V.llGameIndex = llIndex;

				pSelect->SetType(PAK_TYPE_LOGIN);
				pSelect->SetAck(DATAD_OKAY);
				pSelect->SetSessionId((LLong)krSocket);
				pSelect->SetAuthCode(pPair->m_V.llUserId); // user-id
				pSelect->SetAddr(m_ServerInfo.NetAddr[LOGINI_UDP]);

				uAck = DATAD_OKAY;
			}
		}
	}
	if (uAck == DATAD_OKAY) {
		CSyncLockWaitScope scope(m_GameSvrMap.GetLock());
		SVR_GAME_MAP::SVR_MAP_PAIR* pGameSvr = m_GameSvrMap.Find((DataRef)llIndex);
		if (pGameSvr != nullptr) {
			if ((DataRef)m_pShareGameSvr == pGameSvr->m_K) {
				m_pShareGameSvr->OnShareRoutine(PAK_EVENT_LOGIN_SELECT_GAME, *pSelect, reinterpret_cast<LLong>(this));
			}
			else {
				m_NetworkPtr->SendTo(m_krUDPService, *pSelect, pGameSvr->m_V.NetAddr[GAMEI_UDP]);
			}
			DEV_INFO(TF("[登陆服务器]客户端请求登陆游戏服务器, 发送UserId=%llx的连接请求给对应的游戏服务器"), pSelect->GetAuthCode());
		}
		else {
			DEV_INFO(TF("[登陆服务器]客户端请求登陆游戏服务器, 但游戏服务器信息[%llx]错误"), llIndex);
			uAck = LOGIN_ERROR_GAME_INDEX;
		}
	}
	if (uAck != DATAD_OKAY) {
		CPAKAck SelectGameAck(PAK_EVENT_LOGIN_SELECT_GAME, (UInt)PAK_TYPE_LOGIN);
		SelectGameAck.SetAck(uAck);
		SelectGameAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, SelectGameAck, SEND_CLOSE);
	}
	return true;
}
//--------------------------------------
bool CLoginServer::OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			assert((PktPtr->GetType() == PAK_TYPE_GAME) || (PktPtr->GetType() == PAK_TYPE_GATE));
			SelectGameAck(static_cast<CPAKSessionAck*>(PktPtr.Get()));
		}
		break;
	case PAK_EVENT_LOGIN_LINK_GAME:
		{
			assert(PktPtr->GetType() == PAK_TYPE_GATE);
			LinkGameAck(static_cast<CPAKLoginLinkGame*>(PktPtr.Get()));
		}
		break;
	case PAK_EVENT_LOGIN_QUEUE_PLAY:
		{
			assert(PktPtr->GetType() == PAK_TYPE_GATE);
			PlayGameAck(static_cast<CPAKSessionAck*>(PktPtr.Get()));
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆服务器]%p连接信息信令包类型[%d]不是游戏服务器类型"), pUdp->krSocket, PktPtr->GetType());
		}
	}
	return true;
}
//--------------------------------------
bool CLoginServer::OnTcpAccept(KeyRef krAccept, KeyRef krListen)
{
	UNREFERENCED_PARAMETER( krListen );
	assert(krListen == m_krTCPService);
	if (m_nStatus == STATUSC_RUN) {
		NET_ADDR ClientAddr;
		m_NetworkPtr->GetAddr(krAccept, ClientAddr);
		// 判断IP是否在黑名单中
		if (CheckAddrBlacklist(ClientAddr) == false) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);

			DEV_INFO(TF("[登陆服务器]客户端[%s]:%d连接完成!"), *strAddr, usPort);
			m_ServerInfo.Incr();
			return true;
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]登陆服务器处于暂停状态!"));
	}
	return false;
}

bool CLoginServer::CheckAddrBlacklist(NET_ADDR& ClientAddr)
{
	// TODO!!! 黑名单校验未完成
	size_t stHash = 0;
	if (m_pConfig->GetNetworkAttr() & ATTR_IPV6) {
		stHash = CHash::Hash(ClientAddr.Addr.bAddr, NET_ADDR::LEN_BYTE);
	}
	else {
		stHash = (size_t)ClientAddr.Addr.uAddr[0];
	}
	// 1. 是否在永久黑名单
	// 2. 是否在定时黑名单(短期是否有多次连续请求->增加屏蔽时间)--管理员查看可以将屏蔽时间长的加入永久黑名单
	// 3. 临时地址里面短期是否有多次连续请求->存在加入定时黑名单
	return (stHash == 0);
}
//--------------------------------------
bool CLoginServer::OnTcpConnect(UInt uError, KeyRef krConnect)
{
	if (krConnect == m_krConnectCenter) {
		m_bCenterCnnted = (uError == 0);
		if (m_bCenterCnnted) {
			LinkCenterServer();
		}
		else {
			DEV_INFO(TF("[登陆服务器]连接中心服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[登陆服务器]连接中心服务器失败%X!"), uError);
		}
	}
	else if (krConnect == m_krConnectLoginDB) {
		m_bLoginDBCnnted = (uError == 0);
		if (m_bLoginDBCnnted) {
			LinkLoginDBServer();
		}
		else {
			DEV_INFO(TF("[登陆服务器]连接登陆DB服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[登陆服务器]连接登陆DB服务器失败%X!"), uError);
		}
	}
	return true;
}
// 连接中心服务器
void CLoginServer::LinkCenterServer(void)
{
	if (m_bCenterCnnted && (m_bCenterLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CLoginLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CStreamScopePtr StreamPtr;
		m_NetworkPtr->AllocBuffer(StreamPtr);
		if (StreamPtr != nullptr) {
			Link.Serialize(*StreamPtr);
			m_ServerInfo.Addr(*StreamPtr);

			m_NetworkPtr->Send(m_krConnectCenter, *StreamPtr);
		}
		else {
			DEV_WARN(TF("[登陆服务器]连接中心服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[登陆服务器]连接中心服务器创建网络数据流异常!"));
		}
	}
}
// 断连中心服务器
void CLoginServer::UnlinkCenterServer(void)
{
	if (m_bCenterCnnted && m_bCenterLinked) {
		m_bCenterLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_LOGIN> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectCenter, Unlink);
	}
}
// 连接登陆DB服务器
void CLoginServer::LinkLoginDBServer(void)
{
	if (m_bLoginDBCnnted && (m_bLoginDBLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CLoginLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CStreamScopePtr StreamPtr;
		m_NetworkPtr->AllocBuffer(StreamPtr);
		if (StreamPtr != nullptr) {
			Link.Serialize(*StreamPtr);
			m_ServerInfo.Addr(*StreamPtr);

			m_NetworkPtr->Send(m_krConnectLoginDB, *StreamPtr);
		}
		else {
			DEV_WARN(TF("[登陆服务器]连接登陆DB服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[登陆服务器]连接登陆DB服务器创建网络数据流异常!"));
		}
	}
}
// 断连登陆DB服务器
void CLoginServer::UnlinkLoginDBServer(void)
{
	if (m_bLoginDBCnnted && m_bLoginDBLinked)
	{
		m_bLoginDBLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_LOGIN> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectLoginDB, Unlink);
	}
}
//--------------------------------------
bool CLoginServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krConnectCenter) {
		m_krConnectCenter = nullptr;
		m_bCenterCnnted   = false;
		m_bCenterLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_CENTER);
		DEV_INFO(TF("[登陆服务器]连接中心服务器连接断连/断开"));
	}
	else if (krSocket == m_krConnectLoginDB) {
		m_krConnectLoginDB = nullptr;
		m_bLoginDBCnnted   = false;
		m_bLoginDBLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_LOGINDB);
		DEV_INFO(TF("[登陆服务器]连接登陆DB服务器连接断连/断开"));
	}
	else if (krSocket == m_krTCPService) {
		m_krTCPService = nullptr;
		DEV_INFO(TF("[登陆服务器]监听客户端连接关闭"));
	}
	else { // 单个服务器断连/断开
		switch (llLiveData) {
		case PAK_TYPE_CLIENT_LOGIN:
			{
				DEV_INFO(TF("[登陆服务器]客户端[%p]断接"), krSocket);
				CSyncLockWaitScope scope(m_SessionMap.GetLock());
				SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)krSocket);
				if (pPair != nullptr) {
					if ((pPair->m_V.nStatus > SESSION_DATA::SESSION_STATUS_LINK) &&
						(pPair->m_V.nStatus < SESSION_DATA::SESSION_STATUS_UNLINK)) {

						CTRefCountPtr<CPAKLoginUnlink> UnlinkPtr = MNEW CPAKLoginUnlink;
						UnlinkPtr->SetType(PAK_TYPE_LOGIN);
						//UnlinkPtr->SetSessionId(0); // 断开SessionId设置为0
						UnlinkPtr->SetUserId(pPair->m_V.llUserId);
						UnlinkPtr->SetTime(pPair->m_V.llOnline);
						UnlinkPtr->SetGameId(pPair->m_V.nGameId);
						UnlinkPtr->SetCltLast(pPair->m_V.CltLast);
						UnlinkPtr->AdjustSize();
						if (m_pShareLoginDBSvr == nullptr) {
							m_NetworkPtr->Send(m_krConnectLoginDB, *UnlinkPtr);
						}
						else {
							m_pShareLoginDBSvr->OnShareRoutine(PAK_EVENT_LOGIN_UNLINK, *UnlinkPtr);
						}
					}
					m_SessionMap.RemoveAt(reinterpret_cast<PINDEX>(pPair));
				}
				else {
					DEV_WARN(TF("[登陆服务器]客户端断开时无法查找到SessionId[%p]数据"), krSocket);
					LOGV_WARN(m_FileLog, TF("[登陆服务器]客户端断开时无法查找到SessionId[%p]数据"), krSocket);
				}
			}
			break;
		case PAK_TYPE_CLIENT_READY:
			{
				CSyncLockWaitScope scope(m_SessionMap.GetLock());
				m_SessionMap.Remove((DataRef)krSocket);
			}
			break;
		default:
			{
			}
		}
		m_ServerInfo.Decr();
	}
	return true;
}
//--------------------------------------
bool CLoginServer::OnUdpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krUDPService){
		m_krUDPService = nullptr;
		DEV_INFO(TF("[登陆服务器]UDP监听游戏服务器关闭"));
	}
	return true;
}
//--------------------------------------
bool CLoginServer::CheckSessionTimeout(void)
{
	TIMEOUT_MAP Timeout;
	{
		CSyncLockWaitScope scope(m_SessionMap.GetLock());
		PINDEX index = m_SessionMap.First();
		while (index != nullptr) {
			SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Next(index);
			switch (pPair->m_V.nStatus) {
			case SESSION_DATA::SESSION_STATUS_LINK:
			case SESSION_DATA::SESSION_STATUS_SELECT:
			case SESSION_DATA::SESSION_STATUS_GAME:
			case SESSION_DATA::SESSION_STATUS_UNLINK:
				{
					if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_OUT_MIN)) {
						Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_ACK_TIMEOUT);
					}
				}
				break;
			case SESSION_DATA::SESSION_STATUS_SELECTACK:
			case SESSION_DATA::SESSION_STATUS_GAMEACK:
				{
					if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_OUT_MAX)) {
						Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_GAME_PLAY);
					}
				}
				break;
			case SESSION_DATA::SESSION_STATUS_NONE:
			case SESSION_DATA::SESSION_STATUS_LINKACK:
			default:
				{
					if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_LIMIT)) {
						Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_LINK_IDLE);
					}
				}
			}
		}
	}
	CPAKAck UnlinkAck(PAK_EVENT_LOGIN_UNLINKACK, (UInt)PAK_TYPE_LOGIN);
	UnlinkAck.AdjustSize();
	for (PINDEX index = Timeout.GetFirst(); index != nullptr; ) {
		TIMEOUT_MAP_PAIR* pPair = Timeout.GetNext(index);
		UnlinkAck.SetAck(pPair->m_V);
		m_NetworkPtr->Send(pPair->m_K, UnlinkAck, SEND_CLOSE);
	}
	return true;
}
// 定时检测连接中心服务器的连接对象是否有效
bool CLoginServer::CheckConnectCenterServer(void)
{
	if (m_bCenterCnnted == false) {
		if (StartConnectCenterServer() == false) {
			return false;
		}
	}
	if (m_bCenterLinked == false) {
		LinkCenterServer();
	}
	return true;
}
// 定时检测连接登陆DB服务器的连接对象是否有效
bool CLoginServer::CheckConnectLoginDBServer(void)
{
	if (m_bLoginDBCnnted == false) {
		if (StartConnectLoginDBServer() == false) {
			return false;
		}
	}
	if (m_bLoginDBLinked == false) {
		LinkLoginDBServer();
	}
	return true;
}
// 定时检测UDP监听游戏服务器的连接对象是否有效
bool CLoginServer::CheckUDPService(void)
{
	if (m_krUDPService == nullptr) {
		return StartUDPService();
	}
	return true;
}
// 定时检测监听客户端的连接对象是否有效
bool CLoginServer::CheckTCPService(void)
{
	if (m_krTCPService == nullptr) {
		return StartTCPService();
	}
	return true;
}
//--------------------------------------
// 同步服务器信息给界面
bool CLoginServer::SyncServerInfo(void)
{
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		CLoginUpdate Update;
		Update.SetServerData(m_ServerInfo);
		Update.AdjustSize();
		if (m_pShareCenterSvr != nullptr) {
			m_pShareCenterSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectCenter, Update);
		}
		if (m_pShareLoginDBSvr != nullptr) {
			m_pShareLoginDBSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectLoginDB, Update);
		}
		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_LOGIN);
	}
	else { // 发送心跳
		if (m_pShareCenterSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_LOGIN> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectCenter, Live);
		}
		if (m_pShareLoginDBSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_LOGIN> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectLoginDB, Live);
		}
	}
	return true;
}
// 同步游戏服务器负载情况数组并同步给界面
bool CLoginServer::SyncGameServerInfo(CStream& Stream)
{
	LLong llPos = (LLong)Stream.Tell();
	m_pUIHandler->OnHandle(PAK_EVENT_SYNC, Stream, DATA_INDEX_GAME);
	m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_GameInfo), DATA_INDEX_GAME);

	Stream.Seek(llPos); // 跳转到数据开始的地方

	CSyncLockWaitScope scope(m_GameSvrMap.GetLock());
	return m_GameSvrMap.Serialize(Stream);
}
// 同步游戏服务器信息给客户端
bool CLoginServer::SyncGameServerInfo(void)
{
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	if (m_GameInfo.usStatus == STATUSU_SYNC) {
		CStreamScopePtr StreamPtr;
		m_NetworkPtr->AllocJumboBuffer(StreamPtr);
		if (StreamPtr != nullptr) {
			CLoginSync Sync;
			Sync.SetServerData(m_GameInfo);
			Sync.Serialize(*StreamPtr);

			bool bRet = false;
			{ CSyncLockWaitScope scope(m_GameSvrMap.GetLock()); bRet = m_GameSvrMap.Serialize(*StreamPtr, STATUSU_SYNCXLINK|STATUSU_PING); }
			if (bRet) {
				if (m_NetworkPtr->Send(nullptr, *StreamPtr, SEND_BROADCAST_AS, PAK_TYPE_CLIENT_LOGIN)) {
					CSyncLockWaitScope scope(m_GameSvrMap.GetLock());
					m_GameSvrMap.Update(); // 同步完成, 更新状态和清除断连的数据
					m_GameInfo.usStatus = STATUSU_OKAY;
				}
			}
			else { // 没有更新内容
				DEV_INFO(TF("[登陆服务器]同步游戏服务器信息给客户端发现没有同步数据2"));
			}
		}
		return false;
	}
	else {
		DEV_INFO(TF("[登陆服务器]同步游戏服务器信息给客户端发现没有同步数据1"));
	}
	return true;
}
//--------------------------------------
// 发送登陆处理结果给客户端
void CLoginServer::GameLinkAck(CPAKSessionAck* pAck)
{
	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)pAck->GetSessionId());
	if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_LINK)) {
		if (pAck->GetAck() == DATAD_OKAY) {
			pAck->SetType(PAK_TYPE_LOGIN);
			pAck->SetSessionId(reinterpret_cast<LLong>(pPair));
			// 加上游戏服务器信息
			// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
			//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
			//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
			CStreamScopePtr StreamPtr;
			m_NetworkPtr->AllocJumboBuffer(StreamPtr);
			if (StreamPtr != nullptr) {
				pAck->Serialize(*StreamPtr);
				StreamPtr->Write(&m_GameInfo, sizeof(SERVER_INFO));

				{ CSyncLockWaitScope gamescope(m_GameSvrMap.GetLock());  m_GameSvrMap.Serialize(*StreamPtr, STATUSU_OKAYSYNC|STATUSU_PING); }

				m_NetworkPtr->Send((KeyRef)pPair->m_K, *StreamPtr);
				m_NetworkPtr->SetAttr((KeyRef)pPair->m_K, PAK_TYPE_CLIENT_LOGIN);

				CPAKLoginLinkAck* pLinkAck = static_cast<CPAKLoginLinkAck*>(pAck);
				pPair->m_V.llUserId  = pLinkAck->GetUserId();
				pPair->m_V.llTimeout = CPlatform::GetRunningTime();
				pPair->m_V.llOnline  = CPlatform::GetRunningTime();
				pPair->m_V.nGameId   = pLinkAck->GetGameId();
				pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_LINKACK;
				pPair->m_V.nCount    = 0;
				m_NetworkPtr->GetAddr((KeyRef)pPair->m_K, pPair->m_V.CltLast);
			}
			else {
				DEV_WARN(TF("[登陆服务器]创建缓存用于处理PAK_EVENT_LOGIN_LINKACK时创建失败"));
				LOGV_WARN(m_FileLog, TF("[登陆服务器]创建缓存用于处理PAK_EVENT_LOGIN_LINKACK时创建失败"));
			}
		}
		else {
			CPAKAck LinkAck(PAK_EVENT_LOGIN_LINKACK, (UInt)PAK_TYPE_LOGIN);
			LinkAck.AdjustSize();

			pPair->m_V.nCount += 1;
			if (pPair->m_V.nCount > SESSION_DATA::SESSION_TIME_LINK_MAX) {
				LinkAck.SetAck(LOGIN_ERROR_LINK_LIMIT);
				// 加入定时黑名单
			}
			else {
				LinkAck.SetAck(pAck->GetAck());
			}
			if (LinkAck.GetAck() == LOGIN_ERROR_LINK_LIMIT) {
				m_NetworkPtr->Send((KeyRef)pPair->m_K, LinkAck, SEND_CLOSE);
			}
			else {
				pPair->m_V.llTimeout = CPlatform::GetRunningTime();
				pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_NONE;

				m_NetworkPtr->Send((KeyRef)pPair->m_K, LinkAck);
			}
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]登陆DB服务器处理的请求返回时无法查找到SessionId[%llx]数据或者Session状态不匹配"), pAck->GetSessionId());
	}
}
// 发送登出处理结果给客户端
void CLoginServer::GameUnlinkAck(CPAKSessionAck* pAck)
{
	assert(pAck->GetSessionId() > 0); // 断开客户端的SessionId为0
	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)pAck->GetSessionId());
	if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_UNLINK)) {
		Int nFlag = SEND_NORMAL;
		if (pAck->GetAck() == DATAD_OKAY) {
			pPair->m_V.Reset();
			m_NetworkPtr->SetAttr((KeyRef)pPair->m_K, PAK_TYPE_CLIENT_READY);
		}
		else {
			nFlag = SEND_CLOSE;
		}
		CPAKAck UnlinkAck(PAK_EVENT_LOGIN_UNLINKACK, (UInt)PAK_TYPE_LOGIN);
		UnlinkAck.SetAck(pAck->GetAck());
		UnlinkAck.AdjustSize();
		m_NetworkPtr->Send((KeyRef)pPair->m_K, UnlinkAck, nFlag);
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]登陆DB服务器处理的请求返回时无法查找到SessionId[%llx]数据或者Session状态不匹配"), pAck->GetSessionId());
	}
}
// 选择游戏服务器回复包
void CLoginServer::SelectGameAck(CPAKSessionAck* pAck)
{
	DEV_INFO(TF("[登陆服务器]收到%d类型服务器的选择游戏服务器回复包%d[%llx]"), pAck->GetType(), pAck->GetAck(), pAck->GetSessionId());
	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)pAck->GetSessionId());
	if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_SELECT)) {
		pPair->m_V.llTimeout = CPlatform::GetRunningTime();
		if (pAck->GetAck() == DATAD_OKAY) {
			pAck->SetType(PAK_TYPE_LOGIN);
			pAck->SetSessionId(reinterpret_cast<LLong>(pPair));// TODO!!!可以给客户端一个唯一的id进行校验

			pPair->m_V.nStatus = SESSION_DATA::SESSION_STATUS_SELECTACK;

			LLong llTemp = (LLong)pPair->m_K;
			/////////////////////////////////////////////////////////////////
			// 加密AuthCode(与客户端唯一性信息有关, 例如异或IP)
			if (pPair->m_V.CltLast.usAttr & ATTR_IPV6) {
				llTemp ^= pPair->m_V.CltLast.Addr.ullAddr[0];
				llTemp ^= pPair->m_V.CltLast.Addr.ullAddr[1];
				DEV_INFO(TF("[登陆服务器]加密Key前后为[%llx-%llx-%llx+%llx]"), pPair->m_K, llTemp, pPair->m_V.CltLast.Addr.ullAddr[0], pPair->m_V.CltLast.Addr.ullAddr[1]);
			}
			else {
				llTemp ^= pPair->m_V.CltLast.Addr.uAddr[0];
				DEV_INFO(TF("[登陆服务器]加密Key前后为[%llx-%llx-%x]"), pPair->m_K, llTemp, pPair->m_V.CltLast.Addr.uAddr[0]);
			}
			/////////////////////////////////////////////////////////////////
			(static_cast<CPAKLoginSelectGame*>(pAck))->SetAuthCode(llTemp);
			m_NetworkPtr->Send((KeyRef)pPair->m_K, (*pAck));
		}
		else {
			pPair->m_V.nStatus = SESSION_DATA::SESSION_STATUS_LINKACK;

			CPAKAck SelectGameAck(PAK_EVENT_LOGIN_SELECT_GAME, (UInt)PAK_TYPE_LOGIN);
			SelectGameAck.SetAck(pAck->GetAck());
			SelectGameAck.AdjustSize();
			m_NetworkPtr->Send((KeyRef)pPair->m_K, SelectGameAck);
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]游戏服务器处理的选择请求返回时无法查找到SessionId[%llx]数据或者Session状态不匹配"), pAck->GetSessionId());
	}
}
// 登录游戏服务器回复包
void CLoginServer::LinkGameAck(CPAKLoginLinkGame* pLink)
{
	DEV_INFO(TF("[登陆服务器]收到%d服务器的登陆1校验回复包%d[%llx-%llx]"), pLink->GetType(), pLink->GetAck(), pLink->GetSessionId(), pLink->GetAuthCode());
	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)pLink->GetSessionId());
	if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_SELECTACK)) {

		pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_GAME;
		pPair->m_V.llTimeout = CPlatform::GetRunningTime();

		pLink->SetType(PAK_TYPE_LOGIN);
		pLink->SetSessionId(reinterpret_cast<LLong>(pPair));
		m_NetworkPtr->Send((KeyRef)pPair->m_K, (*pLink));
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]游戏服务器处理的登陆请求返回时无法查找到SessionId[%llx]数据或者Session状态不匹配"), pLink->GetSessionId());
	}
}
// 用户进入/退出游戏排队或者游戏世界
void CLoginServer::PlayGameAck(CPAKSessionAck* pAck)
{
	DEV_INFO(TF("[登陆服务器]收到%d服务器的进入游戏回复包%d[%llx]"), pAck->GetType(), pAck->GetAck(), pAck->GetSessionId());
	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Find((DataRef)pAck->GetSessionId());
	if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_GAME)) {
		pPair->m_V.llTimeout = CPlatform::GetRunningTime();
		if (pAck->GetAck() == DATAD_OKAY) {
			pPair->m_V.nStatus  = SESSION_DATA::SESSION_STATUS_GAMEACK;
			
		}
		else {
			pPair->m_V.nCount += 1;
			if (pPair->m_V.nCount > SESSION_DATA::SESSION_TIME_LINK_MAX) {
				CPAKAck UnlinkAck(PAK_EVENT_LOGIN_UNLINKACK, (UInt)PAK_TYPE_LOGIN);
				UnlinkAck.SetAck(LOGIN_ERROR_SELECT_LIMIT);
				UnlinkAck.AdjustSize();
				m_NetworkPtr->Send((KeyRef)pPair->m_K, UnlinkAck, SEND_CLOSE);
			}
			else { 
				pPair->m_V.nStatus  = SESSION_DATA::SESSION_STATUS_LINKACK;
			}
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆服务器]游戏服务器处理的用户进入/退出游戏排队或者游戏世界返回时无法查找到SessionId[%llx]数据或者Session状态不匹配"), pAck->GetSessionId());
	}
}

