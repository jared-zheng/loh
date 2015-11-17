///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : event.h                                                                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : asynchronous events                                                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __EVENT_H__
#define __EVENT_H__

#pragma once

#include "streambuf.h"
#include "refcount.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEventBase
class NOVTABLE CEventBase ABSTRACT : public CTRefCount<CEventBase>
{
public:
	virtual size_t Length(void) PURE_METHOD;
	virtual void   Serialize(CStream&) PURE_METHOD;
protected:
	CEventBase(void);
	virtual ~CEventBase(void);

	CEventBase(const CEventBase&);
	CEventBase& operator=(const CEventBase&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEventHandler
class NOVTABLE CEventHandler ABSTRACT : public MObject
{
public:
	// EVENT_TYPE_NONE & EVENT_TYPE_USERCACHE
	virtual bool OnHandle(Int nEvent, uintptr_t utParam, LLong llParam) PURE_METHOD;
	// EVENT_TYPE_REFCOUNT
	virtual bool OnHandle(Int nEvent, CEventBase& EventRef, LLong llParam) PURE_METHOD;
	// EVENT_TYPE_MANAGECACHE
	virtual bool OnHandle(Int nEvent, CStream& Stream, LLong llParam) PURE_METHOD;
	// tick event
	virtual bool OnHandle(Int nEvent, UInt uCount) PURE_METHOD;
protected:
	CEventHandler(void);
	virtual ~CEventHandler(void);
private:
	CEventHandler(const CEventHandler&);
	CEventHandler& operator=(const CEventHandler&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CEventQueue
class CEventQueue;
typedef CTRefCountPtr<CEventQueue> CEventQueuePtr;

class CORECLASS NOVTABLE CEventQueue : public CTRefCount<CEventQueue>
{
public:
	enum EVENT_TYPE
	{
		EVENT_TYPE_NONE    = 0, // utParam is basic data or utParam is ptr, discard after onhandle->OnHandle(Int, uintptr_t, LLong)
		EVENT_TYPE_USERCACHE,   // user cache memory, discard after onhandle->OnHandle(Int, uintptr_t, LLong)
		EVENT_TYPE_REFCOUNT,    // an eventbase instance ptr, Release after onhandle->OnHandle(Int, CEventBase&, LLong)	
		EVENT_TYPE_MANAGECACHE, // copy user data to managed cache memory, delete after onhandle->OnHandle(Int, CStream&, LLong)
	};

	enum EVENT_THREAD
	{
		EVENT_THREAD_MAX = 2,   // 2 * cpu cores
	};

public:
	// stCacheSize --> max cache size the queue used, 0 -- none cache used
	// nThreads    --> max threads the queue used, 0 -- only one
	static  bool EventQueue(CEventQueuePtr& EventQueuePtrRef, CEventHandler& EventHandlerRef, size_t stCacheSize = 0, Int nThreads = 0);
	// tick min time 100ms
	static  bool CreateTickEvent(Int nTick, UInt uInterval, CEventHandler& EventHandlerRef, UInt uCount = (UInt)CSyncTraits::SYNCW_INFINITE);
	static  bool DestroyTickEvent(Int nTick);
public:
	virtual bool Init(void) PURE_METHOD;
	virtual void Exit(void) PURE_METHOD;

	// EVENT_TYPE_NONE
	// EVENT_TYPE_USERCACHE(utParam should be a valid cache ptr, llParam is cache index, sMCCheck before add)
	// EVENT_TYPE_REFCOUNT(utParam should be a valid ptr to a CEventBase instance, sMCheck before add, AddRef)
	// EVENT_TYPE_MANAGECACHE(utParam should be a valid ptr, llParam is size <= stCacheSize)
	virtual bool Add(Int nEvent, uintptr_t utParam = 0, LLong llParam = 0, EVENT_TYPE eType = EVENT_TYPE_NONE) PURE_METHOD;

	// EVENT_TYPE_REFCOUNT(sMCheck before add, AddRef)
	// EVENT_TYPE_MANAGECACHE(CEventBase's Length <= stCacheSize)
	virtual bool Add(Int nEvent, CEventBase& EventRef, LLong llParam = 0, EVENT_TYPE eType = EVENT_TYPE_REFCOUNT) PURE_METHOD;

	// EVENT_TYPE_MANAGECACHE(Stream's size <= stCacheSize, otherwise just copy stCacheSize data)
	// CStream mode read(write stream use refer to get a read mode stream) 
	virtual bool Add(Int nEvent, CStream& Stream, LLong llParam = 0) PURE_METHOD;

	virtual bool MaxCount(Int nMax) PURE_METHOD;                // max queue length
	virtual Int  GetCount(void) PURE_METHOD;                    // current queue length
	virtual Int  GetThreads(void) PURE_METHOD;                  // current work thread count
	virtual void SetThreads(UInt uThreadCount = 0) PURE_METHOD; // 0 --- default 1, max EVENT_THREAD_MAX * cpu cores
protected:
	CEventQueue(void);
	virtual ~CEventQueue(void);
private:
	CEventQueue(const CEventQueue&);
	CEventQueue& operator=(const CEventQueue&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "event.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __EVENT_H__