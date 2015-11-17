//////////////////////////////////////////////////////////////////
//   The Legend of Heros                                        //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CommonRoutine.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务器事务标准接口                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_ROUTINE_H__
#define __COMMON_ROUTINE_H__

#pragma once

#include "object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ICommonRoutine
class NOVTABLE ICommonRoutine ABSTRACT : public CObject
{
public:
	virtual bool Start(class ICommonServer* pServer) PURE_METHOD;
	virtual bool Pause(bool bPause = true) PURE_METHOD;
	virtual bool Update(void) PURE_METHOD;
	virtual void Stop(void) PURE_METHOD;

	virtual bool Add(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) PURE_METHOD;
	virtual bool Add(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) PURE_METHOD;
	virtual bool Add(Int nEvent, CStream& Stream, LLong llParam = 0) PURE_METHOD;
};
typedef CTScopePtr<ICommonRoutine> CCommonRoutinePtr;

#endif // __COMMON_ROUTINE_H__
