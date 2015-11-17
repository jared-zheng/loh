///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetsync.inl                                                                        //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows multiple threads synchronization                                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_SYNC_INL__
#define __TARGET_SYNC_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSyncBase
template <Int nType>
INLINE CTSyncBase<nType>::CTSyncBase(void)
{
}

template <Int nType>
INLINE CTSyncBase<nType>::~CTSyncBase(void)
{
}

template <Int nType>
INLINE CTSyncBase<nType>::CTSyncBase(const CTSyncBase<nType>&)
{
}

template <Int nType>
INLINE CTSyncBase<nType>& CTSyncBase<nType>::operator=(const CTSyncBase<nType>&)
{
	return (*this);
}

template <Int nType>
INLINE Int CTSyncBase<nType>::GetType(void)
{
	return nType;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSyncScope
template <Int nType, UInt uWait>
INLINE CTSyncScope<nType, uWait>::CTSyncScope(CTSyncBase<nType>& Sync)
: m_pSync(&Sync)
{
	assert(m_pSync != nullptr);
	if (uWait == (UInt)CSyncTraits::SYNCW_IGNORE)
	{
		m_pSync->Signal();
	}
	else
	{
		m_pSync->Wait(uWait);
	}
}

template <Int nType, UInt uWait>
INLINE CTSyncScope<nType, uWait>::~CTSyncScope(void)
{
	assert(m_pSync != nullptr);
	m_pSync->Reset();
}

template <Int nType, UInt uWait>
INLINE CTSyncScope<nType, uWait>::CTSyncScope(const CTSyncScope<nType, uWait>&)
{ 
}

template <Int nType, UInt uWait>
INLINE CTSyncScope<nType, uWait>& CTSyncScope<nType, uWait>::operator=(const CTSyncScope<nType, uWait>&)
{ 
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncCounter
INLINE CSyncCounter::CSyncCounter(Long lCounter)
: m_lCounter(lCounter)
{
}

INLINE CSyncCounter::~CSyncCounter(void)
{
}

INLINE CSyncCounter::CSyncCounter(const CSyncCounter&)
: m_lCounter(0)
{
}

INLINE CSyncCounter& CSyncCounter::operator=(const CSyncCounter&)
{
	return (*this);
}

INLINE Long CSyncCounter::Signal(void)
{
	return (Long)CAtomics::Increment(&m_lCounter);
}

INLINE Long CSyncCounter::Reset(void)
{
	return (Long)CAtomics::Decrement(&m_lCounter);
}

INLINE Long CSyncCounter::Wait(UInt)
{
	return (Long)RET_OKAY;
}

INLINE bool CSyncCounter::Valid(void)
{
	return (m_lCounter >= 0);
}

INLINE Long CSyncCounter::GetCount(void)
{
	return m_lCounter;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncLock
INLINE CSyncLock::CSyncLock(void)
: m_uWaitSpin((UInt)CSyncTraits::SYNCW_LOCK_MAX)
{
	::InitializeCriticalSectionAndSpinCount(&m_SyncLock, (ULong)CSyncTraits::SYNCW_LOCK_MAX);
}

INLINE CSyncLock::~CSyncLock(void)
{
	::DeleteCriticalSection(&m_SyncLock);
}

INLINE CSyncLock::CSyncLock(const CSyncLock&)
: m_uWaitSpin((UInt)CSyncTraits::SYNCW_LOCK_MAX)
{
}

INLINE CSyncLock& CSyncLock::operator=(const CSyncLock&)
{ 
	return (*this);
}

INLINE Long CSyncLock::Signal(void)
{
	return (Long)((::TryEnterCriticalSection(&m_SyncLock) == TRUE) ? RET_OKAY : RET_FAIL);
}

INLINE Long CSyncLock::Reset(void)
{
	::LeaveCriticalSection(&m_SyncLock);
	return (Long)RET_OKAY;
}

INLINE Long CSyncLock::Wait(UInt uWait)
{
	if (uWait > (UInt)CSyncTraits::SYNCW_LOCK_MAX)
	{
		uWait = (UInt)CSyncTraits::SYNCW_LOCK_MAX;
		if (m_uWaitSpin != uWait)
		{
			m_uWaitSpin = uWait;
			::SetCriticalSectionSpinCount(&m_SyncLock, (ULong)uWait);
		}
	}
	::EnterCriticalSection(&m_SyncLock);
	return (Long)RET_OKAY;
}

INLINE bool CSyncLock::Valid(void)
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncMutex
INLINE CSyncMutex::CSyncMutex(bool bCreateSignal, PCXStr pszName)
: m_bOwned(bCreateSignal)
{
	m_hMutex = ::CreateMutex(nullptr, (bCreateSignal ? TRUE : FALSE), pszName);
	if (m_hMutex != nullptr)
	{
		if (m_bOwned == true)
		{
			m_bOwned = (::GetLastError() != ERROR_ALREADY_EXISTS);
		}
	}
	else
	{
		m_bOwned = false;
	}
}

INLINE CSyncMutex::~CSyncMutex(void)
{
}

INLINE CSyncMutex::CSyncMutex(const CSyncMutex&)
: m_bOwned(false)
{ 
}

INLINE CSyncMutex& CSyncMutex::operator=(const CSyncMutex&)
{ 
	return (*this);
}

INLINE Long CSyncMutex::Signal(void)
{
	return (Long)(m_bOwned ? RET_OKAY : RET_FAIL);
}

INLINE Long CSyncMutex::Reset(void)
{
	if (m_bOwned)
	{
		if (::ReleaseMutex(m_hMutex.m_krHandle))
		{
			return (Long)RET_OKAY;
		}
		return (Long)RET_FAIL;
	}
	return (Long)RET_OKAY;
}

INLINE Long CSyncMutex::Wait(UInt uWait)
{
	if (m_bOwned == false)
	{
		if (m_hMutex != nullptr)
		{
			Long lRet = (Long)::WaitForSingleObject(m_hMutex.m_krHandle, (ULong)uWait);
			m_bOwned  = (lRet == WAIT_OBJECT_0);
			return lRet;
		}
		return (Long)RET_FAIL;
	}
	return (Long)RET_OKAY;
}

INLINE bool CSyncMutex::Valid(void)
{
	return (m_hMutex != nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncEvent
INLINE CSyncEvent::CSyncEvent(bool bCreateSignal, bool bManualReset, PCXStr pszName)
{
	m_hEvent = ::CreateEvent(nullptr, (bManualReset ? TRUE : FALSE), (bCreateSignal ? TRUE : FALSE), pszName);
}

INLINE CSyncEvent::~CSyncEvent(void)
{
}

INLINE CSyncEvent::CSyncEvent(const CSyncEvent&)
{ 
}

INLINE CSyncEvent& CSyncEvent::operator=(const CSyncEvent&)
{ 
	return (*this);
}

INLINE Long CSyncEvent::Signal(void)
{
	if (::SetEvent(m_hEvent.m_krHandle))
	{
		return (Long)RET_OKAY;
	}
	else
	{
		return (Long)RET_FAIL;
	}
}

INLINE Long CSyncEvent::Reset(void)
{
	if (::ResetEvent(m_hEvent.m_krHandle))
	{
		return (Long)RET_OKAY;
	}
	else
	{
		return (Long)RET_FAIL;
	}
}

INLINE Long CSyncEvent::Wait(UInt uWait)
{
	if (m_hEvent != nullptr)
	{
		return (Long)::WaitForSingleObject(m_hEvent.m_krHandle, (ULong)uWait);
	}
	return (Long)RET_FAIL;
}

INLINE bool CSyncEvent::Valid(void)
{
	return (m_hEvent != nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSyncSemaphore
INLINE CSyncSemaphore::CSyncSemaphore(bool bCreateSignal, Long lLockCount, PCXStr pszName)
{
	m_hSemaphore = ::CreateSemaphore(nullptr, (bCreateSignal ? 0 : lLockCount), lLockCount, pszName);
}

INLINE CSyncSemaphore::~CSyncSemaphore(void)
{
}

INLINE CSyncSemaphore::CSyncSemaphore(const CSyncSemaphore&)
{ 
}

INLINE CSyncSemaphore& CSyncSemaphore::operator=(const CSyncSemaphore&)
{ 
	return (*this);
}

INLINE Long CSyncSemaphore::Signal(void)
{
	if (m_hSemaphore != nullptr)
	{
		return (Long)::WaitForSingleObject(m_hSemaphore.m_krHandle, 0);
	}
	return (Long)RET_FAIL;
}

INLINE Long CSyncSemaphore::Reset(void)
{
	if (::ReleaseSemaphore(m_hSemaphore.m_krHandle, 1, nullptr))
	{
		return (Long)RET_OKAY;
	}
	else
	{
		return (Long)RET_FAIL;
	}
}

INLINE Long CSyncSemaphore::Wait(UInt uWait)
{
	if (m_hSemaphore != nullptr)
	{
		return (Long)::WaitForSingleObject(m_hSemaphore.m_krHandle, (ULong)uWait);
	}
	return (Long)RET_FAIL;
}

INLINE bool CSyncSemaphore::Valid(void)
{
	return (m_hSemaphore != nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRWLock
#if (NTDDI_VERSION >= NTDDI_VISTA)
INLINE CRWLock::CRWLock(bool bManual)
: m_bManual(bManual)
{
	::InitializeSRWLock(&m_SRWLock);
	::InitializeConditionVariable(&m_ReadCVar);
	::InitializeConditionVariable(&m_WriteCVar);
}

INLINE CRWLock::~CRWLock(void)
{
}

INLINE CRWLock::CRWLock(const CRWLock&)
{ 
}

INLINE CRWLock& CRWLock::operator=(const CRWLock&)
{ 
	return (*this);
}

INLINE bool CRWLock::LockRead(UInt uWait)
{
#if (NTDDI_VERSION >= NTDDI_WIN7)
	if (::TryAcquireSRWLockShared(&m_SRWLock) != FALSE)
	{
		return true;
	}
#endif // (_WIN32_WINNT >= NTDDI_WIN7)
	if ((uWait != (UInt)CSyncTraits::SYNCW_INFINITE) &&
		(::SleepConditionVariableSRW(&m_ReadCVar, &m_SRWLock, (ULong)uWait, CONDITION_VARIABLE_LOCKMODE_SHARED) == FALSE))
	{
		return false;
	}
	::AcquireSRWLockShared(&m_SRWLock);
	return true;
}

INLINE void CRWLock::UnlockRead(void)
{
	::ReleaseSRWLockShared(&m_SRWLock);
	if (m_bManual)
	{
		::WakeAllConditionVariable(&m_ReadCVar);
	}
	else
	{
		::WakeConditionVariable(&m_ReadCVar);
	}
}

INLINE bool CRWLock::LockWrite(UInt uWait)
{
#if (NTDDI_VERSION >= NTDDI_WIN7)
	if (::TryAcquireSRWLockExclusive(&m_SRWLock) != FALSE)
	{
		return true;
	}
#endif // (_WIN32_WINNT >= NTDDI_WIN7)
	if ((uWait != (UInt)CSyncTraits::SYNCW_INFINITE) &&
		(::SleepConditionVariableSRW(&m_WriteCVar, &m_SRWLock, (ULong)uWait, 0) == FALSE))
	{
		return false;
	}
	::AcquireSRWLockExclusive(&m_SRWLock);
	return true;
}

INLINE void CRWLock::UnlockWrite(void)
{
	::ReleaseSRWLockExclusive(&m_SRWLock);
	if (m_bManual)
	{
		::WakeAllConditionVariable(&m_WriteCVar);
	}
	else
	{
		::WakeConditionVariable(&m_WriteCVar);
	}
}
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
INLINE CRWLock::CRWLock(void)
: m_uCount(0)
, m_NoneReadEvent(true, true)
{
}

INLINE CRWLock::~CRWLock(void)
{
	assert(m_uCount == 0);
}

INLINE CRWLock::CRWLock(const CRWLock&)
{ 
}

INLINE CRWLock& CRWLock::operator=(const CRWLock&)
{ 
	return (*this);
}

INLINE bool CRWLock::LockRead(UInt uWait)
{
	if (m_WriteMutex.Wait(uWait) != (Long)RET_OKAY)
	{
		return false;
	}

	m_ReadLock.Wait();
	{
		++m_uCount;
		if (m_uCount == 1)
		{
			m_NoneReadEvent.Reset();
		}
	}
	m_ReadLock.Reset();

	m_WriteMutex.Reset();
	return true;
}

INLINE void CRWLock::UnlockRead(void)
{
	m_ReadLock.Wait();
	{
		--m_uCount;
		if (m_uCount == 0)
		{
			m_NoneReadEvent.Signal();
		}
	}
	m_ReadLock.Reset();
}

INLINE bool CRWLock::LockWrite(UInt uWait)
{
	if (m_WriteMutex.Wait(uWait) != (Long)RET_OKAY)
	{
		return false;
	}
	if (m_NoneReadEvent.Wait(uWait) != (Long)RET_OKAY)
	{
		m_WriteMutex.Reset();
		return false;
	}
	return true;
}

INLINE void CRWLock::UnlockWrite(void)
{
	m_WriteMutex.Reset();
}
#endif // (_WIN32_WINNT >= NTDDI_VISTA)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRWLockScope
template <bool bRead>
INLINE CTRWLockScope<bRead>::CTRWLockScope(CRWLock& RWLock, UInt uWait)
: m_pRWLock(&RWLock)
{
	assert(m_pRWLock != nullptr);
	if (bRead == true)
	{
		m_pRWLock->LockRead(uWait);
	}
	else
	{
		m_pRWLock->LockWrite(uWait);
	}
}

template <bool bRead>
INLINE CTRWLockScope<bRead>::~CTRWLockScope(void)
{
	assert(m_pRWLock != nullptr);
	if (bRead == true)
	{
		m_pRWLock->UnlockRead();
	}
	else
	{
		m_pRWLock->UnlockWrite();
	}
}

template <bool bRead>
INLINE CTRWLockScope<bRead>::CTRWLockScope(const CTRWLockScope<bRead>&)
{ 
}

template <bool bRead>
INLINE CTRWLockScope<bRead>& CTRWLockScope<bRead>::operator=(const CTRWLockScope<bRead>&)
{ 
	return (*this);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_SYNC_INL__