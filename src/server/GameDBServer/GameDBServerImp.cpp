//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameDBServerImp.cpp                          //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏DB服务器管理实现                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDBServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBServer
UInt CGameDBServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CGameDBServer::Update(void)
{
	CheckListenGameServer();
	CheckListenGateServer();
	SyncServerInfo();
	return 0;
}
//--------------------------------------
UInt CGameDBServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}
// 获取共享的配置对象和网络对象
bool CGameDBServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_DEFAULT_CONFIG, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_GAMEDB);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[游戏DB服务器]从界面回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[游戏DB服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件
	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[游戏DB服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[游戏DB服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	// 保存游戏DB服务器指针到动态变量表
	LOGV_INFO(m_FileLog, TF("[游戏DB服务器]保存游戏DB服务器指针到动态变量表完成[key=%s]!"), CServerConfig::GameDBServer);
	m_pConfig->GetKeyValue()->AddItem(CServerConfig::GameDBServer, this);
	return true;
}
// 初始化配置
bool CGameDBServer::InitLoadConfig(void)
{
	assert(m_krListenGame == nullptr);
	assert(m_krListenGate == nullptr);
	assert(m_pShareGameSvr == nullptr);
	assert(m_pShareGateSvr == nullptr);

	assert(m_GameSvrMap.Size() == 0);
	assert(m_GateSvrMap.Size() == 0);

	m_ServerInfo[INFOI_GAMEDB].usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_GAMEDB);
	m_ServerInfo[INFOI_GAMEDB].usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_GAMEDB);
	LOGV_INFO(m_FileLog, TF("[游戏DB服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo[INFOI_GAMEDB].usId, m_ServerInfo[INFOI_GAMEDB].usIncr);
	assert(m_GameDBRoutine == nullptr);
	m_GameDBRoutine = (ICommonRoutine*)CRTTI::CreateByName(TF("CGameDBRoutine"));
	if (m_GameDBRoutine != nullptr) {
		return (m_GameDBRoutine->Init() == (UInt)RET_OKAY);
	}
	LOG_ERROR(m_FileLog, TF("[游戏DB服务器]创建事务处理对象失败"));
	return false;
}
//--------------------------------------
void CGameDBServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务退出处理完成!"));
	}
}
// 清除共享的配置对象和网络对象
void CGameDBServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CGameDBServer::ExitUnloadConfig(void)
{
	assert(m_krListenGame == nullptr);
	assert(m_krListenGate == nullptr);
	assert(m_pShareGameSvr == nullptr);
	assert(m_pShareGateSvr == nullptr);

	assert(m_GameSvrMap.Size() == 0);
	assert(m_GateSvrMap.Size() == 0);
	LOGV_INFO(m_FileLog, TF("[游戏DB服务器]从动态变量表清除游戏DB服务器指针完成[key=%s]!"), CServerConfig::GameDBServer);
	m_pConfig->GetKeyValue()->Remove(CServerConfig::GameDBServer);

	m_GameDBRoutine->Exit();
	m_GameDBRoutine = nullptr;
}
//--------------------------------------
bool CGameDBServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务启动开始!"));

		if ((StartListenGameServer() == false) ||
			(StartListenGateServer() == false)){
			return false;
		}
		m_GameDBRoutine->Start(this);

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务启动完成!"));
	}
	return true;
}
// 运行创建监听游戏服务器连接的连接对象
bool CGameDBServer::StartListenGameServer(void)
{
	Int        nPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, 0, strAddr, nPort);

	assert(m_krListenGame == nullptr);
	bool bRet = true;
	m_krListenGame = m_NetworkPtr->Create(*this, (UShort)nPort, *strAddr);
	if (m_krListenGame != nullptr) {
		bRet = m_NetworkPtr->Listen(m_krListenGame);
		LOGV_INFO(m_FileLog, TF("[游戏DB服务器]创建监听游戏服务器的连接[%s]:%d成功, %s"), *strAddr, nPort, bRet ? TF("监听连接成功") : TF("监听连接失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[游戏DB服务器]创建监听游戏服务器的连接[%s]:%d失败"), *strAddr, nPort);
		bRet = false;
	}
	return bRet;
}
// 运行创建监听网关服务器连接的连接对象
bool CGameDBServer::StartListenGateServer(void)
{
	Int        nPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_GAMEDB, CServerConfig::CFG_DEFAULT_GATE, strAddr, nPort);

	assert(m_krListenGate == nullptr);
	bool bRet = true;
	m_krListenGate = m_NetworkPtr->Create(*this, (UShort)nPort, *strAddr);
	if (m_krListenGate != nullptr) {
		bRet = m_NetworkPtr->Listen(m_krListenGate);
		LOGV_INFO(m_FileLog, TF("[游戏DB服务器]创建监听网关服务器的连接[%s]:%d成功, %s"), *strAddr, nPort, bRet ? TF("监听连接成功") : TF("监听连接失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[游戏DB服务器]创建监听网关服务器的连接[%s]:%d失败"), *strAddr, nPort);
		bRet = false;
	}
	return bRet;
}
//--------------------------------------
bool CGameDBServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]暂停服务"));
		m_GameDBRoutine->Pause();
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]允许服务"));
		m_GameDBRoutine->Pause(false);
		return true;
	}
	return false;
}
//--------------------------------------
void CGameDBServer::Stop(void)
{
	if (m_nStatus > STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务停止开始!"));

		m_GameDBRoutine->Stop();

		StopListenGateServer();
		StopListenGameServer();

		m_ServerInfo[INFOI_GAMEDB].Zero();
		m_ServerInfo[INFOI_GAME].Reset();
		m_ServerInfo[INFOI_GATE].Reset();
		m_GameSvrMap.RemoveAll();
		m_GateSvrMap.RemoveAll();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]游戏DB服务停止完成!"));
	}
}
// 停止监听游戏服务器连接
void CGameDBServer::StopListenGameServer(void)
{
	if (m_krListenGame != nullptr) {
		m_NetworkPtr->Destroy(m_krListenGame, false);
		m_krListenGame = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]销毁监听游戏服务器的连接成功"));
	}
	m_pShareGameSvr = nullptr;
}
// 停止监听网关服务器连接
void CGameDBServer::StopListenGateServer(void)
{
	if (m_krListenGate != nullptr) {
		m_NetworkPtr->Destroy(m_krListenGate, false);
		m_krListenGate = nullptr;
		LOG_INFO(m_FileLog, TF("[游戏DB服务器]销毁监听网关服务器的连接成功"));
	}
	m_pShareGateSvr = nullptr;
}
//--------------------------------------
bool CGameDBServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	assert((m_pConfig->GetLoadServers() & (CServerConfig::CFG_DEFAULT_GAME| CServerConfig::CFG_DEFAULT_GATE)) != 0);
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
	case PAK_EVENT_GATE_SELECT:
	case PAK_EVENT_GATE_PLAY:
	case PAK_EVENT_GATE_CREATE:
	case PAK_EVENT_GATE_DELETE:
	case PAK_EVENT_GATE_RECOVER:
		{
			m_GameDBRoutine->Add(nEvent, EventRef, llParam, eType);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return true;
}
// 同进程服务器处理
bool CGameDBServer::OnShareLink(CEventBase& EventRef, LLong llParam)
{
	CPAKLink* pLink = static_cast<CPAKLink*>(&EventRef);
	if (pLink->GetType() == PAK_TYPE_GAME) {
		if (m_pShareGameSvr == nullptr) {
			// 0.获得共享指针
			m_pShareGameSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]同进程游戏服务器[%p]连接"), m_pShareGameSvr);
			return ServerLink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pLink, (DataRef)llParam, m_GameSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]同进程游戏服务器对象已经存在[%p-%p]"), m_pShareGameSvr, llParam);
		}
	}
	else if (pLink->GetType() == PAK_TYPE_GATE) {
		if (m_pShareGateSvr == nullptr) {
			// 0.获得共享指针
			m_pShareGateSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]同进程网关服务器[%p]连接"), m_pShareGateSvr);
			return ServerLink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pLink, (DataRef)llParam, m_GateSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]同进程网关服务器对象已经存在[%p-%p]"), m_pShareGateSvr, llParam);
		}
	}
	return false;
}

bool CGameDBServer::OnShareUpdate(CEventBase& EventRef, LLong llParam)
{
	CPAKUpdate* pUpdate = static_cast<CPAKUpdate*>(&EventRef);
	if (pUpdate->GetType() == PAK_TYPE_GAME) {
		if (m_pShareGameSvr != nullptr) {
			DEV_INFO(TF("[游戏DB服务器]同进程游戏服务器[%p]更新"), m_pShareGameSvr);
			assert(m_pShareGameSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pUpdate, (DataRef)llParam, m_GameSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]同进程游戏服务器对象不存在"));
		}
	}
	else if (pUpdate->GetType() == PAK_TYPE_GATE) {
		if (m_pShareGateSvr != nullptr) {
			DEV_INFO(TF("[游戏DB服务器]同进程网关服务器[%p]更新"), m_pShareGateSvr);
			assert(m_pShareGateSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pUpdate, (DataRef)llParam, m_GateSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]同进程网关服务器对象不存在"));
		}
	}
	return false;
}
//--------------------------------------
bool CGameDBServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	if (PktPtr->GetEvent() != PAK_EVENT_LIVE) {
		switch (PktPtr->GetEvent()) {
		case PAK_EVENT_LINK:
			{
				bRet = OnServerLink(static_cast<CPAKLink*>(PktPtr.Get()), pTcp->krSocket);
			}
			break;
		case PAK_EVENT_UPDATE:
			{
				bRet = OnServerUpdate(static_cast<CPAKUpdate*>(PktPtr.Get()), pTcp->krSocket);
			}
			break;
		case PAK_EVENT_UNLINK:
			{
				bRet = OnServerUnlink(PktPtr.Get(), pTcp->krSocket);
			}
			break;
		case PAK_EVENT_GATE_SELECT:
		case PAK_EVENT_GATE_PLAY:
		case PAK_EVENT_GATE_CREATE:
		case PAK_EVENT_GATE_DELETE:
		case PAK_EVENT_GATE_RECOVER:
			{
				m_GameDBRoutine->Add((Int)PktPtr->GetEvent(), *PktPtr, (LLong)pTcp->krSocket);
			}
			break;
		default:
			{
				LOGV_WARN(m_FileLog, TF("[游戏DB服务器]%p连接信息无法识别的%X服务器信令包数据[event=%d]"), pTcp->krSocket, PktPtr->GetType(), PktPtr->GetEvent());
			}
		}
	}
	else {
		CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_GAMEDB> LiveAck;
		LiveAck.AdjustSize();
		bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
	}
	return bRet;
}
// 服务器处理
bool CGameDBServer::OnServerLink(CPAKLink* pLink, KeyRef krSocket)
{
	bool bRet = false;
	switch (pLink->GetType()) {
	case PAK_TYPE_GAME:
		{
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]游戏服务器[%p]连接"), krSocket);
			bRet = ServerLink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pLink, (DataRef)krSocket, m_GameSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]网关服务器[%p]连接"), krSocket);
			bRet = ServerLink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pLink, (DataRef)krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]%p连接信息无法识别的%X服务器信令包数据"), krSocket, pLink->GetType());
		}
	}
	if (bRet) {
		// 3.设置连接为选择服务器属性
		m_NetworkPtr->SetAttr(krSocket, pLink->GetType());
		// 4.发送连接回执包
		CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_GAMEDB> LinkAck;
		LinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, LinkAck);
	}
	return bRet;
}

bool CGameDBServer::OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUpdate->GetType()) {
	case PAK_TYPE_GAME:
		{
			DEV_INFO(TF("[游戏DB服务器]游戏服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pUpdate, (DataRef)krSocket, m_GameSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			DEV_INFO(TF("[游戏DB服务器]网关服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(pUpdate, (DataRef)krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]%p更新信息无法识别的%X服务器信令包数据"), krSocket, pUpdate->GetType());
		}
	}
	if (bRet) {
		// 发送更新回执包
		CPAKSimple<PAK_EVENT_UPDATEACK, PAK_TYPE_GAMEDB> UpdateAck;
		UpdateAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UpdateAck);
	}
	return bRet;
}

bool CGameDBServer::OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUnlink->GetType()) {
	case PAK_TYPE_GAME:
		{
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]游戏服务器[%p]断接"), krSocket);
			bRet = ServerUnlink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(krSocket, m_GameSvrMap);
		}
		break;
	case PAK_TYPE_GATE:
		{
			LOGV_INFO(m_FileLog, TF("[游戏DB服务器]网关服务器[%p]断接"), krSocket);
			bRet = ServerUnlink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(krSocket, m_GateSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[游戏DB服务器]%p断连信息无法识别的%X服务器信令包数据"), krSocket, pUnlink->GetType());
		}
	}
	if (bRet) {
		// 3.设置连接为无效服务器属性
		m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_NONE);
		// 4.发送断连回执包
		CPAKSimple<PAK_EVENT_UNLINKACK, PAK_TYPE_GAMEDB> UnlinkAck;
		UnlinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UnlinkAck);
	}
	return bRet;
}
//--------------------------------------
bool CGameDBServer::OnTcpAccept(KeyRef krAccept, KeyRef krListen)
{
	if (m_nStatus == STATUSC_RUN) {
		UShort     usPort = 0;
		CStringKey strAddr;
		m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);

		if (krListen == m_krListenGame) {
			DEV_INFO(TF("[游戏DB服务器]游戏服务器[%s]:%d连接完成!"), *strAddr, usPort);
		}
		else if (krListen == m_krListenGate) {
			DEV_INFO(TF("[游戏DB服务器]网关服务器[%s]:%d连接完成!"), *strAddr, usPort);
		}
		m_ServerInfo[INFOI_GAMEDB].Incr();
		return true;
	}
	else {
		LOGV_WARN(m_FileLog, TF("[游戏DB服务器]游戏DB服务器处于暂停状态!"));
	}
	return false;
}
//--------------------------------------
bool CGameDBServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krListenGame) {
		m_krListenGame = nullptr;
		DEV_INFO(TF("[游戏DB服务器]监听游戏服务器连接关闭"));
	}
	else if (krSocket == m_krListenGate) {
		m_krListenGate = nullptr;
		DEV_INFO(TF("[游戏DB服务器]监听网关服务器连接关闭"));
	}
	else { // 单个服务器断连/断开
		switch (llLiveData) {
		case PAK_TYPE_GAME:
			{
				ServerUnlink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(krSocket, m_GameSvrMap);
			}
			break;
		case PAK_TYPE_GATE:
			{
				ServerUnlink<SVR_GATE_MAP, DATA_INDEX_GATE, INFOI_GATE>(krSocket, m_GateSvrMap);
			}
			break;
		default:
			{
			}
		}
		m_ServerInfo[INFOI_GAMEDB].Decr();
	}
	return true;
}
//--------------------------------------
// 定时检测监听游戏服务器连接的连接对象是否有效
bool CGameDBServer::CheckListenGameServer(void)
{
	if (m_krListenGame == nullptr) {
		return StartListenGameServer();
	}
	return true;
}
// 定时检测监听网关服务器连接的连接对象是否有效
bool CGameDBServer::CheckListenGateServer(void)
{
	if (m_krListenGate == nullptr) {
		return StartListenGateServer();
	}
	return true;
}
// 同步服务器信息给界面
bool CGameDBServer::SyncServerInfo(void)
{
	if (m_ServerInfo[INFOI_GAMEDB].usStatus == STATUSU_SYNC) {
		m_GameDBRoutine->Update();
		m_ServerInfo[INFOI_GAMEDB].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_GAMEDB), DATA_INDEX_GAMEDB);
	}
	if (m_ServerInfo[INFOI_GAME].usStatus == STATUSU_SYNC) {
		m_ServerInfo[INFOI_GAME].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_GAME), DATA_INDEX_GAME);
	}
	if (m_ServerInfo[INFOI_GATE].usStatus == STATUSU_SYNC) {
		m_ServerInfo[INFOI_GATE].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_GATE), DATA_INDEX_GATE);
	}
	return true;
}


