//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : GameDBRoutine.cpp                            //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 游戏DB服务器事务实现                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameDBServerImp.h"
#include "GameDBRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBRoutine
IMPLEMENT_RTTI_BEGIN_DUMP_CREATE( CGameDBRoutine, CObject, CObjectTraits::OBJECTT_TRANSIENT, 3 )
DUMP_RTTI_PTR( m_pServer, VART_NONE, void )
DUMP_RTTI_STD( m_stCount, VART_SIZE_T, size_t )
DUMP_RTTI_DAT( m_EventQueuePtr, VART_REFCOUNT, CEventQueuePtr, sizeof(CEventQueuePtr) )
END_DUMP_RTTI( CGameDBRoutine, CObject, 3 )

bool CGameDBRoutine::OnHandle(Int nEvent, CEventBase& EventRef, LLong llParam)
{
	switch (nEvent) {
	// gate
	case PAK_EVENT_GATE_SELECT:
		{
			CPAKGateSelect* pSelect = static_cast<CPAKGateSelect*>(&EventRef);
			// 数据库读取账号在GameId区上的角色信息处理
			LOGV_WARN(m_pServer->GetFileLog(), TF("游戏DB服务器]账号:%lld[session key=%llx!]读取在%d区的角色信息"),
					  pSelect->GetUserId(), pSelect->GetSessionId(), pSelect->GetGameId());
			// 错误返回CPAKSessionAck
			CTRefCountPtr<CPAKGateSelectAck> SelectAckPtr = MNEW CPAKGateSelectAck(PAK_EVENT_GATE_SELECTACK, (UInt)PAK_TYPE_GAMEDB);
			SelectAckPtr->SetSessionId(pSelect->GetSessionId());
			SelectAckPtr->SetRoleCount(1);
			// 角色信息数据
			SelectAckPtr->AdjustSize();

			if (llParam == 0) {
				assert(m_pServer->GetShareGameServer() != nullptr);
				m_pServer->GetShareGameServer()->OnShareRoutine(PAK_EVENT_GATE_SELECTACK, *SelectAckPtr);
			}
			else {
				m_pServer->GetNetworkPtr()->Send((KeyRef)llParam, *SelectAckPtr);
			}
		}
		break;
	case PAK_EVENT_GATE_PLAY:
		{
			CPAKGatePlay* pPlay = static_cast<CPAKGatePlay*>(&EventRef);
			// 数据库读取角色Id的详细信息处理
			LOGV_WARN(m_pServer->GetFileLog(), TF("游戏DB服务器][session key=%llx!]读取角色Id=%d的详细信息缓存"), pPlay->GetSessionId(), pPlay->GetRoleId());
			// 错误返回CPAKSessionAck
			CTRefCountPtr<CPAKSessionAck> PlayAckPtr = MNEW CPAKSessionAck(PAK_EVENT_GATE_PLAYACK, (UInt)PAK_TYPE_GAMEDB);
			PlayAckPtr->SetSessionId(pPlay->GetSessionId());
			PlayAckPtr->AdjustSize();

			if (llParam == 0) {
				assert(m_pServer->GetShareGameServer() != nullptr);
				m_pServer->GetShareGameServer()->OnShareRoutine(PAK_EVENT_GATE_PLAYACK, *PlayAckPtr);
			}
			else {
				m_pServer->GetNetworkPtr()->Send((KeyRef)llParam, *PlayAckPtr);
			}
		}
		break;
	case PAK_EVENT_GATE_CREATE:
		{
		}
		break;
	case PAK_EVENT_GATE_DELETE:
		{
		}
		break;
	case PAK_EVENT_GATE_RECOVER:
		{
		}
		break;
	default:
		{
		}
	}
	return true;
}

bool CGameDBRoutine::Start(class ICommonServer* pServer)
{
	m_pServer = static_cast<CGameDBServer*>(pServer);
	if (m_pServer != nullptr) {
		// 默认队列处理线程为1
		if (CEventQueue::EventQueue(m_EventQueuePtr, *this, 0, m_pServer->GetConfig()->GetServerQueue(CServerConfig::CFG_DEFAULT_GAMEDB))) {
			return m_EventQueuePtr->Init();
		}
	}
	return false;
}

bool CGameDBRoutine::Pause(bool)
{
	return false;
}

bool CGameDBRoutine::Update(void)
{
	return false;
}

void CGameDBRoutine::Stop(void)
{
	if (m_EventQueuePtr != nullptr) {
		m_EventQueuePtr->Exit();
		m_EventQueuePtr = nullptr;
	}
	m_pServer = nullptr;
	m_stCount = 0;
}

bool CGameDBRoutine::Add(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	return m_EventQueuePtr->Add(nEvent, EventRef, llParam, eType);
}


