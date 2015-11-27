//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Source File : ZoneRoutine.cpp                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器事务实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZoneServerImp.h"
#include "ZoneRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CZoneRoutine
IMPLEMENT_RTTI_BEGIN_DUMP_CREATE( CZoneRoutine, CObject, CObjectTraits::OBJECTT_TRANSIENT, 3 )
DUMP_RTTI_PTR( m_pServer, VART_NONE, void )
DUMP_RTTI_STD( m_stCount, VART_SIZE_T, size_t )
DUMP_RTTI_DAT( m_EventQueuePtr, VART_REFCOUNT, CEventQueuePtr, sizeof(CEventQueuePtr) )
END_DUMP_RTTI( CZoneRoutine, CObject, 3 )

bool CZoneRoutine::OnHandle(Int nEvent, CEventBase& EventRef, LLong llParam)
{
	switch (nEvent) {
	case 0:
		{
		}
		break;
	default:
		{
		}
	}
	return true;
}

bool CZoneRoutine::Start(class ICommonServer* pServer)
{
	m_pServer = static_cast<CZoneServer*>(pServer);
	if (m_pServer != nullptr) {
		// 默认队列处理线程为1
		if (CEventQueue::EventQueue(m_EventQueuePtr, *this, 0, m_pServer->GetConfig()->GetServerQueue(CServerConfig::CFG_DEFAULT_GATE))) {
			return m_EventQueuePtr->Init();
		}
	}
	return false;
}

bool CZoneRoutine::Pause(bool)
{
	return false;
}

bool CZoneRoutine::Update(void)
{
	return false;
}

void CZoneRoutine::Stop(void)
{
	if (m_EventQueuePtr != nullptr) {
		m_EventQueuePtr->Exit();
		m_EventQueuePtr = nullptr;
	}
	m_pServer = nullptr;
	m_stCount = 0;
}

bool CZoneRoutine::Add(Int nEvent, CEventBase& EventRef, LLong llParam, CEventQueue::EVENT_TYPE eType)
{
	return m_EventQueuePtr->Add(nEvent, EventRef, llParam, eType);
}


