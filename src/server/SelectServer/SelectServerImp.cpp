//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : SelectServerImp.cpp                          //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 选择服务器管理实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectServerImp.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSelectServer
UInt CSelectServer::Command(PCXStr pszCMD, uintptr_t utParam)
{
	// 按配置文件中配置项名称更新相关参数
	UNREFERENCED_PARAMETER(pszCMD);
	UNREFERENCED_PARAMETER(utParam);
	return (UInt)RET_OKAY;
}

UInt CSelectServer::Update(void)
{
	CheckConnectCenterServer();
	CheckUDPService();
	SyncServerInfo();
	return 0;
}
//--------------------------------------
UInt CSelectServer::Init(CEventHandler& EventHandlerRef)
{
	if (m_nStatus == STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务初始化开始!"));
		m_pUIHandler = &EventHandlerRef;

		if ((InitLoadShare() == false) ||
			(InitLoadConfig() == false)) {
			return (UInt)RET_FAIL;
		}

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务初始化完成!"));
		return (UInt)RET_OKAY;
	}
	return (UInt)RET_FAIL;
}
// 获取共享的配置对象和网络对象
bool CSelectServer::InitLoadShare(void)
{
	assert(m_pConfig == nullptr);
	m_pUIHandler->OnHandle(CServerConfig::CFG_DEFAULT_CONFIG, reinterpret_cast<uintptr_t>(&m_pConfig), DATA_INDEX_SELECT);
	if (m_pConfig == nullptr) {
		LOG_ERROR(m_FileLog, TF("[选择服务器]从同进程共享数据回调接口获取配置对象指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[选择服务器]从界面回调接口获取配置对象指针完成!"));
	m_pConfig->Dump(m_FileLog); // 配置信息输出到日志文件

	// 从动态变量表获取网络层智能指针
	assert(m_NetworkPtr == nullptr);
	CKeyValue::VALUE xValue = { 0 };
	m_pConfig->GetKeyValue()->GetValue(CServerConfig::NetworkPtr, xValue);
	if (xValue.pValue == nullptr) {
		LOG_ERROR(m_FileLog, TF("[选择服务器]从动态变量表获取网络层智能指针无效"));
		return false;
	}
	LOG_INFO(m_FileLog, TF("[选择服务器]从动态变量表获取网络层智能指针完成!"));
	m_NetworkPtr = *(reinterpret_cast<CNetworkPtr*>(xValue.pValue));
	return true;
}
// 初始化配置
bool CSelectServer::InitLoadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krUDPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);

	assert(m_LoginSvrMap.Size() == 0);
	m_ServerInfo.usId   = m_pConfig->GetServerId(CServerConfig::CFG_DEFAULT_SELECT);
	m_ServerInfo.usIncr = m_pConfig->GetServerIncr(CServerConfig::CFG_DEFAULT_SELECT);
	LOGV_INFO(m_FileLog, TF("[选择服务器]从配置对象读取服务器Id=%d, Incr=%d!"), m_ServerInfo.usId, m_ServerInfo.usIncr);
	if (m_pConfig->GetNetworkAttr() & ATTR_IPV6) {
		m_ServerInfo.NetAddr[SELECTI_UDP].usAttr = ATTR_IPV6;
	}

	UShort     usPort = 0;
	CStringKey strAddr;
	m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_SELECT, 0, strAddr, usPort);
	m_NetworkPtr->TranslateAddr(strAddr, usPort, m_ServerInfo.NetAddr[SELECTI_UDP]); // client udp
	return true;
}
//--------------------------------------
void CSelectServer::Exit(void)
{
	if (m_nStatus != STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务退出处理开始!"));
		m_pUIHandler = nullptr;

		ExitUnloadConfig();
		ExitUnloadShare();

		m_nStatus = STATUSC_NONE;
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务退出处理完成!"));
	}
}
// 清除共享的配置对象和网络对象
void CSelectServer::ExitUnloadShare(void)
{
	m_pConfig    = nullptr;
	m_NetworkPtr = nullptr;
}
//
void CSelectServer::ExitUnloadConfig(void)
{
	assert(m_krConnectCenter == nullptr);
	assert(m_krUDPService == nullptr);

	assert(m_pShareCenterSvr == nullptr);

	assert(m_LoginSvrMap.Size() == 0);
}
//--------------------------------------
bool CSelectServer::Start(void)
{
	if (m_nStatus == STATUSC_INIT) {
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务启动开始!"));

		if ((StartConnectCenterServer() == false) ||
			(StartUDPService() == false)) {
			return false;
		}

		m_nStatus = STATUSC_RUN;
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务启动完成!"));
	}
	return true;
}
// 运行创建连接中心服务器的连接对象
bool CSelectServer::StartConnectCenterServer(void)
{
	// 选择和中心在不同进程,  需要连接内网中心服务器
	if ((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) == 0) {
		if (m_krConnectCenter == nullptr) {
			UShort     usPort = 0;
			CStringKey strAddr;
			m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_SELECT, CServerConfig::CFG_DEFAULT_CENTER, strAddr, usPort);
			m_krConnectCenter = m_NetworkPtr->Create(*this, usPort, *strAddr);
		}
		if (m_krConnectCenter != nullptr) {
			if (m_bCenterCnnted == false) {
				LOG_INFO(m_FileLog, TF("[选择服务器]选择服务器和中心服务器在不同进程, 创建连接中心服务器Socket成功"));
				UShort     usPort = 0;
				CStringKey strAddr;
				m_pConfig->GetServerAddr(CServerConfig::CFG_DEFAULT_CENTER, CServerConfig::CFG_DEFAULT_SELECT, strAddr, usPort);
				if (m_NetworkPtr->Connect(m_krConnectCenter, usPort, *strAddr) == false) {
					LOGV_ERROR(m_FileLog, TF("[选择服务器]连接中心服务器[%s]:%d请求失败"), *strAddr, usPort);
					return false;
				}
				LOGV_INFO(m_FileLog, TF("[选择服务器]连接中心服务器[%s]:%d请求完成"), *strAddr, usPort);
			}
		}
		else {
			LOG_ERROR(m_FileLog, TF("[选择服务器]选择服务器和中心服务器在不同进程, 创建连接中心服务器Socket失败"));
			return false;
		}
	}
	else if (m_bCenterCnnted == false) {
		assert(m_pShareCenterSvr == nullptr);
		CKeyValue::VALUE xValue = { 0 };
		m_pConfig->GetKeyValue()->GetValue(CServerConfig::CenterServer, xValue);
		if (xValue.pValue == nullptr) {
			LOG_ERROR(m_FileLog, TF("[选择服务器]从动态变量表获取同进程中心服务器对象指针无效"));
			return false;
		}
		m_pShareCenterSvr = reinterpret_cast<ICommonServer*>(xValue.pValue);
		m_bCenterCnnted   = true;

		LOG_INFO(m_FileLog, TF("[选择服务器]同进程直接连接中心服务器"));
		m_ServerInfo.usStatus = STATUSU_LINK;

		CSelectLink Link;
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
// 运行创建UDP监听客户端选择空闲登陆服务器的连接对象
bool CSelectServer::StartUDPService(void)
{
	// 开外网UDP, 用于客户端UDP请求登陆服务器地址
	if (m_krUDPService == nullptr) {
		m_krUDPService = m_NetworkPtr->Create(*this, m_ServerInfo.NetAddr[SELECTI_UDP], SOCKET_UDP);
		if (m_krUDPService != nullptr) {
			LOGV_INFO(m_FileLog, TF("[选择服务器]创建外网UDP服务成功"));
		}
		else {
			LOGV_ERROR(m_FileLog, TF("[选择服务器]创建外网UDP服务失败"));
			return false;
		}
	}
	return (m_krUDPService != nullptr);
}
//--------------------------------------
bool CSelectServer::Pause(bool bPause)
{
	if (bPause && (m_nStatus == STATUSC_RUN)) {
		m_nStatus = STATUSC_PAUSE; 
		LOG_INFO(m_FileLog, TF("[选择服务器]暂停UDP服务"));
		return true;
	}
	else if ((bPause == false) && (m_nStatus == STATUSC_PAUSE)) {
		m_nStatus = STATUSC_RUN;   
		LOG_INFO(m_FileLog, TF("[选择服务器]允许UDP服务"));
		return true;
	}
	return false;
}
//--------------------------------------
void CSelectServer::Stop(void)
{
	if (m_nStatus > STATUSC_NONE) {
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务停止开始!"));

		StopUDPService();
		StopConnectCenterServer();

		m_ServerInfo.Zero();
		m_LoginInfo.Reset();
		m_SortChain.Reset();
		m_LoginSvrMap.RemoveAll();

		m_nStatus = STATUSC_INIT;
		LOG_INFO(m_FileLog, TF("[选择服务器]选择服务停止完成!"));
	}
}
// 停止连接中心服务器
void CSelectServer::StopConnectCenterServer(void)
{
	if (m_krConnectCenter != nullptr) {
		m_NetworkPtr->Destroy(m_krConnectCenter, false);
		m_krConnectCenter = nullptr;
		LOG_INFO(m_FileLog, TF("[选择服务器]销毁连接中心服务器的连接成功"));
	}
	m_bCenterCnnted   = false;
	m_bCenterLinked   = false;
	m_pShareCenterSvr = nullptr;
}
// 停止UDP监听客户端连接
void CSelectServer::StopUDPService(void)
{
	if (m_krUDPService != nullptr) {
		m_NetworkPtr->Destroy(m_krUDPService, false);
		m_krUDPService = nullptr;
		LOG_INFO(m_FileLog, TF("[选择服务器]销毁UDP服务的连接成功"));
	}
}
//--------------------------------------
bool CSelectServer::OnShareRoutine(Int nEvent, CStream& Stream, LLong)
{
	assert((m_pConfig->GetLoadServers() & CServerConfig::CFG_DEFAULT_CENTER) != 0);
	if (nEvent == PAK_EVENT_SYNC) {
		LOGV_INFO(m_FileLog, TF("[选择服务器]同进程同步登录服务器信息"));
		CStreamScopePtr StreamPtr;
		if (m_NetworkPtr->ReferBuffer(StreamPtr, Stream)) { // 同进程的Stream是写模式, 创建一个读模式的引用
			CPAKSync Sync;
			Sync.Serialize(*StreamPtr);

			m_LoginInfo.Copy(Sync.GetServerData());
			SyncLoginSortInfo(*StreamPtr);
		}
		else {
			LOGV_WARN(m_FileLog, TF("[选择服务器]同进程同步登录服务器信息错误"));
		}
	}
	else {
		LOGV_WARN(m_FileLog, TF("[选择服务器]同进程服务器无法识别的信令包数据[event=%d]"), nEvent);
		assert(0);
	}
	return true;
}
//--------------------------------------
bool CSelectServer::OnTcpDispatch(const PacketPtr& PktPtr, PTCP_PARAM pTcp)
{
	// PTCP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PTCP_PARAM->pCache, PTCP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PTCP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	assert(pTcp->krSocket == m_krConnectCenter);
	assert(PktPtr->GetType() == PAK_TYPE_CENTER);
	switch (PktPtr->GetEvent()) {
	case PAK_EVENT_LINKACK:
		{
			m_bCenterLinked = true;

			NET_ADDR NetAddr;
			m_NetworkPtr->GetAddr(pTcp->krSocket, NetAddr, false);
			m_pUIHandler->OnHandle(PAK_EVENT_LINK, reinterpret_cast<uintptr_t>(&NetAddr), DATA_INDEX_CENTER);
			LOG_INFO(m_FileLog, TF("[选择服务器]收到中心服务器连接回复包"));
		}
		break;
	case PAK_EVENT_UPDATEACK:
		{
			DEV_INFO(TF("[选择服务器]收到中心服务器更新回复包"));
		}
		break;
	case PAK_EVENT_SYNC:
		{
			DEV_INFO(TF("[选择服务器]中心服务器同步登陆服务器信息"));
			CPAKSync* pSync = static_cast<CPAKSync*>(PktPtr.Get());
			m_LoginInfo.Copy(pSync->GetServerData());

			if (pSync->CheckStream()) {
				SyncLoginSortInfo(pSync->GetStream());
			}
			// 发送同步回执包
			CPAKSimple<PAK_EVENT_SYNCACK, PAK_TYPE_SELECT> SyncAck;
			SyncAck.AdjustSize();
			m_NetworkPtr->Send(pTcp->krSocket, SyncAck);
		}
		break;
	case PAK_EVENT_UNLINKACK:
		{
			m_bCenterLinked = false;
			LOG_INFO(m_FileLog, TF("[选择服务器]收到中心服务器断接回复包"));
		}
		break;
	case PAK_EVENT_LIVEACK:
		{
		}
		break;
	default:
		{
			LOGV_WARN(m_FileLog, TF("[选择服务器]%p连接信息无法识别的中心服务器信令包数据[event=%d]"), pTcp->krSocket, PktPtr->GetEvent());
		}
	}
	return true;
}
//--------------------------------------
bool CSelectServer::OnUdpDispatch(const PacketPtr& PktPtr, PUDP_PARAM pUdp)
{
	// PUDP_PARAM->pCache == nullptr, 上层可以保存从底层申请的缓存数据
	// PUDP_PARAM->pCache, PUDP_PARAM->index是底层申请的缓存数据的起始地址和索引号
	// 如果上层保存缓存, 设置PUDP_PARAM->pData为nullptr通知网络底层
	// 释放缓存使用MObject::sMCFree(index, pCache)
	bool bRet = false;
	if ((PktPtr->GetType() == PAK_TYPE_CLIENT_READY) && (PktPtr->GetEvent() == PAK_EVENT_SELECT_LOGIN)) {
		m_ServerInfo.Incr();
		// 1. 判断IP是否在黑名单
		if (CheckAddrBlacklist(pUdp->NetAddr) == false) {
			// TODO!!! 客户端可以通过DNS智能解析连接同网络的选择服务器
			CPAKSelectLoginAck SelectLoginAck;
			PSORT_RESULT pSortResult = m_SortChain.GetCur();
			if (pSortResult->lCount > 0) {
				Long lIndex = pSortResult->Index();
				DEV_INFO(TF("[选择服务器]选取第%d个登陆服务器"), lIndex);
				SelectLoginAck.SetAddr(pSortResult->Sort[lIndex].Addr);
				SelectLoginAck.SetAck(DATAD_OKAY);
			}
			else {
				SelectLoginAck.SetAck(SELECT_ERROR_NONE_LOGIN);
				DEV_INFO(TF("[选择服务器]没有登录服务器信息错误"));
			}
			SelectLoginAck.AdjustSize();
			bRet = m_NetworkPtr->SendTo(pUdp->krSocket, SelectLoginAck, pUdp->NetAddr);
		}
		//
		m_ServerInfo.Decr();
	}
	else {
		LOGV_WARN(m_FileLog, TF("[选择服务器]无法识别的UDP信令包数据[type=%d, event=%d]"), PktPtr->GetType(), PktPtr->GetEvent());
	}
	return bRet;
}

bool CSelectServer::CheckAddrBlacklist(NET_ADDR& ClientAddr)
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
bool CSelectServer::OnTcpConnect(UInt uError, KeyRef krConnect)
{
	UNREFERENCED_PARAMETER(krConnect);
	assert(krConnect == m_krConnectCenter);
	m_bCenterCnnted = (uError == 0);
	if (m_bCenterCnnted) {
		LinkCenterServer();
	}
	else {
		DEV_INFO(TF("[选择服务器]连接中心服务器失败%X!"), uError);
		LOGV_WARN(m_FileLog, TF("[选择服务器]连接中心服务器失败%X!"), uError);
	}
	return true;
}
// 连接中心服务器
void CSelectServer::LinkCenterServer(void)
{
	if (m_bCenterCnnted && (m_bCenterLinked == false)) {
		m_ServerInfo.usStatus = STATUSU_LINK;

		CSelectLink Link;
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
			DEV_WARN(TF("[选择服务器]连接中心服务器创建网络数据流异常!"));
			LOG_WARN(m_FileLog, TF("[选择服务器]连接中心服务器创建网络数据流异常!"));
		}
	}
}
// 断连中心服务器
void CSelectServer::UnlinkCenterServer(void)
{
	if (m_bCenterCnnted && m_bCenterLinked) {
		m_bCenterLinked = false;
		CPAKSimple<PAK_EVENT_UNLINK, PAK_TYPE_SELECT> Unlink;
		Unlink.AdjustSize();
		m_NetworkPtr->Send(m_krConnectCenter, Unlink);
	}
}
//--------------------------------------
bool CSelectServer::OnTcpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krConnectCenter) {
		m_krConnectCenter = nullptr;
		m_bCenterCnnted   = false;
		m_bCenterLinked   = false;
		DEV_INFO(TF("[选择服务器]连接中心服务器连接断连/断开"));

		m_SortChain.Reset();
		{ CSyncLockWaitScope scope(m_LoginSvrMap.GetLock()); m_LoginSvrMap.RemoveAll(); }
		m_pUIHandler->OnHandle(PAK_EVENT_UNLINK, 0, DATA_INDEX_CENTER);
	}
	return true;
}
//--------------------------------------
bool CSelectServer::OnUdpClose(KeyRef krSocket, LLong)
{
	if (krSocket == m_krUDPService) {
		m_krUDPService = nullptr;
		DEV_INFO(TF("[选择服务器]选择UDP关闭"));
	}
	return true;
}
//--------------------------------------
// 定时检测监听中心服务器连接的连接对象是否有效
bool CSelectServer::CheckConnectCenterServer(void)
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
// 定时检测UDP监听客户端的连接对象是否有效
bool CSelectServer::CheckUDPService(void)
{
	if (m_krUDPService == nullptr) {
		return StartUDPService();
	}
	return true;
}
// 同步服务器信息给界面
bool CSelectServer::SyncServerInfo(void)
{
	if (m_ServerInfo.usStatus == STATUSU_SYNC) {
		CSelectUpdate Update;
		Update.SetServerData(m_ServerInfo);
		Update.AdjustSize();
		if (m_pShareCenterSvr != nullptr) {
			m_pShareCenterSvr->OnShareRoutine(PAK_EVENT_UPDATE, Update, reinterpret_cast<LLong>(this));
		}
		else {
			m_NetworkPtr->Send(m_krConnectCenter, Update);
		}
		m_ServerInfo.usStatus = STATUSU_OKAY;
		return m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_ServerInfo), DATA_INDEX_SELECT);
	}
	else if (m_pShareCenterSvr == nullptr) { // 发送心跳
		CPAKSimple<PAK_EVENT_LIVE, PAK_TYPE_SELECT> Live;
		Live.AdjustSize();
		m_NetworkPtr->Send(m_krConnectCenter, Live);
	}
	return true;
}
// 同步登陆服务器负载情况数组
bool CSelectServer::SyncLoginSortInfo(CStream& Stream)
{
	LLong llPos = (LLong)Stream.Tell();
	m_pUIHandler->OnHandle(PAK_EVENT_SYNC, Stream, DATA_INDEX_LOGIN);
	m_pUIHandler->OnHandle(PAK_EVENT_SYNC, reinterpret_cast<uintptr_t>(&m_LoginInfo), DATA_INDEX_LOGIN);

	Stream.Seek(llPos); // 跳转到数据开始的地方

	Int nCount = 0;
	PSORT_RESULT pSortResult = m_SortChain.GetNext();
	{
		CSyncLockWaitScope scope(m_LoginSvrMap.GetLock());
		m_LoginSvrMap.Serialize(Stream, STATUSU_UPDATE);

		PINDEX index  = m_LoginSvrMap.First();
		while ((index != nullptr) && (nCount < SORT_RESULT::SORTC_LIMIT)) {
			SVR_LOGIN_MAP::SVR_MAP_PAIR* pPair = m_LoginSvrMap.Next(index);
			pSortResult->Sort[nCount].usBusy   = pPair->m_V.usBusy;
			pSortResult->Sort[nCount].usIncr   = pPair->m_V.usIncr;
			pSortResult->Sort[nCount].drServer = pPair->m_K;
			pSortResult->Sort[nCount].Addr     = pPair->m_V.NetAddr[LOGINI_TCP];
			++nCount;
		}
	}
	if (nCount > 1) {
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
	m_SortChain.Swap(); // 交换登陆排序的数据, 将当前更新的作为有效数据
	LOGV_INFO(m_FileLog, TF("[选择服务器]更新登陆服务器负载情况数组有%d个登陆服务器信息"), nCount);
	return (nCount > 0);
}




