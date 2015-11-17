///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : targetsync.h                                                                          //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows multiple threads synchronization                                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_SYNC_H__
#define __TARGET_SYNC_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "atomics.h"
#include "handle.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncTraits
class CSyncTraits
{
public:
	enum SYNC_TYPE
	{
		SYNCT_COUNTER = 1,
		SYNCT_LOCK,
		SYNCT_MUTEX,
		SYNCT_EVENT,
		SYNCT_SEMAPHORE,
	};

	enum SYNC_WAIT
	{
		SYNCW_IGNORE   = 0,
		SYNCW_SECOND   = 1000,       // S - MS
		SYNCW_LOCK_MAX = 0x00000F00, // cpu wait spins
		SYNCW_INFINITE = 0xFFFFFFFF,
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSyncBase
template <Int nType>
class NOVTABLE CTSyncBase ABSTRACT : public MObject
{
public:
	virtual Long Signal(void) PURE_METHOD;
	virtual Long Reset(void)  PURE_METHOD;
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) PURE_METHOD;
	virtual bool Valid(void)  PURE_METHOD;

	Int     GetType(void);
protected:
	CTSyncBase(void);
	virtual ~CTSyncBase(void);
private:
	CTSyncBase(const CTSyncBase&);
	CTSyncBase& operator=(const CTSyncBase&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSyncScope
// This is a utility class that handles scope level locking. It's very useful
// to keep from causing deadlocks due to exceptions being caught and knowing
// about the number of locks a given thread has on a resource. Example:
//
// <code>
//	{
//		// Syncronize thread access to the following data
//		CTSyncScope Scope(SynchObject);
//		// Access data that is shared among multiple threads
//		...
//		// When Scope goes out of scope, other threads can access data
//	}
// </code>
//
template <Int nType, UInt uWait = (UInt)CSyncTraits::SYNCW_IGNORE>
class CTSyncScope : public MObject
{
public:
	CTSyncScope(CTSyncBase<nType>& Sync);
	~CTSyncScope(void); 
private:
	CTSyncScope(const CTSyncScope&);
	CTSyncScope& operator=(const CTSyncScope&);
private:
	CTSyncBase<nType>*   m_pSync;
};

typedef CTSyncScope<CSyncTraits::SYNCT_COUNTER>                                      CSyncCounterScope;
typedef CTSyncScope<CSyncTraits::SYNCT_LOCK>                                         CSyncLockScope;
typedef CTSyncScope<CSyncTraits::SYNCT_MUTEX>                                        CSyncMutexScope;
typedef CTSyncScope<CSyncTraits::SYNCT_EVENT>                                        CSyncEventScope;
typedef CTSyncScope<CSyncTraits::SYNCT_SEMAPHORE>                                    CSyncSemaphoreScope;
typedef CTSyncScope<CSyncTraits::SYNCT_COUNTER,   (UInt)CSyncTraits::SYNCW_INFINITE> CSyncCounterWaitScope;
typedef CTSyncScope<CSyncTraits::SYNCT_LOCK,      (UInt)CSyncTraits::SYNCW_LOCK_MAX> CSyncLockWaitScope;
typedef CTSyncScope<CSyncTraits::SYNCT_MUTEX,     (UInt)CSyncTraits::SYNCW_INFINITE> CSyncMutexWaitScope;
typedef CTSyncScope<CSyncTraits::SYNCT_EVENT,     (UInt)CSyncTraits::SYNCW_INFINITE> CSyncEventWaitScope;
typedef CTSyncScope<CSyncTraits::SYNCT_SEMAPHORE, (UInt)CSyncTraits::SYNCW_INFINITE> CSyncSemaphoreWaitScope;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncCounter
class CSyncCounter : public CTSyncBase<CSyncTraits::SYNCT_COUNTER>
{
public:
	CSyncCounter(Long lCounter = 0);
	virtual ~CSyncCounter(void);

	virtual Long Signal(void) OVERRIDE; // return m_lCounter after increment
	virtual Long Reset(void) OVERRIDE;  // return m_lCounter after decrement
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) OVERRIDE; // RETV_OKAY, wait always return okay
	virtual bool Valid(void) OVERRIDE;

	Long    GetCount(void);
private:
	CSyncCounter(const CSyncCounter&);
	CSyncCounter& operator=(const CSyncCounter&);
private:
	Long   m_lCounter;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncLock
class CSyncLock : public CTSyncBase<CSyncTraits::SYNCT_LOCK>
{
public:
	CSyncLock(void);
	virtual ~CSyncLock(void); 

	virtual Long Signal(void) OVERRIDE; // return RETV_OKAY if current thread owns lock; RETV_FAIL other thread owns lock
	virtual Long Reset(void) OVERRIDE; // always return RETV_OKAY
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) OVERRIDE; // RETV_OKAY, wait always return okay
	virtual bool Valid(void) OVERRIDE;
private:
	CSyncLock(const CSyncLock&);
	CSyncLock& operator=(const CSyncLock&);
private:
	UInt              m_uWaitSpin;
	CRITICAL_SECTION  m_SyncLock;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncMutex
class CSyncMutex : public CTSyncBase<CSyncTraits::SYNCT_MUTEX>
{
public:
	CSyncMutex(bool bCreateSignal = false, PCXStr pszName = nullptr);
	virtual ~CSyncMutex(void); 

	virtual Long Signal(void) OVERRIDE; // owns return RETV_OKAY, other return RETV_FAIL
	virtual Long Reset(void) OVERRIDE; // okay return RETV_OKAY, else RETV_FAIL
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) OVERRIDE;
	virtual bool Valid(void) OVERRIDE;
private:
	CSyncMutex(const CSyncMutex&);
	CSyncMutex& operator=(const CSyncMutex&);
private:
	bool      m_bOwned;
	CHandle   m_hMutex;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncEvent
class CSyncEvent : public CTSyncBase<CSyncTraits::SYNCT_EVENT>
{
public:
	CSyncEvent(bool bCreateSignal = false, bool bManualReset = false, PCXStr pszName = nullptr);
	virtual ~CSyncEvent(void); 

	virtual Long Signal(void) OVERRIDE; // signaled return RETV_OKAY, other return RETV_FAIL
	virtual Long Reset(void) OVERRIDE; // okay return RETV_OKAY, else RETV_FAIL
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) OVERRIDE;
	virtual bool Valid(void) OVERRIDE;
private:
	CSyncEvent(const CSyncEvent&);
	CSyncEvent& operator=(const CSyncEvent&);
private:
	CHandle   m_hEvent;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncSemaphore
class CSyncSemaphore : public CTSyncBase<CSyncTraits::SYNCT_SEMAPHORE>
{
public:
	CSyncSemaphore(bool bCreateSignal = false, Long lLockCount = 1, PCXStr pszName = nullptr);
	virtual ~CSyncSemaphore(void); 

	virtual Long Signal(void) OVERRIDE; // signaled return RETV_OKAY, other return RETV_FAIL
	virtual Long Reset(void) OVERRIDE;
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE) OVERRIDE;
	virtual bool Valid(void) OVERRIDE;
private:
	CSyncSemaphore(const CSyncSemaphore&);
	CSyncSemaphore& operator=(const CSyncSemaphore&);
private:
	CHandle   m_hSemaphore;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRWLock
#if (NTDDI_VERSION >= NTDDI_VISTA)
class CRWLock : public MObject
{
public:
	CRWLock(bool bManual = false);
	~CRWLock(void);

	bool LockRead(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE);
	void UnlockRead(void);

	bool LockWrite(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE);
	void UnlockWrite(void);
private:
	CRWLock(const CRWLock&);
	CRWLock& operator=(const CRWLock&);
private:
	bool                 m_bManual;
	SRWLOCK              m_SRWLock;
	CONDITION_VARIABLE   m_ReadCVar;
	CONDITION_VARIABLE   m_WriteCVar;
};
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
class CRWLock : public MObject
{
public:
	CRWLock(void);
	~CRWLock(void);

	bool LockRead(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE);
	void UnlockRead(void);

	bool LockWrite(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE);
	void UnlockWrite(void);
private:
	CRWLock(const CRWLock&);
	CRWLock& operator=(const CRWLock&);
private:
	UInt         m_uCount;
	CSyncEvent   m_NoneReadEvent;
	CSyncMutex   m_WriteMutex;
	CSyncLock    m_ReadLock;
};
#endif // (_WIN32_WINNT >= NTDDI_VISTA)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRWLockScope
template <bool bRead = true>
class CTRWLockScope : public MObject
{
public:
	CTRWLockScope(CRWLock& RWLock, UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE);
	~CTRWLockScope(void); 
private:
	CTRWLockScope(const CTRWLockScope&);
	CTRWLockScope& operator=(const CTRWLockScope&);
private:
	CRWLock*   m_pRWLock;
};

typedef CTRWLockScope<true>  CRWLockReadScope;
typedef CTRWLockScope<false> CRWLockWriteScope;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "targetsync.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_SYNC_H__