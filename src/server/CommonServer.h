//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CommonServer.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update :                                                   //
//   Detail : 服务器标准接口                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_SERVER_H__
#define __COMMON_SERVER_H__

#pragma once

#include "subsystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ICommonServer
class NOVTABLE ICommonServer ABSTRACT : public CComponent
{
public:
	//// CComponent
	//// 按配置文件中配置项名称更新相关参数
	//virtual UInt   Command(PCXStr pszCMD, uintptr_t utParam) PURE_METHOD;
	//virtual UInt   Update(void) PURE_METHOD;

	virtual UInt   Init(CEventHandler& EventHandlerRef) PURE_METHOD;
	virtual void   Exit(void) PURE_METHOD;

	virtual bool   Start(void) PURE_METHOD;
	virtual bool   Pause(bool bPause = true) PURE_METHOD;
	virtual void   Stop(void) PURE_METHOD;
	// 同进程共享数据回调接口
	virtual bool   OnShareRoutine(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) PURE_METHOD;
	virtual bool   OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) PURE_METHOD;
	virtual bool   OnShareRoutine(Int nEvent, CStream& Stream, LLong llParam = 0) PURE_METHOD;
};
typedef CTRefCountPtr<ICommonServer> CCommonServerPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommonServerSystem : CSubSystem
DECLARE_UUID( CCommonServerSystem, {782B6B51-2686-443d-A575-62199CAF1576} )

#endif // __COMMON_SERVER_H__
