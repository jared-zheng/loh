//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : CenterServerImp.cpp                          //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 中心服务器实现                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CenterServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCenterServer
UInt CCenterServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER( pszCMD );
	UNREFERENCED_PARAMETER( utParam );
	return (UInt)RET_OKAY;
}

UInt CCenterServer::Update(void)
{
	CheckListenServers();
	SyncServersInfo();
	return 0;
}
//--------------------------------------
UInt CCenterServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ( (InitLoadShare()  == false) ||
			 (InitLoadConfig() == false) ) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}

bool CCenterServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_CONFIG_PTR, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_CENTER);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[中心服务器]从界面回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[中心服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件

	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[中心服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[中心服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	// 保存中心服务器指针到动态变量表
	LOGV_INFO(m_FileLog, TF("[中心服务器]保存中心服务器指针到动态变量表完成[key=%s]!"), CServerConfig::CenterServer);
	m_pConfig->GetKeyValue()->AddItem(CServerConfig::CenterServer, this);
	return true;
}

bool CCenterServer::InitLoadConfig(void)
{
	assert(m_krListenSelect == nullptr);
	assert(m_krListenLogin  == nullptr);
	assert(m_krListenGame   == nullptr);

	assert(m_pShareSelectSvr == nullptr);
	assert(m_pShareLoginSvr  == nullptr);
	assert(m_pShareGameSvr   == nullptr);

	assert(m_SelectSvrMap.Size() == 0);
	assert(m_LoginSvrMap.Size()  == 0);
	assert(m_GameSvrMap.Size()   == 0);

	m_ServerInfo[INFOI_CENTER].usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_CENTER);
	m_ServerInfo[INFOI_CENTER].usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_CENTER);
	LOGV_INFO(m_FileLog, TF("[中心服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo[INFOI_CENTER].usId, m_ServerInfo[INFOI_CENTER].usIncr);
	return true;
}
//--------------------------------------
void CCenterServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务退出处理完成!"));
	}
}

void CCenterServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CCenterServer::ExitUnloadConfig(void)
{
	assert(m_krListenSelect == nullptr);
	assert(m_krListenLogin  == nullptr);
	assert(m_krListenGame   == nullptr);

	assert(m_pShareSelectSvr == nullptr);
	assert(m_pShareLoginSvr  == nullptr);
	assert(m_pShareGameSvr   == nullptr);

	assert(m_SelectSvrMap.Size() == 0);
	assert(m_LoginSvrMap.Size()  == 0);
	assert(m_GameSvrMap.Size()   == 0);
	LOGV_INFO(m_FileLog, TF("[中心服务器]从动态变量表清除中心服务器指针完成[key=%s]!"), CServerConfig::CenterServer);
	m_pConfig->GetKeyValue()->Remove(CServerConfig::CenterServer);
}
//--------------------------------------
bool CCenterServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务启动开始!"));

		if (StartListenServers() == false) {
			return false;
		}

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务启动完成!"));
	}
	return true;
}

bool CCenterServer::StartListenServers(void)
{
	UShort     usPort = 0;
	CStringKey strAddr;
	// select
	if ((m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_SELECT, strAddr, usPort) == false) || 
		(StartListenServer(m_krListenSelect, strAddr, usPort) == false)) {
		return false;
	}
	// login
	if (m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, usPort) == false) {
		return false;
	}
	if (usPort == 0) {
		m_krListenLogin = (KeyRef)this;
		LOG_INFO(m_FileLog, TF("[中心服务器]监听登陆服务器地址和监听选择服务器地址一样"));
	}
	else if (StartListenServer(m_krListenLogin, strAddr, usPort) == false) {
		return false;
	}
	// game
	if (m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_GAME, strAddr, usPort) == false) {
		return false;
	}
	if (usPort == 0) {
		m_krListenGame = (KeyRef)this;
		LOG_INFO(m_FileLog, TF("[中心服务器]监听游戏服务器地址和监听选择服务器或者登陆服务器地址一样"));
	}
	else if (StartListenServer(m_krListenGame, strAddr, usPort) == false) {
		return false;
	}
	return true;
}

bool CCenterServer::StartListenServer(KeyRef& krListen, const CStringKey& strAddr, UShort usPort)
{
	if (krListen != nullptr) {
		LOGV_INFO(m_FileLog, TF("[中心服务器]创建监听服务器对象[%s]:%d已经存在"), *strAddr, usPort);
		return true;
	}
	bool bRet = true;
	krListen = m_NetworkPtr->Create(*this, usPort, *strAddr);
	if (krListen != nullptr) {
		bRet = m_NetworkPtr->Listen(krListen);
		LOGV_INFO(m_FileLog, TF("[中心服务器]创建监听服务器对象[%s]:%d成功, %s"), *strAddr, usPort, bRet ? TF("监听操作成功") : TF("监听操作失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[中心服务器]创建监听服务器对象[%s]:%d失败"), *strAddr, usPort);
		bRet = false;
	}
	return bRet;
}
//--------------------------------------
bool CCenterServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE;
		LOG_INFO(m_FileLog, TF("[中心服务器]暂停服务器的连接"));
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[中心服务器]允许服务器的连接"));
		return true;
	}
	return false;
}
//--------------------------------------
void CCenterServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务停止开始!"));

		StopListenServers();

		m_pShareSelectSvr = nullptr;
		m_pShareLoginSvr  = nullptr;
		m_pShareGameSvr   = nullptr;

		m_ServerInfo[INFOI_CENTER].Zero();
		m_ServerInfo[INFOI_SELECT].Reset();
		m_ServerInfo[INFOI_LOGIN].Reset();
		m_ServerInfo[INFOI_GAME].Reset();
		m_SelectSvrMap.RemoveAll();
		m_LoginSvrMap.RemoveAll();
		m_GameSvrMap.RemoveAll();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[中心服务器]中心服务停止完成!"));
	}
}

void CCenterServer::StopListenServers(void)
{
	if (m_krListenSelect != nullptr) {
		m_NetworkPtr->Destroy(m_krListenSelect, false);
		m_krListenSelect = nullptr;
		LOG_INFO(m_FileLog, TF("[中心服务器]销毁监听选择服务器对象成功"));
	}
	if (m_krListenLogin != nullptr) {
		if (m_krListenLogin != (KeyRef)this) {
			m_NetworkPtr->Destroy(m_krListenLogin, false);
		}
		m_krListenLogin = nullptr;
		LOG_INFO(m_FileLog, TF("[中心服务器]销毁监听登陆服务器对象成功"));
	}
	if (m_krListenGame != nullptr) {
		if (m_krListenGame != (KeyRef)this) {
			m_NetworkPtr->Destroy(m_krListenGame, false);
		}
		m_krListenGame = nullptr;
		LOG_INFO(m_FileLog, TF("[中心服务器]销毁监听游戏服务器对象成功"));
	}
}
//--------------------------------------
bool CCenterServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE)
{
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
	//case PAK_EVENT_UNLINK: // 同进程服务器注销, 说明服务停止, 不需要处理
	//	break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return false;
}

bool CCenterServer::OnShareLink(CEventBase& EventRef, LLong llParam)
{
	CPAKLink* pLink = static_cast<CPAKLink*>(&EventRef);
	switch (pLink->GetType()) {
	case PAK_TYPE_GAME:
		if (m_pShareGameSvr == nullptr) {
			// 0.获得共享指针
			m_pShareGameSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[中心服务器]同进程游戏服务器[%p]注册"), m_pShareGameSvr);
			return ServerLink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pLink, (DataRef)llParam, m_GameSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程游戏服务器对象已经存在[%p-%p]"), m_pShareGameSvr, llParam);
		}
		break;
	case PAK_TYPE_LOGIN:
		if (m_pShareLoginSvr == nullptr) {
			// 0.获得共享指针
			m_pShareLoginSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[中心服务器]同进程登陆服务器[%p]注册"), m_pShareLoginSvr);
			if (ServerLink<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(pLink, (DataRef)llParam, m_LoginSvrMap)) {
				// 3.刚注册的登陆服务器发送okay和sync状态的游戏服务器信息
				return SendGameServerInfo();
			}		
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程登陆服务器对象已经存在[%p-%p]"), m_pShareLoginSvr, llParam);
		}
		break;
	case PAK_TYPE_SELECT:
		if (m_pShareSelectSvr == nullptr) {
			// 0.获得共享指针
			m_pShareSelectSvr = reinterpret_cast<ICommonServer*>(llParam);
			LOGV_INFO(m_FileLog, TF("[中心服务器]同进程选择服务器[%p]注册"), m_pShareSelectSvr);
			if (ServerLink<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(pLink, (DataRef)llParam, m_SelectSvrMap)) {
				// 3.刚注册的选择服务器发送okay和sync状态的登陆服务器信息
				return SendLoginServerInfo();
			}
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程选择服务器对象已经存在[%p-%p]"), m_pShareSelectSvr, llParam);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程服务器类型未知%X"), pLink->GetType());
		}
	}
	return false;
}

bool CCenterServer::OnShareUpdate(CEventBase& EventRef, LLong llParam)
{
	CPAKUpdate* pUpdate = static_cast<CPAKUpdate*>(&EventRef);
	switch (pUpdate->GetType()) {
	case PAK_TYPE_GAME:
		if (m_pShareGameSvr != nullptr) {
			DEV_INFO(TF("[中心服务器]同进程游戏服务器[%p]更新"), m_pShareGameSvr);
			assert(m_pShareGameSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pUpdate, (DataRef)llParam, m_GameSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程游戏服务器对象未注册"));
		}
		break;
	case PAK_TYPE_LOGIN:
		if (m_pShareLoginSvr != nullptr) {
			DEV_INFO(TF("[中心服务器]同进程登陆服务器[%p]更新"), m_pShareLoginSvr);
			assert(m_pShareLoginSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(pUpdate, (DataRef)llParam, m_LoginSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程登陆服务器对象未注册"));
		}
		break;
	case PAK_TYPE_SELECT:
		if (m_pShareSelectSvr != nullptr) {
			DEV_INFO(TF("[中心服务器]同进程选择服务器[%p]更新"), m_pShareSelectSvr);
			assert(m_pShareSelectSvr == reinterpret_cast<ICommonServer*>(llParam));
			return ServerUpdate<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(pUpdate, (DataRef)llParam, m_SelectSvrMap);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程选择服务器对象未注册"));
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]同进程服务器更新类型未知%X"), pUpdate->GetType());
		}
	}
	return false;
}
//--------------------------------------
bool CCenterServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
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
	case PAK_EVENT_SYNCACK:
		{
			bRet = true;
			DEV_INFO(TF("[中心服务器]向%X服务器[%p]同步服务器信息完成"), PktPtr->GetType(), pTcp->krSocket);
		}
		break;
	case PAK_EVENT_LIVE:
		{
			CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_CENTER> LiveAck;
			LiveAck.AdjustSize();
			bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]%p无法识别的%X服务器信令包数据[event=%d]"), pTcp->krSocket, PktPtr->GetType(), PktPtr->GetEvent());
		}
	}
	return bRet;
}

bool CCenterServer::OnServerLink(CPAKLink* pLink, KeyRef krSocket)
{
	switch (pLink->GetType()) {
	case PAK_TYPE_GAME:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]游戏服务器[%p]注册"), krSocket);
			if (ServerLink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pLink, (DataRef)krSocket, m_GameSvrMap)) {
				// 3.设置为游戏服务器类型
				m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_GAME);
				// 4.发送注册回执包
				CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_CENTER> LinkAck;
				LinkAck.AdjustSize();
				return m_NetworkPtr->Send(krSocket, LinkAck);
			}
		}
		break;
	case PAK_TYPE_LOGIN:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]登陆服务器[%p]注册"), krSocket);
			if (ServerLink<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(pLink, (DataRef)krSocket, m_LoginSvrMap)) {
				// 3.设置为登陆服务器类型
				m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_LOGIN_CENTER);
				// 4.发送注册回执包
				CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_CENTER> LinkAck;
				LinkAck.AdjustSize();
				m_NetworkPtr->Send(krSocket, LinkAck);
				// 5.刚注册的登陆服务器发送okay和sync状态的游戏服务器信息
				return SendGameServerInfo(krSocket);
			}
		}
		break;
	case PAK_TYPE_SELECT:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]选择服务器[%p]注册"), krSocket);
			if (ServerLink<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(pLink, (DataRef)krSocket, m_SelectSvrMap)) {
				// 3.设置为选择服务器类型
				m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_SELECT);
				// 4.发送注册回执包
				CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_CENTER> LinkAck;
				LinkAck.AdjustSize();
				m_NetworkPtr->Send(krSocket, LinkAck);
				// 5.刚注册的选择服务器发送okay和sync状态的登陆服务器信息
				return SendLoginServerInfo(krSocket);
			}
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]%p注册信息无法识别的%X服务器信令包数据"), krSocket, pLink->GetType());
		}
	}
	return false;
}

bool CCenterServer::OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUpdate->GetType()) {
	case PAK_TYPE_GAME:
		{
			DEV_INFO(TF("[中心服务器]游戏服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(pUpdate, (DataRef)krSocket, m_GameSvrMap);
		}
		break;
	case PAK_TYPE_LOGIN:
		{
			DEV_INFO(TF("[中心服务器]登陆服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(pUpdate, (DataRef)krSocket, m_LoginSvrMap);
		}
		break;
	case PAK_TYPE_SELECT:
		{
			DEV_INFO(TF("[中心服务器]选择服务器[%p]更新"), krSocket);
			bRet = ServerUpdate<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(pUpdate, (DataRef)krSocket, m_SelectSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]%p更新信息无法识别的%X服务器信令包数据"), krSocket, pUpdate->GetType());
		}
	}
	if (bRet) {
		// 发送更新回执包
		CPAKSimple<PAK_EVENT_UPDATEACK, PAK_TYPE_CENTER> UpdateAck;
		UpdateAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UpdateAck);
	}
	return bRet;
}

bool CCenterServer::OnServerUnlink(CPAKHead* pUnlink, KeyRef krSocket)
{
	bool bRet = false;
	switch (pUnlink->GetType()) {
	case PAK_TYPE_GAME:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]游戏服务器[%p]注销"), krSocket);
			bRet = ServerUnlink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(krSocket, m_GameSvrMap);
		}
		break;
	case PAK_TYPE_LOGIN:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]登陆服务器[%p]注销"), krSocket);
			bRet = ServerUnlink<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(krSocket, m_LoginSvrMap);
		}
		break;
	case PAK_TYPE_SELECT:
		{
			LOGV_INFO(m_FileLog, TF("[中心服务器]选择服务器[%p]注销"), krSocket);
			bRet = ServerUnlink<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(krSocket, m_SelectSvrMap);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[中心服务器]%p注销信息无法识别的%X服务器信令包数据"), krSocket, pUnlink->GetType());
		}
	}
	if (bRet) {
		// 3.设置为无效服务器类型
		m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_NONE);
		// 4.发送注销回执包
		CPAKSimple<PAK_EVENT_UNLINKACK, PAK_TYPE_CENTER> UnlinkAck;
		UnlinkAck.AdjustSize();
		m_NetworkPtr->Send(krSocket, UnlinkAck);
	}
	return bRet;
}
//--------------------------------------
bool CCenterServer::OnTcpAccept(KeyRef krAccept, KeyRef)
{
	if (m_nStatus == STATUSC_RUN) {
		UShort     usPort = 0;
		CStringKey strAddr;
		m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);
		DEV_INFO(TF("[中心服务器]服务器[%s]:%d连接完成!"), *strAddr, usPort);

		m_ServerInfo[INFOI_CENTER].Incr();
		return true;
	}
	else {
		LOGV_WARN(m_FileLog, TF("[中心服务器]中心服务器处于暂停状态!"));
	}
	return false;
}
//--------------------------------------
bool CCenterServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krListenGame) {
		m_krListenGame = nullptr;
		DEV_INFO(TF("[中心服务器]监听游戏服务器对象关闭"));
		LOG_INFO(m_FileLog, TF("[中心服务器]监听游戏服务器对象关闭"));
	}
	else if (krSocket == m_krListenLogin) {
		m_krListenLogin = nullptr;
		DEV_INFO(TF("[中心服务器]监听登陆服务器对象关闭"));
		LOG_INFO(m_FileLog, TF("[中心服务器]监听登陆服务器对象关闭"));
	}
	else if (krSocket == m_krListenSelect) {
		m_krListenSelect = nullptr;
		DEV_INFO(TF("[中心服务器]监听选择服务器对象关闭"));
		LOG_INFO(m_FileLog, TF("[中心服务器]监听选择服务器对象关闭"));
	}
	else { // 单个服务器断开
		switch (llLiveData) {
		case PAK_TYPE_GAME:
			{
				ServerUnlink<SVR_GAME_MAP, DATA_INDEX_GAME, INFOI_GAME>(krSocket, m_GameSvrMap);
				LOGV_INFO(m_FileLog, TF("[中心服务器]游戏服务器[%p]断开"), krSocket);
			}
			break;
		case PAK_TYPE_LOGIN_CENTER:
			{
				ServerUnlink<SVR_LOGIN_MAP, DATA_INDEX_LOGIN, INFOI_LOGIN>(krSocket, m_LoginSvrMap);
				LOGV_INFO(m_FileLog, TF("[中心服务器]登陆服务器[%p]断开"), krSocket);
			}
			break;
		case PAK_TYPE_SELECT:
			{
				ServerUnlink<SVR_SELECT_MAP, DATA_INDEX_SELECT, INFOI_SELECT>(krSocket, m_SelectSvrMap);
				LOGV_INFO(m_FileLog, TF("[中心服务器]选择服务器[%p]断开"), krSocket);
			}
			break;
		default:
			{
			}
		}
		m_ServerInfo[INFOI_CENTER].Decr();
	}
	return true;
}
//--------------------------------------
bool CCenterServer::CheckListenServers(void)
{
	return (CheckListenSelectServer() && CheckListenLoginServer() && CheckListenGameServer());
}

bool CCenterServer::CheckListenSelectServer(void)
{
	if (m_krListenSelect == nullptr){
		UShort     usPort = 0;
		CStringKey strAddr;
		if ((m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_SELECT, strAddr, usPort) == false) ||
			(StartListenServer(m_krListenSelect, strAddr, usPort) == false)){
			return false;
		}
	}
	return true;
}

bool CCenterServer::CheckListenLoginServer(void)
{
	if (m_krListenLogin == nullptr){
		UShort     usPort = 0;
		CStringKey strAddr;
		if ((m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_LOGIN, strAddr, usPort) == false) ||
			(StartListenServer(m_krListenLogin, strAddr, usPort) == false)){
			return false;
		}
	}
	return true;
}

bool CCenterServer::CheckListenGameServer(void)
{
	if (m_krListenGame == nullptr){
		UShort     usPort = 0;
		CStringKey strAddr;
		if ((m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_GAME, strAddr, usPort) == false) ||
		    (StartListenServer(m_krListenGame, strAddr, usPort) == false)){
			return false;
		}
	}
	return true;
}

bool CCenterServer::SyncServersInfo(void)
{
	SyncCenterServerInfo();
	SyncSelectServerInfo();
	SyncLoginServerInfo();
	SyncGameServerInfo();
	return true;
}

bool CCenterServer::SyncCenterServerInfo(void)
{
	if (m_ServerInfo[INFOI_CENTER].usStatus == STATUSU_SYNC) {
		m_ServerInfo[INFOI_CENTER].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_CENTER), DATA_INDEX_CENTER);
	}
	return true;
}

bool CCenterServer::SyncSelectServerInfo(void)
{
	if (m_ServerInfo[INFOI_SELECT].usStatus == STATUSU_SYNC) {
		m_ServerInfo[INFOI_SELECT].usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_SELECT), DATA_INDEX_SELECT);
	}
	return true;
}

bool CCenterServer::SyncLoginServerInfo(void)
{
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	if (m_ServerInfo[INFOI_LOGIN].usStatus == STATUSU_SYNC) {
		DEV_INFO(TF("[中心服务器]同步登陆服务器信息给所有选择服务器"));
		CStreamScopePtr StreamPtr;
		if (m_NetworkPtr->AllocJumboBuffer(StreamPtr)) {
			CCenterSync Sync;
			Sync.SetServerData(m_ServerInfo[INFOI_LOGIN]);
			Sync.Serialize(*StreamPtr);

			bool bRet = false;
			{ CSyncLockWaitScope scope(m_LoginSvrMap.GetLock()); bRet = m_LoginSvrMap.Serialize(*StreamPtr); }
			if (bRet) {
				if (m_pShareSelectSvr != nullptr) {
					m_pShareSelectSvr->OnShareRoutine(PAK_EVENT_SYNC, *StreamPtr);
				}
				if (m_NetworkPtr->Send(nullptr, *StreamPtr, SEND_BROADCAST_AS, PAK_TYPE_SELECT)) { // 广播到所有选择服务器
					CSyncLockWaitScope scope(m_LoginSvrMap.GetLock());
					m_LoginSvrMap.Update();
					m_ServerInfo[INFOI_LOGIN].usStatus = STATUSU_OKAY;
				}
			}
			return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_LOGIN), DATA_INDEX_LOGIN);
		}
		return false;
	}
	else {
		DEV_INFO(TF("[中心服务器]同步登陆服务器信息给所有选择服务器发现没有同步数据"));
	}
	return true;
}

bool CCenterServer::SyncGameServerInfo(void)
{
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	if (m_ServerInfo[INFOI_GAME].usStatus == STATUSU_SYNC) {
		DEV_INFO(TF("[中心服务器]同步游戏服务器信息给登陆服务器"));
		CStreamScopePtr StreamPtr;
		if (m_NetworkPtr->AllocJumboBuffer(StreamPtr)) {
			CCenterSync Sync;
			Sync.SetServerData(m_ServerInfo[INFOI_GAME]);
			Sync.Serialize(*StreamPtr);

			bool bRet = false;
			{ CSyncLockWaitScope scope(m_GameSvrMap.GetLock()); bRet = m_GameSvrMap.Serialize(*StreamPtr); }
			if (bRet) {
				if (m_pShareLoginSvr != nullptr) {
					m_pShareLoginSvr->OnShareRoutine(PAK_EVENT_SYNC, *StreamPtr);
				}
				if (m_NetworkPtr->Send(nullptr, *StreamPtr, SEND_BROADCAST_AS, PAK_TYPE_LOGIN_CENTER)) { // 广播到所有登陆服务器
					CSyncLockWaitScope scope(m_GameSvrMap.GetLock());
					m_GameSvrMap.Update();
					m_ServerInfo[INFOI_GAME].usStatus = STATUSU_OKAY;
				}
			}
			return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(m_ServerInfo + INFOI_GAME), DATA_INDEX_GAME);
		}
		return false;
	}
	else {
		DEV_INFO(TF("[中心服务器]同步游戏服务器信息给登陆服务器发现没有同步数据"));
	}
	return true;
}

bool CCenterServer::SendLoginServerInfo(KeyRef krSocket)
{
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	CStreamScopePtr StreamPtr;
	if (m_NetworkPtr->AllocJumboBuffer(StreamPtr)) {
		CCenterSync Sync;
		Sync.SetServerData(m_ServerInfo[INFOI_LOGIN]);
		Sync.Serialize(*StreamPtr);

		bool bRet = false;
		{ CSyncLockWaitScope scope(m_LoginSvrMap.GetLock()); bRet = m_LoginSvrMap.Serialize(*StreamPtr, STATUSU_OKAYSYNC); }
		if (bRet){
			if (krSocket != nullptr) {
				m_NetworkPtr->Send(krSocket, *StreamPtr);
			}
			else {
				assert(m_pShareSelectSvr != nullptr);
				m_pShareSelectSvr->OnShareRoutine(PAK_EVENT_SYNC, *StreamPtr);
			}
		}
		return true;
	}
	return false;
}

bool CCenterServer::SendGameServerInfo(KeyRef krSocket)
{
	// 3. TODO!!!这么没有考虑数据超出流对象保存上限的问题, 这里只是使用jumbo buffer发送, 
	//    后期如果不满足分多个buffer/jumbo buffer发送, 默认jumbo buffer可以最多发送同类型120个服务器的全部信息,
	//    目前同步发送只有在中心服务器同步服务器信息给选择和登陆时存在, 还有登陆服务器同步游戏服务器信息给客户端
	CStreamScopePtr StreamPtr;
	if (m_NetworkPtr->AllocJumboBuffer(StreamPtr)) {
		CCenterSync Sync;
		Sync.SetServerData(m_ServerInfo[INFOI_GAME]);
		Sync.Serialize(*StreamPtr);

		bool bRet = false;
		{ CSyncLockWaitScope scope(m_GameSvrMap.GetLock()); bRet = m_GameSvrMap.Serialize(*StreamPtr, STATUSU_OKAYSYNC); }
		if (bRet){
			if (krSocket != nullptr) {
				m_NetworkPtr->Send(krSocket, *StreamPtr);
			}
			else {
				assert(m_pShareLoginSvr != nullptr);
				m_pShareLoginSvr->OnShareRoutine(PAK_EVENT_SYNC, *StreamPtr);
			}
		}
		return true;
	}
	return false;
}




