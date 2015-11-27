//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CommonRoutine.h                              //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 服务器事务标准接口                                 //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_ROUTINE_H__
#define __COMMON_ROUTINE_H__

#pragma once

#include "object.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器事务标准接口
class NOVTABLE ICommonRoutine ABSTRACT : public CObject
{
public:
	/// 开启事务处理
	/// @param[in] ICommonServer* pServer事务处理的事件源服务器对象
	/// @return 成功返回true， 否则false
	virtual bool Start(class ICommonServer* pServer) PURE_METHOD;
	/// 暂停或者继续
	virtual bool Pause(bool bPause = true) PURE_METHOD;
	/// 更新
	virtual bool Update(void) PURE_METHOD;
	/// 停止
	virtual void Stop(void) PURE_METHOD;
	/// 加入事件到队列1
	virtual bool Add(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) PURE_METHOD;
	/// 加入事件到队列2
	virtual bool Add(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) PURE_METHOD;
	/// 加入事件到队列3
	virtual bool Add(Int nEvent, CStream& Stream, LLong llParam = 0) PURE_METHOD;
};
typedef CTScopePtr<ICommonRoutine> CCommonRoutinePtr;

#endif // __COMMON_ROUTINE_H__
