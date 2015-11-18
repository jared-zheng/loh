//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameServerImp.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameServer
UInt CGameServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CGameServer::Update(void)
{
	CheckConnectCenterServer();
	CheckConnectGameDBServer();
	CheckListenZoneServer();
	CheckListenGateServer();
	CheckUDPService();
	SyncServersInfo();
	return 0;
}
//--------------------------------------
UInt CGameServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}
// 获取共享的配置对象和网络对象
bool CGameServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_DEFAULT_CONFIG, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_GAME);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[游戏服务器]从界面回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[游戏服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件
	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[游戏服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[游戏服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	// 保存游戏服务器指针到动态变量表
	LOGV_INFO(m_FileLog, TF("[游戏服务器]保存游戏服务器指针到动态变量表完成[key=%s]!"), CServerConfig::GameServer);
	m_pConfig->GetKeyValue()->AddItem(CServerConfig::GameServer, this);
	return true;
}
// 初始化配置
bool CGameServer::InitLoadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krConnectGameDB == nullptr);
	assert(m_krListenZone == nullptr);
	assert(m_krListenGate == nullptr);
	assert(m_krUDPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);
	assert(m_pShareGameDBSvr == nullptr);
	assert(m_pShareZoneSvr == nullptr);
	assert(m_pShareGateSvr == nullptr);

	assert(m_ZoneSvrMap.Size() == 0);
	assert(m_ZoneIdMap.Size() == 0);
	assert(m_GateSvrMap.Size() == 0);

	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_GAME);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_GAME);
	LOGV_INFO(m_FileLog, TF("[游戏服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);
	if (m_pConfig->GetNetworkAttr() & ATTR_IPV6) {
		m_ServerInfo.NetAddr[GAMEI_GATE].usAttr        = ATTR_IPV6;
		m_ServerInfo.NetAddr[GAMEI_ZONE].usAttr        = ATTR_IPV6;
		m_ServerInfo.NetAddr[GAMEI_UDP].usAttr         = ATTR_IPV6;
		m_ServerInfo.NetAddr[GAMEI_CENTER].usAttr      = ATTR_IPV6;
		m_ServerInfo.NetAddr[GAMEI_GAMEDB].usAttr      = ATTR_IPV6;
		m_ServerInfo.NetAddr[GAMEI_GAMEDB_ADDR].usAttr = ATTR_IPV6;
	}
	Int        nPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, 0, strAddr, nPort);
	UShort     usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_GATE]); // gate

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_ZONE, strAddr, nPort);
	usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_ZONE]); // zone

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, nPort);
	usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_UDP]); // udp

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_CENTER, strAddr, nPort);
	usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_CENTER]); // connect center 

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_GAMEDB, strAddr, nPort);
	usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_GAMEDB]); // connect gamedb

	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, 0, strAddr, nPort);
	usPort = (UShort)nPort;
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[GAMEI_GAMEDB_ADDR]); // gamedb
	return true;
}
//--------------------------------------
void CGameServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务退出处理完成!"));
	}
}
// 清除共享的配置对象和网络对象
void CGameServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CGameServer::ExitUnloadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krConnectGameDB == nullptr);
	assert(m_krListenZone == nullptr);
	assert(m_krListenGate == nullptr);
	assert(m_krUDPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);
	assert(m_pShareGameDBSvr == nullptr);
	assert(m_pShareZoneSvr == nullptr);
	assert(m_pShareGateSvr == nullptr);

	assert(m_ZoneSvrMap.Size() == 0);
	assert(m_ZoneIdMap.Size() == 0);
	assert(m_GateSvrMap.Size() == 0);

	m_pConfig->GetKeyValue()->Remove(CServerConfig::GameServer);
}
//--------------------------------------
bool CGameServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务启动开始!"));

		if ((StartConnectCenterServer() == false) ||
			(StartConnectGameDBServer() == false) ||
			(StartListenZoneServer() == false) ||
			(StartListenGateServer() == false) ||
			(StartUDPService() == false)) {
			return false;
		}

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务启动完成!"));
	}
	return true;
}
// 运行创建连接中心服务器的连接对象
bool CGameServer::StartConnectCenterServer(void)
{
	// 游戏和中心在不同进程,  需要连接内网中心服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) == 0) {
		if (m_krConnectCenter == nullptr) {
			m_krConnectCenter = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GAMEI_CENTER]);
		}
		if (m_krConnectCenter != nullptr) {
			if (m_bCenterCnnted == false) {
				LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务器和中心服务器在不同进程, 创建连接中心服务器Socket成功"));
				Int         nPort = 0;
				CStringKey  strAddr;
				m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_GAME, strAddr, nPort);
				if (m_NetworkPtr->Connect(m_krConnectCenter, (UShort)nPort, *strAddr) == false)
				{
					LOGV_ERROR(m_FileLog, TF("[游戏服务器]连接中心服务器[%s]:%d请求失败"), *strAddr, nPort);
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[游戏服务器]连接中心服务器[%s]:%d请求完成"), *strAddr, nPort);
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]游戏服务器和中心服务器在不同进程, 创建连接中心服务器Socket失败"));
			return false;
		}
	}
	else if (m_bCenterCnnted == false) {
		assert(m_pShareCenterSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::CenterServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]从动态变量表获取同进程中心服务器对象指针无效"));
			return false;
		}
		m_pShareCenterSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bCenterCnnted   = true;

		LOG_INFO(m_FileLog, TF("[游戏服务器]同进程直接连接中心服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;
		m_ServerInfo.NetAddr[GAMEI_CENTER].usPort = 0; // 0 == 同进程共享

		CGameLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		CBufReadStream brs((PByte)m_ServerInfo.NetAddr, sizeof(m_ServerInfo.NetAddr));
		Link.SetStream(brs);

		m_bCenterLinked = m_pShareCenterSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<uintptr_t>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_CENTER);
		return m_bCenterLinked;
	}
	return true;
}
// 运行创建连接游戏DB服务器的连接对象
bool CGameServer::StartConnectGameDBServer(void)
{
	// 游戏和游戏DB在不同进程,  需要连接内网游戏DB服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAMEDB) == 0) {
		if (m_krConnectGameDB == nullptr) {
			m_krConnectGameDB = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GAMEI_GAMEDB]);
		}
		if (m_krConnectGameDB != nullptr) {
			if (m_bGameDBCnnted == false) {
				LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务器和游戏DB服务器在不同进程, 创建连接游戏DB服务器Socket成功"));
				if (m_NetworkPtr->Connect(m_krConnectGameDB, m_ServerInfo.NetAddr[GAMEI_GAMEDB_ADDR]) == false)
				{
					LOGV_ERROR(m_FileLog, TF("[游戏服务器]连接游戏DB服务器请求失败"));
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[游戏服务器]连接游戏DB服务器请求完成"));
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]游戏服务器和游戏DB服务器在不同进程, 创建连接游戏DB服务器Socket失败"));
			return false;
		}
	}
	else if (m_bGameDBCnnted == false) {
		assert(m_pShareGameDBSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::GameDBServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]从动态变量表获取同进程游戏DB服务器对象指针无效"));
			return false;
		}
		m_pShareGameDBSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bGameDBCnnted   = true;

		LOG_INFO(m_FileLog, TF("[游戏服务器]同进程直接连接游戏DB服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;
		m_ServerInfo.NetAddr[GAMEI_GAMEDB].usPort = 0; // 0 == 同进程共享

		CGameLink Link;
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
// 运行创建监听地图服务器连接的连接对象
bool CGameServer::StartListenZoneServer(void)
{
	if (m_krListenZone != nullptr) {
		LOGV_INFO(m_FileLog, TF("[游戏服务器]创建监听地图服务器的连接已经存在"));
		return true;
	}
	m_krListenZone = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GAMEI_ZONE]);
	if (m_krListenZone != nullptr) {
		LOGV_INFO(m_FileLog, TF("[游戏服务器]创建监听地图服务器的连接成功"));
		if (m_NetworkPtr->Listen(m_krListenZone) == false) {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]监听地图服务器连接失败"));
			return false;
		}
		else {
			LOG_INFO(m_FileLog, TF("[游戏服务器]监听地图服务器连接成功"));
		}
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[游戏服务器]创建监听地图服务器的连接失败"));
		return false;
	}
	return true;
}
// 运行创建监听网关服务器连接的连接对象
bool CGameServer::StartListenGateServer(void)
{
	if (m_krListenGate != nullptr) {
		LOGV_INFO(m_FileLog, TF("[游戏服务器]创建监听网关服务器的连接已经存在"));
		return true;
	}
	m_krListenGate = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GAMEI_GATE]);
	if (m_krListenGate != nullptr) {
		LOGV_INFO(m_FileLog, TF("[游戏服务器]创建监听网关服务器的连接成功"));
		if (m_NetworkPtr->Listen(m_krListenGate) == false) {
			LOG_ERROR(m_FileLog, TF("[游戏服务器]监听网关服务器连接失败"));
			return false;
		}
		else {
			LOG_INFO(m_FileLog, TF("[游戏服务器]监听网关服务器连接成功"));
		}
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[游戏服务器]创建监听网关服务器的连接失败"));
		return false;
	}
	return true;
}
// 运行创建UDP监听登陆服务器校验连接对象
bool CGameServer::StartUDPService(void)
{
	if (m_krUDPService == nullptr) {
		m_krUDPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[GAMEI_UDP], SOCKET_UDP);
		if (m_krUDPService != nullptr) {
			LOGV_INFO(m_FileLog, TF("[游戏服务器]创建UDP监听登陆服务器成功"));
		}
		else {
			LOGV_ERROR(m_FileLog, TF("[游戏服务器]创建UDP监听登陆服务器失败"));
			return false;
		}
	}
	return (m_krUDPService != nullptr);
}
//--------------------------------------
bool CGameServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[游戏服务器]暂停服务"));
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[游戏服务器]允许服务"));
		return true;
	}
	return false;
}
//--------------------------------------
void CGameServer::Stop(void)
{
	if (m_nStatus > STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务停止开始!"));

		StopUDPService();
		StopListenGateServer();
		StopListenZoneServer();
		StopConnectGameDBServer();
		StopConnectCenterServer();

		m_ServerInfo.Zero();
		m_OthersInfo[INFOI_ZONE].Reset();
		m_OthersInfo[INFOI_GATE].Reset();
		m_ZoneSvrMap.RemoveAll();
		m_ZoneIdMap.RemoveAll();
		m_GateSvrMap.RemoveAll();
		m_SortChain.Reset();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务停止完成!"));
	}
}
// 停止连接中心服务器
void CGameServer::StopConnectCenterServer(void)
{
	if (m_krConnectCenter != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectCenter, false);
		m_krConnectCenter = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏服务器]销毁连接中心服务器的连接成功"));
	}
	m_bCenterCnnted   = false;
	m_bCenterLinked   = false;
	m_pShareCenterSvr = nullptr;
}
// 停止连接游戏DB服务器
void CGameServer::StopConnectGameDBServer(void)
{
	if (m_krConnectGameDB != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectGameDB, false);
		m_krConnectGameDB = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏服务器]销毁连接游戏DB服务器的连接成功"));
	}
	m_bGameDBCnnted   = false;
	m_bGameDBLinked   = false;
	m_pShareGameDBSvr = nullptr;
}
// 停止监听地图服务器连接
void CGameServer::StopListenZoneServer(void)
{
	if (m_krListenZone != nullptr) {
		m_NetworkPtr->Destroy(m_krListenZone, false);
		m_krListenZone = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏服务器]销毁监听地图服务器的连接成功"));
	}
	m_pShareZoneSvr = nullptr;
}
// 停止监听网关服务器连接
void CGameServer::StopListenGateServer(void)
{
	if (m_krListenGate != nullptr) {
		m_NetworkPtr->Destroy(m_krListenGate, false);
		m_krListenGate = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏服务器]销毁监听网关服务器的连接成功"));
	}
	m_pShareGateSvr = nullptr;
}
// 停止UDP监听登陆服务器
void CGameServer::StopUDPService(void)
{
	if (m_krUDPService != nullptr) {
		m_NetworkPtr->Destroy(m_krUDPService, false);
		m_krUDPService = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏服务器]销毁UDP服务的连接成功"));
	}
}
//--------------------------------------
bool CGameServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE)
{
	assert((m_pConfig->GetLoadServers() & (CServerConfig::CFG_DEFAULT_CENTER|CServerConfig::CFG_DEFAULT_GAMEDB|CServerConfig::CFG_DEFAULT_ZONE|CServerConfig::CFG_DEFAULT_GATE)) != 0);
	switch (nEvent) {
	case PAK_EVENT_LINK:
		{
			return OnShareLink(EventRef, llParam);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			return OnShareUpdate(EventRef, llParam);
		}
		break;
	//case PAK_EVENT_UNLINK: // 同进程服务器断连, 说明服务停止, 不需要处理
	//	break;
	case PAK_EVENT_LOGIN_SELECT_GAME:
		{
			CPAKLoginSelectGame* pSelect = static_cast<CPAKLoginSelectGame*>(&EventRef);
			if (SelectGame(pSelect, llParam) == false) {
				pSelect->SetAck(LOGIN_ERROR_NONE_GATE);
				reinterpret_cast<ICommonServer*>(llParam)->OnShareRoutine(PAK_EVENT_LOGIN_SELECT_GAME, *pSelect);
				DEV_INFO(TF("[游戏服务器]同进程游戏服务器收到客户端登陆游戏服务器请求, 但是没有网关服务器"));
			}
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return true;
}
// 同进程服务器处理
bool CGameServer::OnShareLink(CEventBase& EventRef, LLong llParam)
{
	CPAKLink* pLink = static_cast<CPAKLink*>(&EventRef);
	if (pLink->GetType() == PAK_TYPE_ZONE) {
		if (m_pShareZoneSvr == nullptr) {
			// 0.获得共享指针
			m_pShareZoneSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[游戏服务器]同进程地图服务器[%p]连接"), m_pShareZoneSvr);
			return ServerLink<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(pLink, (DataRef)llParam, m_ZoneSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏服务器]同进程地图服务器对象已经存在[%p-%p]"), m_pShareZoneSvr, llParam);
		}
	}
	else if (pLink->GetType() == PAK_TYPE_GATE) {
		if (m_pShareGateSvr == nullptr) {
			// 0.获得共享指针
			m_pShareGateSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[游戏服务器]同进程网关服务器[%p]连接"), m_pShareGateSvr);
			return ServerLink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pLink, (DataRef)llParam, m_GateSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏服务器]同进程网关服务器对象已经存在[%p-%p]"), m_pShareGateSvr, llParam);
		}
	}
	return false;
}

bool CGameServer::OnShareUpdate(CEventBase& EventRef, LLong llParam)
{
	CPAKUpdate* pUpdate = static_cast<CPAKUpdate*>(&EventRef);
	if (pUpdate->GetType() == PAK_TYPE_ZONE) {
		if (m_pShareZoneSvr != nullptr) {
			DEV_INFO(TF("[游戏服务器]同进程地图服务器[%p]更新"), m_pShareZoneSvr);
			assert(m_pShareZoneSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(pUpdate, (DataRef)llParam, m_ZoneSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏服务器]同进程地图服务器对象不存在"));
		}
	}
	else if (pUpdate->GetType() == PAK_TYPE_GATE) {
		if (m_pShareGateSvr != nullptr) {
			DEV_INFO(TF("[游戏服务器]同进程网关服务器[%p]更新"), m_pShareGateSvr);
			assert(m_pShareGateSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pUpdate, (DataRef)llParam, m_GateSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏服务器]同进程网关服务器对象不存在"));
		}
	}
	return false;
}
//--------------------------------------
bool CGameServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	if (PktPtr->GetEvent() != PAK_EVENT_LIVE) {
		switch (PktPtr->GetType()) {
		case PAK_TYPE_GATE:
			{
				bRet = DispatchGateServer(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_ZONE:
			{
				bRet = DispatchZoneServer(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_GAMEDB:
			{
				bRet = DispatchGameDBServer(PktPtr, pTcp->krSocket);
			}
			break;
		case PAK_TYPE_CENTER:
			{
				bRet = DispatchCenterServer(PktPtr, pTcp->krSocket);
			}
			break;
		default:
			{
				LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信令包服务器类型[%d]不正确"), pTcp->krSocket, PktPtr->GetType());
				bRet = false;
			}
		}
	}
	else {
		CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_GAME> LiveAck;
		LiveAck.AdjustSize();
		bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
	}
	return bRet;
}
// 处理中心服务器的信令包
bool CGameServer::DispatchCenterServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bCenterLinked = true;
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(m_ServerInfo.NetAddr + GAMEI_CENTER), DATA_INDEX_CENTER);
			LOG_INFO(m_FileLog, TF("[游戏服务器]收到中心服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[游戏服务器]收到中心服务器更新回复包"));
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bCenterLinked = false;
			LOG_INFO(m_FileLog, TF("[游戏服务器]收到中心服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信息无法识别的中心服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
// 处理游戏DB服务器的信令包
bool CGameServer::DispatchGameDBServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bGameDBLinked = true;
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(m_ServerInfo.NetAddr + GAMEI_GAMEDB), DATA_INDEX_GAMEDB);
			LOG_INFO(m_FileLog, TF("[游戏服务器]收到游戏DB服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[游戏服务器]收到游戏DB服务器更新回复包"));
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bGameDBLinked = false;
			LOG_INFO(m_FileLog, TF("[游戏服务器]收到游戏DB服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信息无法识别的游戏DB服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
// 处理地图服务器的信令包
bool CGameServer::DispatchZoneServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	bool bRet = false;
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINK:
		{
			bRet = OnServerLink(static_cast<CPAKLink*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			bRet = OnServerUpdate(static_cast<CPAKUpdate*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_UNLINK:
		{
			bRet = OnServerUnlink(PktPtr.Get(), krSocket);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信息无法识别的地图服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return bRet;
}
// 处理网关服务器的信令包
bool CGameServer::DispatchGateServer(const PacketPtr& PktPtr, KeyRef krSocket)
{
	bool bRet = false;
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINK:
		{
			bRet = OnServerLink(static_cast<CPAKLink*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			bRet = OnServerUpdate(static_cast<CPAKUpdate*>(PktPtr.Get()), krSocket);
		}
		break;
	case PAK_EVENT_UNLINK:
		{
			bRet = OnServerUnlink(PktPtr.Get(), krSocket);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信息无法识别的网关服务器信令包数据[event=%d]"), krSocket, PktPtr->GetEvent());
		}
	}
	return bRet;
}
// 服务器处理
bool CGameServer::OnServerLink(CPAKLink* pLink, KeyRef krSocket)
{
	bool bRet = false;
	switch (pLink->GetType()) {
	case PAK_TYPE_ZONE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏服务器]地图服务器[%p]连接"), krSocket);
			bRet = ServerLink<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(pLink, (DataRef)krSocket, m_ZoneSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏服务器]网关服务器[%p]连接"), krSocket);
			bRet = ServerLink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pLink, (DataRef)krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p连接信息无法识别的%X服务器信令包数据"), krSocket, pLink->GetType());
		}
	}
	if (bRet) {
		// 3.设置连接为选择服务器属性
		m_NetworkPtr->SetAttr(krSocket, pLink->GetType());
		// 4.发送连接回执包
		CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_GAME> LinkAck;
		LinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, LinkAck);
	}
	return bRet;
}

bool CGameServer::OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUpdate->GetType()) {
	case PAK_TYPE_ZONE:
		{
			DEV_INFO(TF("[游戏服务器]地图服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(pUpdate, (DataRef)krSocket, m_ZoneSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			DEV_INFO(TF("[游戏服务器]网关服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pUpdate, (DataRef)krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p更新信息无法识别的%X服务器信令包数据"), krSocket, pUpdate->GetType());
		}
	}
	if (bRet) {
		// 发送更新回执包
		CPAKSimple<PAK_EVENT_UPDATEACK, PAK_TYPE_GAME> UpdateAck;
		UpdateAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UpdateAck);
	}
	return bRet;
}

bool CGameServer::OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUnlink->GetType()) {
	case PAK_TYPE_ZONE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏服务器]地图服务器[%p]断接"), krSocket);
			bRet = ServerUnlink<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(krSocket, m_ZoneSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏服务器]网关服务器[%p]断接"), krSocket);
			bRet = ServerUnlink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏服务器]%p断连信息无法识别的%X服务器信令包数据"), krSocket, pUnlink->GetType());
		}
	}
	if (bRet) {
		// 3.设置连接为无效服务器属性
		m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_NONE);
		// 4.发送断连回执包
		CPAKSimple<PAK_EVENT_UNLINKACK, PAK_TYPE_GAME> UnlinkAck;
		UnlinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UnlinkAck);
	}
	return bRet;
}
//--------------------------------------
bool CGameServer::OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	assert(PktPtr->GetEvent() == PAK_EVENT_LOGIN_SELECT_GAME);
	CPAKLoginSelectGame* pSelect = static_cast<CPAKLoginSelectGame*>(PktPtr.Get());
	if (SelectGame(pSelect) == false) {
		pSelect->SetAck(LOGIN_ERROR_NONE_GATE);
		m_NetworkPtr->SendTo(pUdp->krSocket, *pSelect, pUdp->NetAddr);
		LOG_INFO(m_FileLog, TF("[游戏服务器]游戏服务器收到客户端登陆游戏服务器请求, 但是没有网关服务器"));
	}
	return true;
}

bool CGameServer::SelectGame(CPAKLoginSelectGame* pSelect, LLong llParam)
{
	assert(pSelect->GetType() == PAK_TYPE_LOGIN);
	pSelect->SetType(PAK_TYPE_GAME);
	// 实际需要按电信或者网通线路PING值为参考来排序
	PSORT_RESULT pSortResult = m_SortChain.GetCur();
	if (pSortResult->lCount > 0) {
		Long lIndex = pSortResult->Index();
		if ((DataRef)m_pShareGateSvr == pSortResult->Sort[lIndex].drServer) {
			m_pShareGateSvr->OnShareRoutine(PAK_EVENT_LOGIN_SELECT_GAME, *pSelect, llParam);
		}
		else {
			m_NetworkPtr->SendTo(m_krUDPService, *pSelect, pSortResult->Sort[lIndex].Addr);
		}
		DEV_INFO(TF("[游戏服务器]选取第%d个网关服务器"), lIndex);
		return true;
	}
	return false;
}
//--------------------------------------
bool CGameServer::OnTcpAccept(KeyRef krAccept, KeyRef krListen)
{
	if (m_nStatus == STATUSC_RUN) {
		UShort     usPort = 0;
		CStringKey strAddr;
		m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);

		if (krListen == m_krListenZone) {
			DEV_INFO(TF("[游戏服务器]地图服务器[%s]:%d连接完成!"), *strAddr, usPort);
		}
		else if (krListen == m_krListenGate) {
			DEV_INFO(TF("[游戏服务器]网关服务器[%s]:%d连接完成!"), *strAddr, usPort);
		}
		m_ServerInfo.Incr();
		return true;
	}
	else {
		LOGV_WARN(m_FileLog, TF("[游戏服务器]游戏服务器处于暂停状态!"));
	}
	return false;
}
//--------------------------------------
bool CGameServer::OnTcpConnect(UInt uError, KeyRef krConnect)
{
	if (krConnect == m_krConnectCenter) {
		m_bCenterCnnted = (uError == 0);
		if (m_bCenterCnnted) {
			m_NetworkPtr->GetAddr(krConnect, m_ServerInfo.NetAddr[GAMEI_CENTER], false);
		}
		else {
			DEV_INFO(TF("[游戏服务器]连接中心服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[游戏服务器]连接中心服务器失败%X!"), uError);
		}
	}
	else if (krConnect == m_krConnectGameDB) {
		m_bGameDBCnnted = (uError == 0);
		if (m_bGameDBCnnted) {
			m_NetworkPtr->GetAddr(krConnect, m_ServerInfo.NetAddr[GAMEI_GAMEDB], false);
		}
		else {
			DEV_INFO(TF("[游戏服务器]连接游戏DB服务器失败%X!"), uError);
			LOGV_WARN(m_FileLog, TF("[游戏服务器]连接游戏DB服务器失败%X!"), uError);
		}
	}
	if (m_bCenterCnnted && m_bGameDBCnnted) {
		LinkCenterServer();
		LinkGameDBServer();
	}
	return true;
}
// 连接中心服务器
void CGameServer::LinkCenterServer(void)
{
	if (m_bCenterCnnted && (m_bCenterLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGameLink Link;
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
			DEV_WARN(TF("[游戏服务器]连接中心服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[游戏服务器]连接中心服务器创建网络数据流异常!"));
		}
	}
}
// 断连中心服务器
void CGameServer::UnlinkCenterServer(void)
{
	if (m_bCenterCnnted && m_bCenterLinked) {
		m_bCenterLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_GAME> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectCenter, Unlink);
	}
}
// 连接游戏DB服务器
void CGameServer::LinkGameDBServer(void)
{
	if (m_bGameDBCnnted && (m_bGameDBLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CGameLink Link;
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
			DEV_WARN(TF("[游戏服务器]连接游戏DB服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[游戏服务器]连接游戏DB服务器创建网络数据流异常!"));
		}
	}
}
// 断连游戏DB服务器
void CGameServer::UnlinkGameDBServer(void)
{
	if (m_bGameDBCnnted && m_bGameDBLinked)
	{
		m_bGameDBLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_GAME> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGameDB, Unlink);
	}
}
//--------------------------------------
bool CGameServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krConnectCenter) {
		m_krConnectCenter = nullptr;
		m_bCenterCnnted   = false;
		m_bCenterLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_CENTER);
		DEV_INFO(TF("[游戏服务器]连接中心服务器连接断连/断开"));
	}
	else if (krSocket == m_krConnectGameDB) {
		m_krConnectGameDB = nullptr;
		m_bGameDBCnnted   = false;
		m_bGameDBLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_GAMEDB);
		DEV_INFO(TF("[游戏服务器]连接游戏DB服务器连接断连/断开"));
	}
	else if (krSocket == m_krListenZone) {
		m_krListenZone = nullptr;
		DEV_INFO(TF("[游戏服务器]监听地图服务器连接关闭"));
	}
	else if (krSocket == m_krListenGate) {
		m_krListenGate = nullptr;
		DEV_INFO(TF("[游戏服务器]监听网关服务器连接关闭"));
	}
	else { // 单个服务器断连/断开
		switch (llLiveData) {
		case PAK_TYPE_ZONE:
			{
				DEV_INFO(TF("[游戏DB服务器]地图服务器[%p]断接"), krSocket);
				ServerUnlink<SVR_ZONE_INDEX_MAP, DATA_INDEX_ZONE, INFOI_ZONE>(krSocket, m_ZoneSvrMap);
			}
			break;
		case PAK_TYPE_GATE:
			{
				DEV_INFO(TF("[游戏DB服务器]网关服务器[%p]断接"), krSocket);
				ServerUnlink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(krSocket, m_GateSvrMap);
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
bool CGameServer::OnUdpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krUDPService){
		m_krUDPService = nullptr;
		DEV_INFO(TF("[游戏服务器]UDP监听登陆服务器关闭"));
	}
	return true;
}
//--------------------------------------
// 定时检测连接中心服务器的连接对象是否有效
bool CGameServer::CheckConnectCenterServer(void)
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
// 定时检测连接游戏DB服务器的连接对象是否有效
bool CGameServer::CheckConnectGameDBServer(void)
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
// 定时检测监听地图服务器连接的连接对象是否有效
bool CGameServer::CheckListenZoneServer(void)
{
	if (m_krListenZone == nullptr) {
		return StartListenZoneServer();
	}
	return true;
}
// 定时检测监听网关服务器连接的连接对象是否有效
bool CGameServer::CheckListenGateServer(void)
{
	if (m_krListenGate == nullptr) {
		return StartListenGateServer();
	}
	return true;
}
// 定时检测UDP监听游戏服务器的连接对象是否有效
bool CGameServer::CheckUDPService(void)
{
	if (m_krUDPService == nullptr) {
		return StartUDPService();
	}
	return true;
}
//--------------------------------------
// 同步服务器信息给界面
bool CGameServer::SyncServersInfo(void)
{
	SyncGameServerInfo();
	SyncZoneServerInfo();
	SyncGateSortInfo();
	return true;
}
// 同步游戏服务器信息给界面
bool CGameServer::SyncGameServerInfo(void)
{
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		CGameUpdate Update;
		Update.SetServerData(m_ServerInfo);
		Update.AdjustSize();
		if (m_pShareCenterSvr != nullptr) {
			m_pShareCenterSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectCenter, Update);
		}
		if (m_pShareGameDBSvr != nullptr) {
			m_pShareGameDBSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectGameDB, Update);
		}
		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_GAME);
	}
	else { // 发送心跳
		if (m_pShareCenterSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_GAME> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectCenter, Live);
		}
		if (m_pShareGameDBSvr == nullptr) { 
			CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_GAME> Live;
			Live.AdjustSize();
			m_NetworkPtr->Send(m_krConnectGameDB, Live);
		}
	}
	return true;
}
// 同步地图服务器信息给界面
bool CGameServer::SyncZoneServerInfo(void)
{
	if (m_OthersInfo[INFOI_ZONE].usStatus == STATUSU_SYNC) {
		m_OthersInfo[INFOI_ZONE].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_OthersInfo + INFOI_ZONE), DATA_INDEX_ZONE);
	}
	return true;
}
// 同步网关服务器负载情况数组并同步给界面
bool CGameServer::SyncGateSortInfo(void)
{ 
	if (m_OthersInfo[INFOI_GATE].usStatus == STATUSU_SYNC) {
		m_OthersInfo[INFOI_GATE].usStatus = STATUSU_OKAY;
		m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_OthersInfo + INFOI_GATE), DATA_INDEX_GATE);

		Int nCount = 0;
		PSORT_RESULT pSortResult = m_SortChain.GetNext();
		{
			CSyncLockWaitScope scope(m_GateSvrMap.GetLock());

			PINDEX index = m_GateSvrMap.First();
			while ((index != nullptr) && (nCount < SORT_RESULT::SORTC_LIMIT)) {
				SVR_GATE_MAP::SVR_MAP_PAIR* pPair = m_GateSvrMap.Next(index);
				if ((pPair->m_V.usStatus > STATUSU_NONE) && (pPair->m_V.usStatus < STATUSU_UNLINK)) {
					pSortResult->Sort[nCount].usBusy   = pPair->m_V.usBusy;
					pSortResult->Sort[nCount].usIncr   = pPair->m_V.usIncr;
					pSortResult->Sort[nCount].drServer = pPair->m_K;
					pSortResult->Sort[nCount].Addr     = pPair->m_V.NetAddr[GATEI_UDP];
					++nCount;
				}
			}
		}
		if (nCount > 1) {
			// 实际需要按电信或者网通线路PING值为参考来排序
			if (nCount > 2) {
				SERVER_SORT::QuickSort(pSortResult->Sort, nCount);
			}
			else if (pSortResult->Sort[0].usBusy > pSortResult->Sort[1].usBusy) {
				SERVER_SORT ss = pSortResult->Sort[0];
				pSortResult->Sort[0] = pSortResult->Sort[1];
				pSortResult->Sort[1] = ss;
			}
		}
		pSortResult->lCount = (Long)nCount;
		pSortResult->lIndex = 0;
		m_SortChain.Swap(); // 交换网关排序的数据, 将当前更新的作为有效数据
		LOGV_INFO(m_FileLog, TF("[游戏服务器]更新网关服务器负载情况数组有%d个网关服务器信息"), nCount);
	}
	return true;
}

bool CGameServer::AddZoneServerIds(CStream& Stream, PINDEX index, DataRef drServer)
{
	Int nSize = 0;
	Stream >> nSize;
	if (nSize <= 0) {
		return false;
	}
	CSyncLockWaitScope scope(m_ZoneIdMap.GetLock()); // 先锁定server-map， 再锁定id-map
	Int nCount = 0;
	for (; nCount < nSize; ++nCount) {
		UInt uId = (UInt)-1;
		Stream >> uId;
		if (uId != (UInt)-1) {
			assert(m_ZoneIdMap.Find(uId) == nullptr);
			m_ZoneSvrMap.At(index)->m_V.Index.Add(m_ZoneIdMap.Add(uId, drServer));
			continue;
		}
		break;
	}
	return (nCount == nSize);
}

bool CGameServer::RemoveZoneServerIds(PINDEX index)
{
	ZONE_ADDR_INDEX::ARY_INDEX& Index = m_ZoneSvrMap.At(index)->m_V.Index;
	if (Index.GetSize() == 0) {
		return true;
	}
	CSyncLockWaitScope scope(m_ZoneIdMap.GetLock()); // 先锁定server-map， 再锁定id-map
	Int nCount = Index.GetSize();
	for (Int i = 0; i < nCount; ++i) {
		m_ZoneIdMap.RemoveAt(Index[i]);
	}
	Index.RemoveAll();
	return true;
}





