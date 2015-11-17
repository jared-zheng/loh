//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GateRoutine.cpp                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 网关服务器事务实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GateServerImp.h"
#include "GateRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGateRoutine
IMPLEMENT_RTTI_BEGIN_DUMP_CREATE( CGateRoutine, CObject, CObjectTraits::OBJECTT_TRANSIENT, 12 )
DUMP_RTTI_PTR( m_pServer,       VART_NONE, void )
DUMP_RTTI_STD( m_nQueueCur,     VART_INT,  Int )
DUMP_RTTI_STD( m_nQueueEnd,     VART_INT,  Int )
DUMP_RTTI_STD( m_nQueueLimit,   VART_INT,  Int )
DUMP_RTTI_STD( m_nGameCur,      VART_INT,  Int )
DUMP_RTTI_STD( m_nGameSelect,   VART_INT,  Int )
DUMP_RTTI_STD( m_nGameLimit,    VART_INT,  Int )
DUMP_RTTI_DAT( m_QueueRank,     VART_LIST, SESSION_RANK_LST, sizeof(SESSION_RANK_LST) )
DUMP_RTTI_DAT( m_SessionQueue,  VART_MAP,  SESSION_DATA_MAP, sizeof(SESSION_DATA_MAP) )
DUMP_RTTI_DAT( m_SessionLogin,  VART_MAP,  SESSION_DATA_MAP, sizeof(SESSION_DATA_MAP) )
DUMP_RTTI_DAT( m_SessionGame,   VART_MAP,  SESSION_DATA_MAP, sizeof(SESSION_DATA_MAP) )
DUMP_RTTI_DAT( m_EventQueuePtr, VART_REFCOUNT, CEventQueuePtr, sizeof(CEventQueuePtr) )
END_DUMP_RTTI( CGateRoutine, CObject, 12 )

bool CGateRoutine::OnHandle(Int nEvent, uintptr_t utParam, LLong llParam)
{
	if (nEvent == PAK_EVENT_LIVE) {// 客户端连接, 黑名单检测
		CNETTraits::NET_ADDR* pAddr = reinterpret_cast<CNETTraits::NET_ADDR*>(utParam);
		return (CheckAddrBlacklist(*pAddr) == false);
	}
	else if (nEvent == PAK_EVENT_UNLINK) {
		// 客户端断连, 通知游戏服务器保存角色数据
		utParam; // session-id
		llParam; // queue & select just remove user info
	}
	return true;
}

bool CGateRoutine::OnHandle(Int nEvent, CEventBase& EventRef, LLong llParam)
{
	switch (nEvent) {
	// gamedb
	case PAK_EVENT_GATE_SELECTACK:
		{
			SelectRoleAck(static_cast<CPAKSessionAck*>(&EventRef));
		}
		break;
	case PAK_EVENT_GATE_PLAYACK:
		{
			PlayRoleAck(static_cast<CPAKSessionAck*>(&EventRef));
		}
		break;
	//case PAK_EVENT_GATE_CREATEACK:
	//case PAK_EVENT_GATE_DELETEACK:
	//case PAK_EVENT_GATE_RECOVERACK:
	//	{
	//	}
	//	break;
	// game
	case PAK_EVENT_LOGIN_SELECT_GAME: // 选择游戏服务器
		{
			SelectGame(static_cast<CPAKLoginSelectGame*>(&EventRef), llParam);
		}
		break;
	// client
	case PAK_EVENT_LOGIN_LINK_GAME: // 连接游戏服务器
		{
			LinkGame(static_cast<CPAKLoginLinkGame*>(&EventRef), (KeyRef)llParam);
		}
		break;
	case PAK_EVENT_GATE_QUEUE: // 当前排队序号查询
		{
			QueueRank(static_cast<CPAKSession*>(&EventRef), (KeyRef)llParam);
		}
		break;
	case PAK_EVENT_GATE_SELECT: // 角色选择
		{
			SelectRole(static_cast<CPAKGateSelect*>(&EventRef), (KeyRef)llParam);
		}
		break;
	case PAK_EVENT_GATE_PLAY: // 进入游戏世界
		{
			PlayRole(static_cast<CPAKGatePlay*>(&EventRef), (KeyRef)llParam);
		}
		break;
	default:
		{
			if ((nEvent > PAK_EVENT_REALM_BEGIN) && (nEvent < PAK_EVENT_REALM_END)) {
				// 检测是否游戏进行状态, 是的转发给游戏服务器
			}
		}
	}
	return true;
}

bool CGateRoutine::Start(class ICommonServer* pServer)
{
	m_pServer = static_cast<CGateServer*>(pServer);
	if (m_pServer != nullptr) {

		CStringFix strExtConfig;
		if (m_pServer->GetConfig()->GetServerExtConfig(CServerConfig::CFG_DEFAULT_GATE, strExtConfig)) {
			m_pServer->GetConfig()->GetFilePath()->GetFullPath(CFilePath::ModulePath, strExtConfig);
			CIni ini(*strExtConfig); // 可以使用xml, 如果数据复杂
			if (ini.IsExist()) {
				m_nQueueLimit = (Int)ini.GetKeyIntValue(TF("GateRoutine"), TF("QueueLimit"), SESSION_LIMIT_QUEUE);
				m_nGameLimit  = (Int)ini.GetKeyIntValue(TF("GateRoutine"), TF("GameLimit"),  SESSION_LIMIT_GAME);
			}
		}

		// 默认队列处理线程为1
		if (CEventQueue::EventQueue(m_EventQueuePtr, *this, 0, m_pServer->GetConfig()->GetServerQueue(CServerConfig::CFG_DEFAULT_GATE))) {
			return m_EventQueuePtr->Init();
		}
	}
	return false;
}

bool CGateRoutine::Pause(bool)
{
	return false;
}

bool CGateRoutine::Update(void)
{
	CheckSessionTimeout();
	return true;
}

void CGateRoutine::Stop(void)
{
	if (m_EventQueuePtr != nullptr) {
		m_EventQueuePtr->Exit();
		m_EventQueuePtr = nullptr;
	}
	m_pServer = nullptr;
}

bool CGateRoutine::Add(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	return m_EventQueuePtr->Add(nEvent, EventRef, llParam, eType);
}
//--------------------------------------
bool CGateRoutine::CheckAddrBlacklist(CNETTraits::NET_ADDR& ClientAddr)
{
	// TODO!!! 黑名单校验未完成
	size_t stHash = 0;
	if (m_pServer->GetConfig()->GetNetworkAttr() & CNETTraits::ATTR_IPV6) {
		stHash = CHash::Hash(ClientAddr.Addr.bAddr, CNETTraits::NET_ADDR::LEN_BYTE);
	}
	else {
		stHash = (size_t)ClientAddr.Addr.uAddr[0];
	}
	// 1. 是否在永久黑名单
	// 2. 是否在定时黑名单(短期是否有多次连续请求->增加屏蔽时间)--管理员查看可以将屏蔽时间长的加入永久黑名单
	// 3. 临时地址里面短期是否有多次连续请求->存在加入定时黑名单
	return (stHash == 0);
}

bool CGateRoutine::CheckSessionTimeout(void)
{
	//TIMEOUT_MAP Timeout;
	//{
	//	CSyncLockWaitScope scope(m_SessionMap.GetLock());
	//	PINDEX index = m_SessionMap.First();
	//	while (index != nullptr) {
	//		SESSION_MAP::DATA_MAP_PAIR* pPair = m_SessionMap.Next(index);
	//		switch (pPair->m_V.nStatus) {
	//		case SESSION_DATA::SESSION_STATUS_LINK:
	//		case SESSION_DATA::SESSION_STATUS_SELECT:
	//		case SESSION_DATA::SESSION_STATUS_GAME:
	//		case SESSION_DATA::SESSION_STATUS_UNLINK:
	//			{
	//				if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_OUT_MIN)) {
	//					Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_ACK_TIMEOUT);
	//				}
	//			}
	//			break;
	//		case SESSION_DATA::SESSION_STATUS_SELECTACK:
	//		case SESSION_DATA::SESSION_STATUS_GAMEACK:
	//			{
	//				if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_OUT_MAX)) {
	//					Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_GAME_PLAY);
	//				}
	//			}
	//			break;
	//		case SESSION_DATA::SESSION_STATUS_NONE:
	//		case SESSION_DATA::SESSION_STATUS_LINKACK:
	//		default:
	//			{
	//				if (CPlatform::GetRunningTime() > (pPair->m_V.llTimeout + SESSION_DATA::SESSION_TIME_LIMIT)) {
	//					Timeout.Add((KeyRef)pPair->m_K, LOGIN_ERROR_LINK_IDLE);
	//				}
	//			}
	//		}
	//	}
	//}
	//CPAKAck UnlinkAck(PAK_EVENT_LOGIN_UNLINKACK, (UInt)PAK_TYPE_LOGIN);
	//UnlinkAck.AdjustSize();
	//for (PINDEX index = Timeout.GetFirst(); index != nullptr; ) {
	//	TIMEOUT_MAP_PAIR* pPair = Timeout.GetNext(index);
	//	UnlinkAck.SetAck(pPair->m_V);
	//	m_NetworkPtr->Send(pPair->m_K, UnlinkAck, SEND_CLOSE);
	//}
	return true;
}

void CGateRoutine::SelectGame(CPAKLoginSelectGame* pSelect, LLong llParam)
{
	pSelect->SetType(PAK_TYPE_GATE);
	DEV_INFO(TF("[网关服务器]收到登陆服务器转发的客户端登陆游戏请求, SessionId=%llx, UserId=%llx"), pSelect->GetSessionId(), pSelect->GetAuthCode());

	ICommonServer* pLoginServer    = reinterpret_cast<ICommonServer*>(llParam);
	CNETTraits::NET_ADDR AddrLogin = pSelect->GetAddr();

	if ((m_SessionLogin.Size() + m_SessionQueue.Size()) < m_nQueueLimit) { // 没有线程锁, 具体人数会有一定的误差, 应该不会影响游戏
		SESSION_DATA LoginData;
		LoginData.llUserId     = pSelect->GetAuthCode();
		LoginData.llTimeout    = CPlatform::GetRunningTime(); // 加入队列的时间, 超时SESSION_TIME_SELECT_GAME将被删除
		LoginData.llLoginIndex = pSelect->GetSessionId();
		LoginData.pLoginServer = pLoginServer;
		LoginData.AddrLogin    = pSelect->GetAddr();
		{
			CSyncLockWaitScope scope(m_SessionLogin.GetLock());
			assert(m_SessionLogin.Find((DataRef)pSelect->GetSessionId()) == nullptr);
			m_SessionLogin.Add((DataRef)pSelect->GetSessionId(), LoginData);
		}
		pSelect->SetAddr(m_pServer->GetGateAddr().NetAddr[GATEI_TCP]);
		pSelect->SetAck(DATAD_OKAY);
	}
	else {
		pSelect->SetAck(LOGIN_ERROR_QUEUE_LIMIT);
	}
	if (pLoginServer != nullptr) {
		pLoginServer->OnShareRoutine(PAK_EVENT_LOGIN_SELECT_GAME, *pSelect);
	}
	else {
		m_pServer->GetNetworkPtr()->SendTo(m_pServer->GetUDPService(), *pSelect, AddrLogin);
	}
}

void CGateRoutine::LinkGame(CPAKLoginLinkGame* pLink, KeyRef krSocket)
{
	pLink->SetType(PAK_TYPE_GATE);
	DEV_INFO(TF("[网关服务器]收到客户端登陆游戏请求, SessionId=%llx, UserId=%llx"), pLink->GetSessionId(), pLink->GetAuthCode());
	LLong llTemp = pLink->GetSessionId();
	if (llTemp != 0) {
		llTemp = pLink->GetAuthCode();
		llTemp ^= (LLong)krSocket;
		LinkGame2(llTemp, pLink, krSocket);
	}
	else { // 验证码1校验
		/////////////////////////////////////////////////////////////////
		// 加密AuthCode(与客户端唯一性信息有关, 例如异或IP), 这里要和loginserver发送给客户端的加密方式对应
		CNETTraits::NET_ADDR AddrClient;
		m_pServer->GetNetworkPtr()->GetAddr(krSocket, AddrClient);

		llTemp = pLink->GetAuthCode();
		if (AddrClient.usAttr & CNETTraits::ATTR_IPV6) {
			llTemp ^= AddrClient.Addr.ullAddr[0];
			llTemp ^= AddrClient.Addr.ullAddr[1];
			DEV_INFO(TF("[网关服务器]加密Key前后为[%llx-%llx-%llx+%llx]"), pLink->GetAuthCode(), llTemp, AddrClient.Addr.ullAddr[0], AddrClient.Addr.ullAddr[1]);
		}
		else {
			llTemp ^= AddrClient.Addr.uAddr[0];
			DEV_INFO(TF("[网关服务器]加密Key前后为[%p-%p-%x]"), pLink->GetAuthCode(), llTemp, AddrClient.Addr.uAddr[0]);
		}
		/////////////////////////////////////////////////////////////////
		LinkGame1(llTemp, pLink, krSocket);
	}
}

void CGateRoutine::LinkGame1(LLong llDataRef, CPAKLoginLinkGame* pLink, KeyRef krSocket)
{
	SESSION_DATA LoginData;
	{
		CSyncLockWaitScope scope(m_SessionLogin.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionLogin.Find((DataRef)llDataRef);
		if (pPair != nullptr) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			if (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_NONE) {
				pPair->m_V.nStatus = SESSION_DATA::SESSION_STATUS_READY;
				LoginData = pPair->m_V;
			}
		}
	}
	if (LoginData.llUserId != 0) {
		DEV_INFO(TF("[网关服务器]客户端验证码1校验完成"));
		pLink->SetAck(DATAD_OKAY);
		pLink->SetSessionId(llDataRef);
		pLink->SetAuthCode(llDataRef ^ (LLong)krSocket); // 验证码2 : login-sessionid ^ gate-socket id
		if (LoginData.pLoginServer != nullptr) {
			LoginData.pLoginServer->OnShareRoutine(PAK_EVENT_LOGIN_LINK_GAME, *pLink);
		}
		else {
			m_pServer->GetNetworkPtr()->SendTo(m_pServer->GetUDPService(), *pLink, LoginData.AddrLogin);
		}
	}
	else {
		DEV_INFO(TF("[网关服务器]客户端验证码1校验无效"));
		CPAKAck LinkGameAck(PAK_EVENT_LOGIN_LINK_GAME, (UInt)PAK_TYPE_GATE);
		LinkGameAck.SetAck(GATE_ERROR_AUTHCODE1);
		LinkGameAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, LinkGameAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::LinkGame2(LLong llDataRef, CPAKLoginLinkGame* pLink, KeyRef krSocket)
{
	Int nQueue = -1;
	SESSION_DATA Data;
	{
		CSyncLockWaitScope scope(m_SessionLogin.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionLogin.Find((DataRef)llDataRef);
		if (pPair != nullptr) {
			Data = pPair->m_V;
			m_SessionLogin.RemoveAt(reinterpret_cast<PINDEX>(pPair));
		}
	}
	if (Data.llUserId == pLink->GetSessionId()) {
		Data.llTimeout = CPlatform::GetRunningTime();
		Data.llOnline  = Data.llTimeout;

		if (m_nGameCur < m_nGameLimit) { // 直接加入游戏队列
			CAtomics::Increment<UInt>((PUInt)&m_nGameCur);
			CAtomics::Increment<UInt>((PUInt)&m_nGameSelect);

			Data.nStatus   = SESSION_DATA::SESSION_STATUS_GAME;
			CSyncLockWaitScope scope(m_SessionGame.GetLock());
			m_SessionGame.Add((DataRef)krSocket, Data);

			nQueue = 0; // 直接进入游戏, 不需要排队
			m_pServer->GetNetworkPtr()->SetAttr(krSocket, PAK_TYPE_CLIENT_SELECT);
		}
		else {	// 进入排队
			if ((m_nQueueEnd > 0) && (m_nQueueCur == m_nQueueEnd)) { // 排队队列空时, 重置
				m_nQueueEnd = 0;
				m_nQueueCur = 0;
			}
			++m_nQueueEnd;
			nQueue       = m_nQueueEnd;
			Data.nData   = m_nQueueEnd;
			Data.nStatus = SESSION_DATA::SESSION_STATUS_QUEUE;
			{
				CSyncLockWaitScope scope(m_SessionQueue.GetLock());
				PINDEX inxQueue = m_SessionQueue.Add((DataRef)krSocket, Data);
				PINDEX inxRank = m_QueueRank.AddTail(inxQueue);
				m_SessionQueue.Get(inxQueue)->m_V.index = inxRank;
			}
			m_pServer->GetNetworkPtr()->SetAttr(krSocket, PAK_TYPE_CLIENT_QUEUE);
		}
		DEV_INFO(TF("[网关服务器]客户端验证码2校验完成"));
		pLink->SetAck(DATAD_OKAY);
		pLink->SetAuthCode(nQueue); // nQueue = 0; // 直接进入游戏, 不需要排队
		m_pServer->GetNetworkPtr()->Send(krSocket, *pLink);
		// 通知对应的登录服务器用户已经完成游戏服务器的登陆过程
		CPAKSessionAck PlayAck(PAK_EVENT_LOGIN_QUEUE_PLAY, (UInt)PAK_TYPE_GATE);
		PlayAck.SetSessionId(llDataRef);
		PlayAck.AdjustSize();
		if (Data.pLoginServer != nullptr) {
			Data.pLoginServer->OnShareRoutine(PAK_EVENT_LOGIN_QUEUE_PLAY, PlayAck);
		}
		else {
			m_pServer->GetNetworkPtr()->SendTo(m_pServer->GetUDPService(), PlayAck, Data.AddrLogin);
		}
	}
	else {
		DEV_INFO(TF("[网关服务器]客户端验证码2校验无效或者session-id(user-id)错误"));
		CPAKAck LinkGameAck(PAK_EVENT_LOGIN_LINK_GAME, (UInt)PAK_TYPE_GATE);
		LinkGameAck.SetAck((Data.llUserId == pLink->GetSessionId()) ? GATE_ERROR_AUTHCODE2 : GATE_ERROR_SESSION);
		LinkGameAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, LinkGameAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::QueueRank(CPAKSession* pQueue, KeyRef krSocket)
{
	UInt  uAck   = GATE_ERROR_QUEUE;
	Int   nParam = -1;
	{
		CSyncLockWaitScope scope(m_SessionQueue.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionQueue.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			if (pQueue->GetSessionId() == pPair->m_V.llUserId) {
				uAck   = DATAD_OKAY;
				nParam = pPair->m_V.nData - m_nQueueCur;
			}
			else {
				uAck = GATE_ERROR_SESSION;
			}
		}
	}
	if (uAck == GATE_ERROR_QUEUE) {
		CSyncLockWaitScope scope(m_SessionGame.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionGame.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			//pPair->m_V.llTimeout = CPlatform::GetRunningTime(); // 错误的排队信息查询, 不更新时间
			uAck   = GATE_ERROR_GAME;
			nParam = pPair->m_V.nStatus; // 在游戏进行中的, 返回当前状态
		}
	}
	if (nParam >= 0) {
		DEV_INFO(TF("[网关服务器]客户端查询排队信息成功返回%d"), nParam);
		CPAKGateQueueAck QueueAck;
		QueueAck.SetAck(uAck);
		QueueAck.SetParam(nParam);
		QueueAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, QueueAck);
	}
	else { // 
		DEV_INFO(TF("[网关服务器]客户端查询排队信息失败"));
		CPAKAck QueueAck(PAK_EVENT_GATE_QUEUEACK, (UInt)PAK_TYPE_GATE);
		QueueAck.SetAck(uAck);
		QueueAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, QueueAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::SelectRole(CPAKGateSelect* pSelect, KeyRef krSocket)
{
	UInt uAck = GATE_ERROR_SESSION;
	{
		CSyncLockWaitScope scope(m_SessionGame.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionGame.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			if (pPair->m_V.llUserId != pSelect->GetSessionId()) {
				uAck  = GATE_ERROR_SESSION;
			}
			else if (pPair->m_V.llUserId != pSelect->GetUserId()) {
				uAck  = GATE_ERROR_USERID;
			}
			else if (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_GAME) {
				pPair->m_V.nStatus = SESSION_DATA::SESSION_STATUS_SELECT;
				uAck   = DATAD_OKAY;
			}
		}
	}
	if (uAck == DATAD_OKAY) { // 向游戏DB服务器发送请求角色信息
		pSelect->SetSessionId((LLong)krSocket);
		ICommonServer* pGameDBServer = m_pServer->GetShareGameDBServer();
		if (pGameDBServer != nullptr) {
			pGameDBServer->OnShareRoutine(PAK_EVENT_GATE_SELECT, *pSelect);
		}
		else {
			m_pServer->GetNetworkPtr()->Send(m_pServer->GetConnectGameDB(), *pSelect);
		}
	}
	else { // 
		DEV_INFO(TF("[网关服务器]客户端请求角色信息错误"));
		CPAKAck SelectAck(PAK_EVENT_GATE_SELECTACK, (UInt)PAK_TYPE_GATE);
		SelectAck.SetAck(uAck);
		SelectAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, SelectAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::PlayRole(CPAKGatePlay* pPlay, KeyRef krSocket)
{
	UInt uAck = GATE_ERROR_SESSION;
	{
		CSyncLockWaitScope scope(m_SessionGame.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionGame.Find((DataRef)krSocket);
		if (pPair != nullptr) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			if (pPair->m_V.llUserId != pPlay->GetSessionId()) {
				uAck  = GATE_ERROR_SESSION;
			}
			else if (pPair->m_V.llUserId != pPlay->GetUserId()) {
				uAck  = GATE_ERROR_USERID;
			}
			else if (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_SELECTACK) {
				pPair->m_V.nStatus = SESSION_DATA::SESSION_STATUS_PLAY;
				pPair->m_V.nData   = pPlay->GetRoleId();
				uAck   = DATAD_OKAY;
			}
		}
	}
	if (uAck == DATAD_OKAY) { // 向游戏服务器发送请求
		pPlay->SetSessionId((LLong)krSocket);
		ICommonServer* pGameServer = m_pServer->GetShareGameServer();
		if (pGameServer != nullptr) {
			pGameServer->OnShareRoutine(PAK_EVENT_GATE_PLAY, *pPlay);
		}
		else {
			m_pServer->GetNetworkPtr()->Send(m_pServer->GetConnectGame(), *pPlay);
		}
	}
	else { // 
		DEV_INFO(TF("[网关服务器]客户端请求进入游戏世界错误"));
		CPAKAck PlayAck(PAK_EVENT_GATE_PLAYACK, (UInt)PAK_TYPE_GATE);
		PlayAck.SetAck(uAck);
		PlayAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send(krSocket, PlayAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::SelectRoleAck(CPAKSessionAck* pAck)
{
	UInt uAck = pAck->GetAck();
	if (uAck == DATAD_OKAY) {
		pAck->SetType(PAK_TYPE_GATE);

		CSyncLockWaitScope scope(m_SessionGame.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionGame.Find((DataRef)pAck->GetSessionId());
		if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_SELECT)) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_SELECTACK;

			pAck->SetSessionId(pPair->m_V.llUserId);
			m_pServer->GetNetworkPtr()->Send((KeyRef)pPair->m_K, *pAck);
		}
		else {
			uAck = GATE_ERROR_SESSION;
		}
	}
	if (uAck != DATAD_OKAY) {
		CPAKAck SelectAck(PAK_EVENT_GATE_SELECTACK, (UInt)PAK_TYPE_GATE);
		SelectAck.SetAck(uAck);
		SelectAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send((KeyRef)pAck->GetSessionId(), SelectAck, CNETTraits::SEND_CLOSE);
	}
}

void CGateRoutine::PlayRoleAck(CPAKSessionAck* pAck)
{
	UInt uAck = pAck->GetAck();
	if (uAck == DATAD_OKAY) {
		pAck->SetType(PAK_TYPE_GATE);

		CSyncLockWaitScope scope(m_SessionGame.GetLock());
		SESSION_DATA_MAP::DATA_MAP_PAIR* pPair = m_SessionGame.Find((DataRef)pAck->GetSessionId());
		if ((pPair != nullptr) && (pPair->m_V.nStatus == SESSION_DATA::SESSION_STATUS_PLAY)) {
			pPair->m_V.llTimeout = CPlatform::GetRunningTime();
			pPair->m_V.nStatus   = SESSION_DATA::SESSION_STATUS_PLAYACK;

			pAck->SetSessionId(pPair->m_V.llUserId);
			m_pServer->GetNetworkPtr()->Send((KeyRef)pPair->m_K, *pAck);
		}
		else {
			uAck = GATE_ERROR_SESSION;
		}
	}
	if (uAck != DATAD_OKAY) {
		CPAKAck SelectAck(PAK_EVENT_GATE_PLAYACK, (UInt)PAK_TYPE_GATE);
		SelectAck.SetAck(uAck);
		SelectAck.AdjustSize();
		m_pServer->GetNetworkPtr()->Send((KeyRef)pAck->GetSessionId(), SelectAck, CNETTraits::SEND_CLOSE);
	}
}



