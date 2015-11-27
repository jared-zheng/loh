//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : ZoneRoutine.h                                //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 网关服务器事务实现                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __ZONE_ROUTINE_H__
#define __ZONE_ROUTINE_H__

#pragma once

#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 网关服务器事务实现
/// - 从服务器扩展配置文件中读取地图加载信息
/// - 处理角色的游戏事务
class CZoneRoutine : public ICommonRoutine
{
	DECLARE_RTTI_CREATE( CZoneRoutine )
public:
	CZoneRoutine(void);
	virtual ~CZoneRoutine(void);
	// CEventHandler
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) OVERRIDE;//
	virtual bool OnHandle(Int nEvent, CEventBase&, LLong) OVERRIDE;
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) OVERRIDE;//
	virtual bool OnHandle(Int nEvent, UInt uCount) OVERRIDE;//
	// ICommonRoutine
	virtual bool Start(class ICommonServer* pServer) OVERRIDE;
	virtual bool Pause(bool bPause = true) OVERRIDE;
	virtual bool Update(void) OVERRIDE;
	virtual void Stop(void) OVERRIDE;

	virtual bool Add(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) OVERRIDE;//
	virtual bool Add(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) OVERRIDE;
	virtual bool Add(Int nEvent, CStream& Stream, LLong llParam = 0) OVERRIDE;//
private:
	class CZoneServer*     m_pServer;         ///< 事务处理对应的服务器
	size_t                 m_stCount;         ///< 临时的ID号
	CEventQueuePtr         m_EventQueuePtr;   ///< 事件队列 
};

INLINE CZoneRoutine::CZoneRoutine(void)
: m_pServer(nullptr)
, m_stCount(0)
{
}

INLINE CZoneRoutine::~CZoneRoutine(void)
{
}

INLINE bool CZoneRoutine::OnHandle(Int, uintptr_t, LLong)
{
	return false;
}

INLINE bool CZoneRoutine::OnHandle(Int, CStream&, LLong)
{ 
	return false; 
}

INLINE bool CZoneRoutine::OnHandle(Int, UInt)
{ 
	return false;
}

INLINE bool CZoneRoutine::Add(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CZoneRoutine::Add(Int, CStream&, LLong)
{
	return false;
}

#endif // __ZONE_ROUTINE_H__
