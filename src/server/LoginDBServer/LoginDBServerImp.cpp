//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : LoginDBServerImp.cpp                         //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 登陆DB服务器实现                                   //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoginDBServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLoginDBServer
UInt CLoginDBServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CLoginDBServer::Update(void)
{
	CheckListenLoginServer();
	SyncServerInfo();
	return 0;
}
//--------------------------------------
UInt CLoginDBServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}

bool CLoginDBServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_CONFIG_PTR, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_LOGINDB);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[登陆DB服务器]从同进程共享数据回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[登陆DB服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件

	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[登陆DB服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[登陆DB服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	// 保存登陆DB服务器指针到动态变量表
	LOGV_INFO(m_FileLog, TF("[登陆DB服务器]保存登陆DB服务器指针到动态变量表完成[key=%s]!"), CServerConfig::LoginDBServer);
	m_pConfig->GetKeyValue()->AddItem(CServerConfig::LoginDBServer, this);
	return true;
}

bool CLoginDBServer::InitLoadConfig(void)
{
	assert(m_krListenLogin == nullptr);
	assert(m_pShareLoginSvr == nullptr);

	assert(m_LoginSvrMap.Size() == 0);
	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_LOGINDB);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_LOGINDB);
	LOGV_INFO(m_FileLog, TF("[登陆DB服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);

	assert(m_LoginDBRoutine == nullptr);
	m_LoginDBRoutine = (ICommonRoutine*)CRTTI::CreateByName(TF("CLoginDBRoutine"));
	if (m_LoginDBRoutine != nullptr) {
		return (m_LoginDBRoutine->Init() == (UInt)RET_OKAY);
	}
	LOG_ERROR(m_FileLog, TF("[登陆DB服务器]创建事务处理对象失败"));
	return false;
}
//--------------------------------------
void CLoginDBServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务退出处理完成!"));
	}
}

void CLoginDBServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}

void CLoginDBServer::ExitUnloadConfig(void)
{
	assert(m_krListenLogin == nullptr);
	assert(m_pShareLoginSvr == nullptr);

	assert(m_LoginSvrMap.Size() == 0);
	LOGV_INFO(m_FileLog, TF("[登陆DB服务器]从动态变量表清除登陆DB服务器指针完成[key=%s]!"), CServerConfig::LoginDBServer);
	m_pConfig->GetKeyValue()->Remove(CServerConfig::LoginDBServer);

	m_LoginDBRoutine->Exit();
	m_LoginDBRoutine = nullptr;
}
//--------------------------------------
bool CLoginDBServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务启动开始!"));

		if (StartListenLoginServer() == false){
			return false;
		}
		m_LoginDBRoutine->Start(this);

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务启动完成!"));
	}
	return true;
}

bool CLoginDBServer::StartListenLoginServer(void)
{
	UShort     usPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_LOGINDB, 0, strAddr, usPort);

	bool bRet = true;
	assert(m_krListenLogin == nullptr);
	m_krListenLogin = m_NetworkPtr->Create(*this, usPort, *strAddr);
	if (m_krListenLogin != nullptr) {
		bRet = m_NetworkPtr->Listen(m_krListenLogin);
		LOGV_INFO(m_FileLog, TF("[登陆DB服务器]创建监听登陆服务器对象[%s]:%d成功, %s"), *strAddr, usPort, bRet ? TF("监听操作成功") : TF("监听操作失败"));
	}
	else {
		LOGV_ERROR(m_FileLog, TF("[登陆DB服务器]创建监听登陆服务器对象[%s]:%d失败"), *strAddr, usPort);
		bRet = false;
	}
	return bRet;
}
//--------------------------------------
bool CLoginDBServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]暂停登陆服务器的连接"));
		m_LoginDBRoutine->Pause();
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]允许登陆服务器的连接"));
		m_LoginDBRoutine->Pause(false);
		return true;
	}
	return false;
}
//--------------------------------------
void CLoginDBServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务停止开始!"));

		m_LoginDBRoutine->Stop();

		StopListenLoginServer();

		m_ServerInfo.Zero();
		m_LoginSvrMap.RemoveAll();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]登陆DB服务停止完成!"));
	}
}

void CLoginDBServer::StopListenLoginServer(void)
{
	if (m_krListenLogin != nullptr) {
		m_NetworkPtr->Destroy(m_krListenLogin, false);
		m_krListenLogin = nullptr;
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]销毁监听登陆服务器对象成功"));
	}
	m_pShareLoginSvr = nullptr;
}
//--------------------------------------
bool CLoginDBServer::OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	assert((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_LOGIN) != 0);
	switch (nEvent) {
	case PAK_EVENT_LINK:
		{
			OnShareLink(EventRef, llParam);
		}
		break;
	case PAK_EVENT_UPDATE:
		{
			OnShareUpdate(EventRef, llParam);
		}
		break;
	//case PAK_EVENT_UNLINK: // 同进程服务器注销, 说明服务停止, 不需要处理
	//	break;
	case PAK_EVENT_LOGIN_LINK:
	case PAK_EVENT_LOGIN_UNLINK:
		{
			m_LoginDBRoutine->Add(nEvent, EventRef, llParam, eType);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆DB服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
			assert(0);
		}
	}
	return true;
}

bool CLoginDBServer::OnShareLink(CEventBase& EventRef, LLong llParam)
{
	CPAKLink* pLink = static_cast<CPAKLink*>(&EventRef);
	assert(pLink->GetType() == PAK_TYPE_LOGIN);
	if (m_pShareLoginSvr == nullptr) {
		// 0.获得共享指针
		m_pShareLoginSvr = reinterpret_cast<ICommonServer*>(llParam);
		LOGV_INFO(m_FileLog, TF("[登陆DB服务器]同进程登陆服务器[%p]注册"), m_pShareLoginSvr);
		return ServerLink(pLink, (DataRef)llParam);
	}
	LOGV_WARN(m_FileLog, TF("[登陆DB服务器]同进程登陆服务器对象已经存在[%p-%p]"), m_pShareLoginSvr, llParam);
	return false;
}

bool CLoginDBServer::OnShareUpdate(CEventBase& EventRef, LLong llParam)
{
	CPAKUpdate* pUpdate = static_cast<CPAKUpdate*>(&EventRef);
	assert(pUpdate->GetType() == PAK_TYPE_LOGIN);
	if (m_pShareLoginSvr != nullptr) {
		DEV_INFO(TF("[登陆DB服务器]同进程登陆服务器[%p]更新"), m_pShareLoginSvr);
		assert(m_pShareLoginSvr == reinterpret_cast<ICommonServer*>(llParam));
		return ServerUpdate(pUpdate, (DataRef)llParam);
	}
	LOGV_WARN(m_FileLog, TF("[登陆DB服务器]同进程登陆服务器对象未注册"));
	return false;
}
//--------------------------------------
bool CLoginDBServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	assert(PktPtr->GetType() == PAK_TYPE_LOGIN);
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
	case PAK_EVENT_LOGIN_LINK:
	case PAK_EVENT_LOGIN_UNLINK:
		{
			bRet = m_LoginDBRoutine->Add((Int)PktPtr->GetEvent(), *PktPtr, (LLong)pTcp->krSocket);
		}
		break;
	case PAK_EVENT_LIVE:
		{
			CPAKSimple<PAK_EVENT_LIVEACK, PAK_TYPE_LOGINDB> LiveAck;
			LiveAck.AdjustSize();
			bRet = m_NetworkPtr->Send(pTcp->krSocket, LiveAck);
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[登陆DB服务器]%p无法识别的登陆服务器信令包数据[event=%d]"), pTcp->krSocket, PktPtr->GetEvent());
		}
	}
	return bRet;
}

bool CLoginDBServer::OnServerLink(CPAKLink* pLink, KeyRef krSocket)
{
	LOGV_INFO(m_FileLog, TF("[登陆DB服务器]登陆服务器[%p]注册"), krSocket);
	if (ServerLink(pLink, (DataRef)krSocket)) {
		// 3.设置为登陆服务器类型
		m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_LOGIN);
		// 4.发送注册回执包
		CPAKSimple<PAK_EVENT_LINKACK, PAK_TYPE_LOGINDB> LinkAck;
		LinkAck.AdjustSize();
		return m_NetworkPtr->Send(krSocket, LinkAck);
	}
	return false;
}

bool CLoginDBServer::OnServerUpdate(CPAKUpdate* pUpdate, KeyRef krSocket)
{
	DEV_INFO(TF("[登陆DB服务器]登陆服务器[%p]更新"), krSocket);
	if (ServerUpdate(pUpdate, (DataRef)krSocket)) {
		// 3.发送更新回执包
		CPAKSimple<PAK_EVENT_UPDATEACK, PAK_TYPE_LOGINDB> UpdateAck;
		UpdateAck.AdjustSize();
		return m_NetworkPtr->Send(krSocket, UpdateAck);
	}
	return false;
}

bool CLoginDBServer::OnServerUnlink(CPAKHead*, KeyRef krSocket)
{
	LOGV_INFO(m_FileLog, TF("[登陆DB服务器]登陆服务器[%p]注销"), krSocket);
	if (ServerUnlink(krSocket)) {
		// 2.设置为无效服务器类型
		m_NetworkPtr->SetAttr(krSocket, PAK_TYPE_NONE);
		// 3.发送注销回执包
		CPAKSimple<PAK_EVENT_UNLINKACK, PAK_TYPE_LOGINDB> UnlinkAck;
		UnlinkAck.AdjustSize();
		return m_NetworkPtr->Send(krSocket, UnlinkAck, SEND_CLOSE);
	}
	return false;
}
//--------------------------------------
bool CLoginDBServer::OnTcpAccept(KeyRef krAccept, KeyRef krListen)
{
	UNREFERENCED_PARAMETER( krListen );
	assert(krListen == m_krListenLogin);
	if (m_nStatus == STATUSC_RUN) {
		UShort     usPort = 0;
		CStringKey strAddr;
		m_NetworkPtr->GetAddr(krAccept, strAddr, usPort);

		DEV_INFO(TF("[登陆DB服务器]登陆服务器[%s]:%d连接完成!"), *strAddr, usPort);
		m_ServerInfo.Incr();
		return true;
	}
	else {
		LOGV_WARN(m_FileLog, TF("[登陆DB服务器]登陆DB服务器处于暂停状态!"));
	}
	return false;
}
//--------------------------------------
bool CLoginDBServer::OnTcpClose(KeyRef krSocket, LLong llLiveData)
{
	if (krSocket == m_krListenLogin) {
		m_krListenLogin = nullptr;
		DEV_INFO(TF("[登陆DB服务器]监听登陆服务器对象关闭"));
		LOG_INFO(m_FileLog, TF("[登陆DB服务器]监听登陆服务器对象关闭"));
	}
	else { // 单个服务器断开
		if (llLiveData == PAK_TYPE_LOGIN) {
			ServerUnlink(krSocket);
			LOGV_INFO(m_FileLog, TF("[登陆DB服务器]登陆服务器[%p]断开"), krSocket);
		}
		m_ServerInfo.Decr();
	}
	return true;
}
//--------------------------------------
bool CLoginDBServer::CheckListenLoginServer(void)
{
	if (m_krListenLogin == nullptr) {
		return StartListenLoginServer();
	}
	return true;
}

bool CLoginDBServer::SyncServerInfo(void)
{
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		m_LoginDBRoutine->Update();

		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_LOGINDB);
	}
	return true;
}


