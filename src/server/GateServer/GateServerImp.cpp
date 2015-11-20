//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GateServerImp.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 网关服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GateServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateServer
UInt CGateServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CGateServer::Update(void)
{
	CheckConnectGameDBServer();
	CheckConnectGameServer();
	CheckUDPService();
	CheckTCPService();
	SyncServerInfo();
	return 0;
}
//--------------------------------------
UInt CGateServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}
// 获取共享的配置对象和网络对象
bool CGateServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_DEFAULT_CONFIG, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_GATE);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[网关服务器]从界面回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[网关服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件
	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[网关服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[网关服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	return true;
}
// 初始化配置
bool CGateServer::InitLoadConfig(void)
{
	assert(m_krConnectGameDB == nullptr);
	assert(m_krConnectGame == nullptr);
	assert(m_krUDPService == nullptr);
	assert(m_krTCPService == nullptr);

	assert(m_pShareGameDBSvr == nullptr);
	assert(m_pShareGameSvr == nullptr);

	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_GATE);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_GATE);
	LOGV_INFO(m_FileLog, TF("[网关服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);
	if (m_pConfig->GetNetworkAttr() & ATTR_IPV6) {
		m_ServerInfo.NetAddr[GATEI_TCP].usAttr    = ATTR_IPV6;
		m_ServerInfo.NetAddr[GATEI_UDP].usAttr    = ATTR_IPV6;
		m_ServerInfo.NetAddr[GATEI_GAMEDB].usAttr = ATTR_IPV6;
	}
	UShort     usPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GATE, 0, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GATEI_TCP]); // tcp

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GATE, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GATEI_UDP]); // udp

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
	if (usPort == 0) {
		m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, 0, strAddr, usPort);
	}
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GATEI_GAMEDB]); // gamedb

	assert(m_GateRoutine == nullptr);
	m_GateRoutine = (ICommonRoutine*)CRTTI::CreateByName(TF("CGateRoutine"));
	if (m_GateRoutine != nullptr) {
		return (m_GateRoutine->Init() == (UInt)RET_OKAY);
	}
	LOG_ERROR(m_FileLog, TF("[网关服务器]创建事务处理对象失败"));
	return false;
}
//--------------------------------------
void CGateServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务退出处理完成!"));
	}
}
// 清除共享的配置对象和网络对象
void CGateServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CGateServer::ExitUnloadConfig(void)
{
	assert(m_krConnectGameDB == nullptr);
	assert(m_krConnectGame == nullptr);
	assert(m_krUDPService == nullptr);
	assert(m_krTCPService == nullptr);

	assert(m_pShareGameDBSvr == nullptr);
	assert(m_pShareGameSvr == nullptr);

	m_GateRoutine->Exit();
	m_GateRoutine = nullptr;
}
//--------------------------------------
bool CGateServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务启动开始!"));

		if ((StartUDPService() == false) || // 提前开启获取地址信息
			(StartConnectGameDBServer() == false) ||
			(StartConnectGameServer() == false) ||
			(StartTCPService() == false)) {
			return false;
		}
		m_GateRoutine->Start(this);

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务启动完成!"));
	}
	return true;
}
// 运行创建连接游戏DB服务器的连接对象
bool CGateServer::StartConnectGameDBServer(void)
{
	// 网关和游戏DB在不同进程,  需要连接内网游戏DB服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAMEDB) == 0) {
		if (m_krConnectGameDB == nullptr) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GATE, CServerConfig::CFG_DEFAULT_GAMEDB, strAddr, usPort);
			m_krConnectGameDB = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectGameDB != nullptr) {
			if (m_bGameDBCnnted == false) {
				LOG_INFO(m_FileLog, TF("[网关服务器]网关服务器和游戏DB服务器在不同进程, 创建连接游戏DB服务器Socket成功"));
				if (m_NetworkPtr->Connect(m_krConnectGameDB, m_ServerInfo.NetAddr[GATEI_GAMEDB]) == false)
				{
					LOGV_ERROR(m_FileLog, TF("[网关服务器]连接游戏DB服务器请求失败"));
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[网关服务器]连接游戏DB服务器请求完成"));
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[网关服务器]网关服务器和游戏DB服务器在不同进程, 创建连接游戏DB服务器Socket失败"));
			return false;
		}
	}
	else if (m_bGameDBCnnted == false) {
		assert(m_pShareGameDBSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::GameDBServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[网关服务器]从动态变量表获取同进程游戏DB服务器对象指针无效"));
			return false;
		}
		m_pShareGameDBSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bGameDBCnnted   = true;

		LOG_INFO(m_FileLog, TF("[网关服务器]同进程直接连接游戏DB服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGateLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CBufReadStream brs((PByte)m_ServerInfo.NetAddr, sizeof(m_ServerInfo.NetAddr));
		Link.SetStream(brs);

		m_bGameDBLinked = m_pShareGameDBSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<uintptr_t>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_GAMEDB);
		return m_bGameDBLinked;
	}
	return true;
}
// 运行创建连接游戏服务器的连接对象
bool CGateServer::StartConnectGameServer(void)
{
	// 网关和游戏在不同进程,  需要连接内网中心服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAME) == 0) {
		if (m_krConnectGame == nullptr) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GATE, CServerConfig::CFG_DEFAULT_GAME, strAddr, usPort);
			m_krConnectGame = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectGame != nullptr) {
			if (m_bGameCnnted == false) {
				LOG_INFO(m_FileLog, TF("[网关服务器]网关服务器和游戏服务器在不同进程, 创建连接游戏服务器Socket成功"));
				UShort      usPort = 0;
				CStringKey  strAddr;
				m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_GATE, strAddr, usPort);
				if (m_NetworkPtr->Connect(m_krConnectGame, usPort, *strAddr) == false) {
					LOGV_ERROR(m_FileLog, TF("[网关服务器]连接游戏服务器[%s]:%d请求失败"), *strAddr, usPort);
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[网关服务器]连接游戏服务器[%s]:%d请求完成"), *strAddr, usPort);
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[网关服务器]网关服务器和游戏服务器在不同进程, 创建连接游戏服务器Socket失败"));
			return false;
		}
	}
	else if (m_bGameCnnted == false) {
		assert(m_pShareGameSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::GameServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[网关服务器]从动态变量表获取同进程游戏服务器对象指针无效"));
			return false;
		}
		m_pShareGameSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bGameCnnted   = true;

		LOG_INFO(m_FileLog, TF("[网关服务器]同进程直接连接中心服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGateLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CBufReadStream brs((PByte)m_ServerInfo.NetAddr, sizeof(m_ServerInfo.NetAddr));
		Link.SetStream(brs);

		m_bGameLinked = m_pShareGameSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<uintptr_t>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_GAME);
		return m_bGameLinked;
	}
	return true;
}
// 运行创建UDP监听登陆服务器校验连接对象
bool CGateServer::StartUDPService(void)
{
	if (m_krUDPService == nullptr) {
		m_krUDPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GATEI_UDP], SOCKET_UDP);
		if (m_krUDPService != nullptr) {
			LOGV_INFO(m_FileLog, TF("[网关服务器]创建UDP监听登陆服务器成功"));
			if (m_ServerInfo.NetAddr[GATEI_UDP].usPort == 0) {
				m_NetworkPtr->GetAddr(m_krUDPService, m_ServerInfo.NetAddr[GATEI_UDP], false);
			}
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(m_ServerInfo.NetAddr + GATEI_UDP), DATA_INDEX_GATE);
		}
		else {
			LOGV_ERROR(m_FileLog, TF("[网关服务器]创建UDP监听登陆服务器失败"));
			return false;
		}
	}
	return true;
}
// 运行创建TCP监听客户端连接对象
bool CGateServer::StartTCPService(void)
{
	if (m_krTCPService != nullptr) {
		LOGV_INFO(m_FileLog, TF("[网关服务器]创建监听客户端的连接已经存在"));
		return true;
	}
	bool bRet = true;
	m_krTCPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GATEI_TCP]);
	if (m_krTCPService != nullptr) {
		bRet = m_NetworkPtr->Listen(m_krTCPService);
		LOGV_INFO(m_FileLog, TF("[网关服务器]创建监听客户端的连接成功, %s"), bRet ? TF("监听连接成功") : TF("监听连接失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[网关服务器]创建监听客户端的连接失败"));
		bRet = false;
	}
	return bRet;
}
//--------------------------------------
bool CGateServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[网关服务器]暂停服务"));
		m_GateRoutine->Pause();
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[网关服务器]允许服务"));
		m_GateRoutine->Pause(false);
		return true;
	}
	return false;
}
//--------------------------------------
void CGateServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务停止开始!"));

		m_GateRoutine->Stop();

		StopUDPService();
		StopTCPService();
		StopConnectGameServer();
		StopConnectGameDBServer();

		m_ServerInfo.Zero();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[网关服务器]网关服务停止完成!"));
	}
}
// 停止连接游戏DB服务器
void CGateServer::StopConnectGameDBServer(void)
{
	if (m_krConnectGameDB != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectGameDB, false);
		m_krConnectGameDB = nullptr;
		LOG_INFO(m_FileLog, TF("[网关服务器]销毁连接游戏DB服务器的连接成功"));
	}
	m_bGameDBCnnted   = false;
	m_bGameDBLinked   = false;
	m_pShareGameDBSvr = nullptr;
}
// 停止连接游戏服务器
void CGateServer::StopConnectGameServer(void)
{
	if (m_krConnectGame != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectGame, false);
		m_krConnectGame = nullptr;
		LOG_INFO(m_FileLog, TF("[网关服务器]销毁连接游戏服务器的连接成功"));
	}
	m_bGameCnnted   = false;
	m_bGameLinked   = false;
	m_pShareGameSvr = nullptr;
}
// 停止UDP监听登陆服务器
void CGateServer::StopUDPService(void)
{
	if (m_krUDPService != nullptr) {
		m_NetworkPtr->Destroy(m_krUDPService, false);
		m_krUDPService = nullptr;
		LOG_INFO(m_FileLog, TF("[网关服务器]销毁UDP服务的连接成功"));
	}
}
// 停止TCP监听客户端
void CGateServer::StopTCPService(void)
{
	if (m_krTCPService != nullptr) {
		m_NetworkPtr->Destroy(m_krTCPService, false);
		m_krTCPService = nullptr;
		LOG_INFO(m_FileLog, TF("[网关服务器]销毁监听客户端的连接成功"));
	}
}
//--------------------------------------
bool CGateServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	assert((m_pConfig->GetLoadServers() & (CServerConfig::CFG_DEFAULT_GAMEDB|CServerConfig::CFG_DEFAULT_GAME)) != 0);
	switch (nEvent) {
	// gamedb
	case PAK_EVENT_GATE_SELECTACK:
	case PAK_EVENT_GATE_PLAYACK:
	case PAK_EVENT_GATE_CREATEACK:
	case PAK_EVENT_GATE_DELETEACK:
	case PAK_EVENT_GATE_RECOVERACK:
	// game
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			m_GateRoutine->Add(nEvent, EventRef, llParam, eType);
		}
		break;
	default:
		{
			if ((nEvent > PAK_EVENT_REALM_BEGIN) && (nEvent < PAK_EVENT_REALM_END)) {
				m_GateRoutine->Add(nEvent, EventRef, llParam, eType);
			}
			else {
				LOGV_WARN(m_FileLog, TF("[网关服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
				assert(0);
			}
		}
	}
	return true;
}
//--------------------------------------
bool CGateServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	if (PktPtr->GetEvent() != PAK_EVENT_LIVE) {
		switch (PktPtr->GetType()) {
		case PAK_TYPE_CLIENT:
			{
				bRet = m_GateRoutine->Add((Int)PktPtr->GetEvent(), *PktPtr, (LLong)pTcp->krSocket);
			}
			break;
		case PAK_TYPE_GAMEDB:
			{
				bRet = DispatchGameDBServer(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_GAME:
			{
				bRet = DispatchGameServer(PktPtr, pTcp->krSocket);
			}
			break;
		default:
			{
				LOGV_WARN(m_FileLog, TF("[网关服务器]%p连接信令包类型[%d]不正确"), pTcp->krSocket, PktPtr->GetType());
				bRet = false;
			}
		}
	}
	else {
		CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_GATE> LiveAck;
		LiveAck.AdjustSize();
		bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
	}
	return bRet;
}
// 处理游戏DB服务器的信令包
bool CGateServer::DispatchGameDBServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bGameDBLinked = true;
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(m_ServerInfo.NetAddr + GATEI_GAMEDB), DATA_INDEX_GAMEDB);
			LOG_INFO(m_FileLog, TF("[网关服务器]收到游戏DB服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[网关服务器]收到游戏DB服务器更新回复包"));
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bGameDBLinked = false;
			LOG_INFO(m_FileLog, TF("[网关服务器]收到游戏DB服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	// gamedb
	case PAK_EVENT_GATE_SELECTACK:
	case PAK_EVENT_GATE_PLAYACK:
	case PAK_EVENT_GATE_CREATEACK:
	case PAK_EVENT_GATE_DELETEACK:
	case PAK_EVENT_GATE_RECOVERACK:
		{	
			m_GateRoutine->Add((Int)PktPtr->GetEvent(), *PktPtr);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[网关服务器]%p连接信息无法识别的游戏DB服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
// 处理游戏服务器的信令包
bool CGateServer::DispatchGameServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bGameLinked = true;

			NET_ADDR NetAddr;
			m_NetworkPtr->GetAddr(krSocket, NetAddr, false);
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&NetAddr), DATA_INDEX_GAME);
			LOG_INFO(m_FileLog, TF("[网关服务器]收到游戏服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[网关服务器]收到游戏服务器更新回复包"));
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bGameLinked = false;
			LOG_INFO(m_FileLog, TF("[网关服务器]收到游戏服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[网关服务器]%p连接信息无法识别的游戏服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
//--------------------------------------
bool CGateServer::OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	if (PktPtr->GetType() == PAK_TYPE_GAME) {
		if (PktPtr->GetEvent() == PAK_EVENT_LOGIN_SELECT_GAME) {
			m_GateRoutine->Add(PAK_EVENT_LOGIN_SELECT_GAME, *PktPtr);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[网关服务器]无法识别的服务器UDP信令包数据[event=%d]"), PktPtr->GetEvent());
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[网关服务器]%p连接信息信令包类型[%d]不是游戏服务器类型"), pUdp->krSocket, PktPtr->GetType());
	}
	return true;
}
//--------------------------------------
bool CGateServer::OnTcpAccept(KeyRef krAccept, KeyRef krListen)
{
	UNREFERENCED_PARAMETER(krListen);
	assert(krListen == m_krTCPService);
	if (m_nStatus == STATUSC_RUN) {
		NET_ADDR ClientAddr;
		m_NetworkPtr->GetAddr(krAccept, ClientAddr);
		if (m_GateRoutine->OnHandle(PAK_EVENT_LIVE, reinterpret_cast<uintptr_t>(&ClientAddr), TRUE)) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);

			DEV_INFO(TF("[网关服务器]客户端[%s]:%d连接完成!"), *strAddr, usPort);
			m_ServerInfo.Incr();
			return true;
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[网关服务器]网关服务器处于暂停状态!"));
	}
	return false;
}
//--------------------------------------
bool CGateServer::OnTcpConnect(UInt uError, KeyRef krConnect)
{
	if (krConnect == m_krConnectGameDB) {
		m_bGameDBCnnted = (uError == 0);
		if (m_bGameDBCnnted) {
			LinkGameDBServer();
		}
		else {
			DEV_INFO(TF("[网关服务器]连接游戏DB服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[网关服务器]连接游戏DB服务器失败%X!"), uError);
		}
	}
	else if (krConnect == m_krConnectGame) {
		m_bGameCnnted = (uError == 0);
		if (m_bGameCnnted) {
			LinkGameServer();
		}
		else {
			DEV_INFO(TF("[网关服务器]连接游戏服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[网关服务器]连接游戏服务器失败%X!"), uError);
		}
	}
	return true;
}
// 连接游戏DB服务器
void CGateServer::LinkGameDBServer(void)
{
	if (m_bGameDBCnnted && (m_bGameDBLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGateLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CStreamScopePtr StreamPtr;
		m_NetworkPtr->AllocBuffer(StreamPtr);
		if (StreamPtr != nullptr) {
			Link.Serialize(*StreamPtr);
			m_ServerInfo.Addr(*StreamPtr);

			m_NetworkPtr->Send(m_krConnectGameDB, *StreamPtr);
		}
		else {
			DEV_WARN(TF("[网关服务器]连接游戏DB服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[网关服务器]连接游戏DB服务器创建网络数据流异常!"));
		}
	}
}
// 断连游戏DB服务器
void CGateServer::UnlinkGameDBServer(void)
{
	if (m_bGameDBCnnted && m_bGameDBLinked)
	{
		m_bGameDBLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_GATE> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGameDB, Unlink);
	}
}
// 连接游戏服务器
void CGateServer::LinkGameServer(void)
{
	if (m_bGameCnnted && (m_bGameLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGateLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CStreamScopePtr StreamPtr;
		m_NetworkPtr->AllocBuffer(StreamPtr);
		if (StreamPtr != nullptr) {
			Link.Serialize(*StreamPtr);
			m_ServerInfo.Addr(*StreamPtr);

			m_NetworkPtr->Send(m_krConnectGame, *StreamPtr);
		}
		else {
			DEV_WARN(TF("[网关服务器]连接游戏服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[网关服务器]连接游戏服务器创建网络数据流异常!"));
		}
	}
}
// 断连游戏服务器
void CGateServer::UnlinkGameServer(void)
{
	if (m_bGameCnnted && m_bGameLinked) {
		m_bGameLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_GATE> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGame, Unlink);
	}
}
//--------------------------------------
bool CGateServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krConnectGameDB) {
		m_krConnectGameDB = nullptr;
		m_bGameDBCnnted   = false;
		m_bGameDBLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_GAMEDB);
		DEV_INFO(TF("[网关服务器]连接游戏DB服务器连接断连/断开"));
	}
	else if (krSocket == m_krConnectGame) {
		m_krConnectGame = nullptr;
		m_bGameCnnted   = false;
		m_bGameLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_GAME);
		DEV_INFO(TF("[网关服务器]连接中心服务器连接断连/断开"));
	}
	else if (krSocket == m_krTCPService) {
		m_krTCPService = nullptr;
		DEV_INFO(TF("[网关服务器]监听客户端连接断连/断开"));
	}
	else { // 单个断连/断开
		switch (llLiveData) {
		case PAK_TYPE_CLIENT:
		case PAK_TYPE_CLIENT_SELECT:
		case PAK_TYPE_CLIENT_QUEUE:
			{
				DEV_INFO(TF("[网关服务器]客户端[%p]断接"), krSocket);
				m_GateRoutine->OnHandle(PAK_EVENT_UNLINK, (uintptr_t)krSocket, llLiveData);
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
bool CGateServer::OnUdpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krUDPService){
		m_krUDPService = nullptr;
		DEV_INFO(TF("[网关服务器]UDP监听登陆服务器关闭"));
	}
	return true;
}
//--------------------------------------
// 定时检测连接游戏DB服务器的连接对象是否有效
bool CGateServer::CheckConnectGameDBServer(void)
{
	if (m_bGameDBCnnted == false) {
		if (StartConnectGameDBServer() == false) {
			return false;
		}
	}
	if (m_bGameDBLinked == false) {
		LinkGameDBServer();
	}
	return true;
}
// 定时检测连接游戏服务器的连接对象是否有效
bool CGateServer::CheckConnectGameServer(void)
{
	if (m_bGameCnnted == false) {
		if (StartConnectGameServer() == false) {
			return false;
		}
	}
	if (m_bGameLinked == false) {
		LinkGameServer();
	}
	return true;
}
// 定时检测UDP监听登陆服务器的连接对象是否有效
bool CGateServer::CheckUDPService(void)
{
	if (m_krUDPService == nullptr) {
		return StartUDPService();
	}
	return true;
}
// 定时检测监听客户端的连接对象是否有效
bool CGateServer::CheckTCPService(void)
{
	if (m_krTCPService == nullptr) {
		return StartTCPService();
	}
	return true;
}
//--------------------------------------
// 同步服务器信息给界面
bool CGateServer::SyncServerInfo(void)
{
	m_GateRoutine->Update();
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		CGateUpdate Update;
		Update.SetServerData(m_ServerInfo);
		Update.AdjustSize();
		if (m_pShareGameDBSvr != nullptr) {
			m_pShareGameDBSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectGameDB, Update);
		}
		if (m_pShareGameSvr != nullptr) {
			m_pShareGameSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectGame, Update);
		}
		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_GATE);
	}
	else { // 发送心跳
		if (m_pShareGameDBSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_GATE> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectGameDB, Live);
		}
		if (m_pShareGameSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_GATE> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectGame, Live);
		}
	}
	return true;
}



