//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : GameDBRoutine.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 游戏DB服务器事务实现                               //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __GAMEDB_ROUTINE_H__
#define __GAMEDB_ROUTINE_H__

#pragma once

#include "CommonRoutine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameDBRoutine
class CGameDBRoutine : public ICommonRoutine
{
	DECLARE_RTTI_CREATE( CGameDBRoutine )
public:
	CGameDBRoutine(void);
	virtual ~CGameDBRoutine(void);
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
	class CGameDBServer*   m_pServer;
	size_t                 m_stCount;         // 临时的ID号
	CEventQueuePtr         m_EventQueuePtr;   // 事件队列  
	// 1. 从服务器扩展配置文件中读取数据库信息
	// 2. 角色的时间点数需要用户在网站充入游戏服务器
};

INLINE CGameDBRoutine::CGameDBRoutine(void)
: m_pServer(nullptr)
, m_stCount(0)
{
}

INLINE CGameDBRoutine::~CGameDBRoutine(void)
{
}

INLINE bool CGameDBRoutine::OnHandle(Int, uintptr_t, LLong)
{
	return false;
}

INLINE bool CGameDBRoutine::OnHandle(Int, CStream&, LLong)
{ 
	return false; 
}

INLINE bool CGameDBRoutine::OnHandle(Int, UInt)
{ 
	return false;
}

INLINE bool CGameDBRoutine::Add(Int, uintptr_t, LLong, CEventQueue::EVENT_TYPE)
{
	return false;
}

INLINE bool CGameDBRoutine::Add(Int, CStream&, LLong)
{
	return false;
}

#endif // __GAMEDB_ROUTINE_H__
