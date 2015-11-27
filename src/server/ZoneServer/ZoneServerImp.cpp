//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : ZoneServerImp.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 地图服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZoneServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CZoneServer
UInt CZoneServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CZoneServer::Update(void)
{
	CheckConnectGameServer();
	SyncServerInfo();
	return 0;
}
//--------------------------------------
UInt CZoneServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}

bool CZoneServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_CONFIG_PTR, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_ZONE);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[地图服务器]从界面回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[地图服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件
	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[地图服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[地图服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	return true;
}

bool CZoneServer::InitLoadConfig(void)
{
	assert(m_krConnectGame == nullptr);
	assert(m_pShareGameSvr == nullptr);

	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_ZONE);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_ZONE);
	LOGV_INFO(m_FileLog, TF("[地图服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);

	assert(m_ZoneRoutine == nullptr);
	m_ZoneRoutine = (ICommonRoutine*)CRTTI::CreateByName(TF("CZoneRoutine"));
	if (m_ZoneRoutine != nullptr) {
		return (m_ZoneRoutine->Init() == (UInt)RET_OKAY);
	}
	LOG_ERROR(m_FileLog, TF("[地图服务器]创建事务处理对象失败"));
	return false;
}
//--------------------------------------
void CZoneServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务退出处理完成!"));
	}
}

void CZoneServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}

void CZoneServer::ExitUnloadConfig(void)
{
	assert(m_krConnectGame == nullptr);
	assert(m_pShareGameSvr == nullptr);

	m_ZoneRoutine->Exit();
	m_ZoneRoutine = nullptr;
}
//--------------------------------------
bool CZoneServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务启动开始!"));

		if (StartConnectGameServer() == false) {
			return false;
		}
		m_ZoneRoutine->Start(this);

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务启动完成!"));
	}
	return true;
}

bool CZoneServer::StartConnectGameServer(void)
{
	// 地图和游戏在不同进程,  需要连接内网游戏服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAME) == 0) {
		if (m_krConnectGame == nullptr) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_ZONE, 0, strAddr, usPort);
			m_krConnectGame = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectGame != nullptr) {
			LOG_INFO(m_FileLog, TF("[地图服务器]地图服务器和游戏服务器在不同进程, 创建连接游戏服务器对象成功"));
			if (m_bGameCnnted == false) {
				UShort      usPort = 0;
				CStringKey  strAddr;
				m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAME, CServerConfig::CFG_DEFAULT_ZONE, strAddr, usPort);
				if (m_NetworkPtr->Connect(m_krConnectGame, usPort, *strAddr) == false) {
					LOGV_ERROR(m_FileLog, TF("[地图服务器]连接游戏服务器[%s]:%d请求失败"), *strAddr, usPort);
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[地图服务器]连接游戏服务器[%s]:%d请求完成"), *strAddr, usPort);
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[地图服务器]地图服务器和游戏服务器在不同进程, 创建连接游戏服务器对象失败"));
			return false;
		}
	}
	else if (m_bGameCnnted == false) {
		assert(m_pShareGameSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::GameServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[地图服务器]从动态变量表获取同进程游戏服务器对象指针无效"));
			return false;
		}
		m_pShareGameSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bGameCnnted   = true;

		LOG_INFO(m_FileLog, TF("[地图服务器]同进程直接向游戏服务器注册"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CZoneLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();

		UInt uIds[2] = { 1, 100 };

		CBufReadStream brs((PByte)uIds, sizeof(uIds)); // 测试数据
		Link.SetStream(brs);

		m_bGameLinked = m_pShareGameSvr->OnShareRoutine(PAK_EVENT_LINK, Link, reinterpret_cast<uintptr_t>(this));
		m_pUIHandler->OnHandle(PAK_EVENT_LINK, 0, DATA_INDEX_GAME);
		return m_bGameLinked;
	}
	return true;
}
//--------------------------------------
bool CZoneServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[地图服务器]暂停服务"));
		m_ZoneRoutine->Pause();
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[地图服务器]允许服务"));
		m_ZoneRoutine->Pause(false);
		return true;
	}
	return false;
}
//--------------------------------------
void CZoneServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务停止开始!"));

		m_ZoneRoutine->Stop();

		StopConnectGameServer();

		m_ServerInfo.Zero();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[地图服务器]地图服务停止完成!"));
	}
}

void CZoneServer::StopConnectGameServer(void)
{
	if (m_krConnectGame != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectGame, false);
		m_krConnectGame = nullptr;
		LOG_INFO(m_FileLog, TF("[地图服务器]销毁连接游戏服务器成功"));
	}
	m_bGameCnnted   = false;
	m_bGameLinked   = false;
	m_pShareGameSvr = nullptr;
}
//--------------------------------------
bool CZoneServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	assert((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_GAME) != 0);
	switch (nEvent) {
	// game
	case 0:
		{
			m_ZoneRoutine->Add(nEvent, EventRef, llParam, eType);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[地图服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return true;
}
//--------------------------------------
bool CZoneServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = true;
	if (PktPtr->GetEvent() != PAK_EVENT_LIVEACK) {
		assert(PktPtr->GetType() == PAK_TYPE_GAME);
		switch (PktPtr->GetEvent()) {
		case PAK_EVENT_LINKACK:
			{
				m_bGameLinked = true;

				NET_ADDR NetAddr;
				m_NetworkPtr->GetAddr(pTcp->krSocket, NetAddr, false);
				m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&NetAddr), DATA_INDEX_GAME);
				LOG_INFO(m_FileLog, TF("[地图服务器]收到游戏服务器注册回复包"));
			}
			break;
		case PAK_EVENT_UPDATEACK:
			{
				DEV_INFO(TF("[地图服务器]收到游戏服务器更新回复包"));
			}
			break;
		case PAK_EVENT_UNLINKACK:
			{
				m_bGameLinked = false;
				LOG_INFO(m_FileLog, TF("[地图服务器]收到游戏服务器注销回复包"));
			}
			break;
		default:
			{
				LOGV_WARN(m_FileLog, TF("[地图服务器]%p无法识别的游戏服务器信令包数据[event=%d]"), pTcp->krSocket, PktPtr->GetEvent());
			}
		}
	}
	return bRet;
}
//--------------------------------------
bool CZoneServer::OnTcpConnect(UInt uError, KeyRef krConnect)
{
	assert(krConnect == m_krConnectGame);
	m_bGameCnnted = (uError == 0);
	if (m_bGameCnnted) {
		LinkGameServer();
	}
	else {
		DEV_INFO(TF("[地图服务器]连接游戏服务器失败%X!"), uError);
		LOGV_WARN(m_FileLog, TF("[地图服务器]连接游戏服务器失败%X!"), uError);
	}
	return true;
}

void CZoneServer::LinkGameServer(void)
{
	if (m_bGameCnnted && (m_bGameLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CZoneLink Link;
		Link.SetServerData(m_ServerInfo);
		Link.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGame, Link);
	}
}

void CZoneServer::UnlinkGameServer(void)
{
	if (m_bGameCnnted && m_bGameLinked) {
		m_bGameLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_ZONE> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGame, Unlink);
	}
}
//--------------------------------------
bool CZoneServer::OnTcpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krConnectGame) {
		m_krConnectGame = nullptr;
		m_bGameCnnted   = false;
		m_bGameLinked   = false;
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_GAME);
		DEV_INFO(TF("[地图服务器]连接游戏服务器断开"));
		LOG_INFO(m_FileLog, TF("[地图服务器]连接游戏服务器断开"));
	}
	return true;
}
//--------------------------------------
bool CZoneServer::CheckConnectGameServer(void)
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
//--------------------------------------
bool CZoneServer::SyncServerInfo(void)
{
	m_ZoneRoutine->Update();
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		CZoneUpdate Update;
		Update.SetServerData(m_ServerInfo);
		Update.AdjustSize();
		if (m_pShareGameSvr != nullptr) {
			m_pShareGameSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectGame, Update);
		}
		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_ZONE);
	}
	else if (m_pShareGameSvr == nullptr) { // 发送心跳
		CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_ZONE> Live;
		Live.AdjustSize();
		m_NetworkPtr->Send(m_krConnectGame, Live);
	}
	return true;
}




