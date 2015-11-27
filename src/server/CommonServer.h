//////////////////////////////////////////////////////////////////
//   The Legend of Heroes                                       //
//   Copyright(C) LH Studio. All Rights Reserved                //
//                                                              //
//   Header File : CommonServer.h                               //
//   Author : jaredz@outlook.com                                //
//   Create : 2012-12-01     version 0.0.0.1                    //
//   Update : 2015-11-25     version 0.0.0.5                    //
//   Detail : 服务器标准接口                                     //
//                                                              //
//////////////////////////////////////////////////////////////////

#ifndef __COMMON_SERVER_H__
#define __COMMON_SERVER_H__

#pragma once

#include "subsystem.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 服务器标准接口
class NOVTABLE ICommonServer ABSTRACT : public CComponent
{
public:
	//// CComponent
	//// 按配置文件中配置项名称更新相关参数
	//virtual UInt   Command(PCXStr pszCMD, uintptr_t utParam) PURE_METHOD;
	//virtual UInt   Update(void) PURE_METHOD;
	/// 初始化服务器
	/// @param[in] CEventHandler& EventHandlerRef事件处理对象
	/// @return 成功返回RET_OKAY(0)， 非零值表示错误编号
	virtual UInt   Init(CEventHandler& EventHandlerRef) PURE_METHOD;
	/// 结束服务器
	virtual void   Exit(void) PURE_METHOD;
	/// 开启服务
	virtual bool   Start(void) PURE_METHOD;
	/// 暂停服务
	/// @param bool bPause 暂停还是继续值
	virtual bool   Pause(bool bPause = true) PURE_METHOD;
	/// 停止服务
	virtual void   Stop(void) PURE_METHOD;
	/// 同进程共享数据回调接口1
	virtual bool   OnShareRoutine(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_NONE) PURE_METHOD;
	/// 同进程共享数据回调接口2
	virtual bool   OnShareRoutine(Int nEvent, CEventBase& EventRef, LLong llParam = 0, CEventQueue::EVENT_TYPE eType = CEventQueue::EVENT_TYPE_REFCOUNT) PURE_METHOD;
	/// 同进程共享数据回调接口3
	virtual bool   OnShareRoutine(Int nEvent, CStream& Stream, LLong llParam = 0) PURE_METHOD;
};
typedef CTRefCountPtr<ICommonServer> CCommonServerPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CCommonServerSystem : CSubSystem
DECLARE_UUID( CCommonServerSystem, {782B6B51-2686-443d-A575-62199CAF1576} )

#endif // __COMMON_SERVER_H__
