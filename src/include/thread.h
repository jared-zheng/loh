///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : thread.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : threading                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __THREAD_H__
#define __THREAD_H__

#pragma once

#include "sync.h"
#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CThreadTraits
// threads:
// export class             inner class                         commit
// CRunnable                CRunnableThread                              --+
// CQueueTask               CQueueTaskThread                               +- CThreadPool
// CThread                  ---                                          --+
// CRunnableThread --> CThread <-- CQueueTaskThread
class CThreadTraits
{
public:
	enum THREAD_LEVEL
	{
		THREADL_NORMAL = 0,
		THREADL_ABOVE_NORMAL,
		THREADL_BELOW_NORMAL,
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRunnable
// This is the base interface for "runnable" object. A runnable object is an
// object that is "run" on an arbitrary thread. The call usage pattern is
// OnStart(), Run(), OnStop(). The thread that is going to "run" this object always
// uses those calling semantics. It does this on the thread that is created so
// that any thread specific uses (TLS, etc.) are available in the contexts of
// those calls. A "runnable" does all initialization in OnStart(). If
// initialization fails, the thread stops execution and returns an error code.
// If it succeeds, Run() is called where the real threaded work is done. Upon
// completion, OnStop() is called to allow correct clean up.
class NOVTABLE CRunnable ABSTRACT : public MObject
{
public:
	virtual bool OnStart(void) PURE_METHOD; // before run, all per runnable inst init
	virtual bool OnStop(void) PURE_METHOD;  // before exit, release data, terminated thread do not called
	virtual bool Run(void) PURE_METHOD;     // return true continue running, false thread should exit
protected:
	CRunnable(void);
	virtual ~CRunnable(void);
private:
	CRunnable(const CRunnable&);
	CRunnable& operator=(const CRunnable&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CQueueTask
// This interface is a type of runnable object that requires no per thread
// initialization. It is meant to be used with pools of threads in an
// abstract way that prevents the pool from needing to know any details
// about the object being run. This allows queueing of disparate tasks and
// servicing those tasks with a generic thread pool.
class NOVTABLE CQueueTask ABSTRACT : public MObject
{
public:
	virtual void OnFinish(void) PURE_METHOD;
	virtual void Work(void) PURE_METHOD;
protected:
	CQueueTask(void);
	virtual ~CQueueTask(void);
private:
	CQueueTask(const CQueueTask&);
	CQueueTask& operator=(const CQueueTask&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CThread
class CORECLASS NOVTABLE CThread ABSTRACT : public MObject
{
	friend class CThreadPool;
public:
	static  UInt CurrentProcessId(void);
	static  UInt CurrentThreadId(void);
	static  UInt SleepEx(UInt uSleep, bool bAlert = false);
	static  void YieldEx(void);
	// concurrent runnable inst
	static  bool StartRunnable(CRunnable& RunnableRef);
	// queuetask-thread inst
	static  bool QueueTask(CQueueTask& QueueTaskRef);
	// 0 --- default queuetask-thread count = 1, max queuetask-thread = CPU core * 2
	static  void QueueThread(UInt uThreadCount = 0); 
public:
	virtual bool Start(bool bSuspend = false);
	virtual bool Stop(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE); 
	virtual bool Kill(UInt uExitCode = EXIT_FAILURE);
	virtual bool Suspend(bool bSuspend = false);
	virtual bool IsRunning(void);
	virtual bool IsSuspend(void);
	virtual Long Wait(UInt uWait = (UInt)CSyncTraits::SYNCW_INFINITE); 
public:
	UInt    GetId(void);
	CHandle GetHandle(void);
protected:
	CThread(void);
	virtual ~CThread(void);

	virtual bool OnStart(void);
	virtual bool OnStop(void);
	virtual bool OnKill(void);
	virtual bool Run(void) PURE_METHOD;
private:
	CThread(const CThread&);
	CThread& operator=(const CThread&);

	void    Routine(void);
protected:
	UInt      m_uId;
	bool      m_bSuspend;
	CHandle   m_hThread;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "thread.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __THREAD_H__