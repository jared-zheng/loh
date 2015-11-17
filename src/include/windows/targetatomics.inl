///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetatomics.inl                                                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows atomics                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_ATOMICS_INL__
#define __TARGET_ATOMICS_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAtomics
template <typename T>
FORCEINLINE T CAtomics::Increment(T volatile* ptValue)
{
	return (T)::InterlockedIncrement(ptValue);
}

template <typename T>
FORCEINLINE T CAtomics::Increment64(T volatile* ptValue)
{
#if (NTDDI_VERSION >= NTDDI_VISTA)
	return (T)::InterlockedIncrement64(ptValue);
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
	#pragma message("Windows XP No Implement")
	return (T)(*ptValue += 1);
#endif // (_WIN32_WINNT >= NTDDI_VISTA)
}

template <typename T>
FORCEINLINE T CAtomics::Decrement(T volatile* ptValue)
{
	return (T)::InterlockedDecrement(ptValue);
}

template <typename T>
FORCEINLINE T CAtomics::Decrement64(T volatile* ptValue)
{
#if (NTDDI_VERSION >= NTDDI_VISTA)
	return (T)::InterlockedDecrement64(ptValue);
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
	#pragma message("Windows XP No Implement")
	return (T)(*ptValue -= 1);
#endif // (_WIN32_WINNT >= NTDDI_VISTA)
}

template <typename T>
FORCEINLINE T CAtomics::Add(T volatile* ptValue, T tAdd)
{
	return (T)::InterlockedExchangeAdd(ptValue, tAdd);
}

template <typename T>
FORCEINLINE T CAtomics::Add64(T volatile* ptValue, T tAdd)
{
#if (NTDDI_VERSION >= NTDDI_VISTA)
	return (T)::InterlockedExchangeAdd64(ptValue, tAdd);
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
	#pragma message("Windows XP No Implement")
	T tOldValue = *ptValue;
	*ptValue   += tAdd;
	return tOldValue;
#endif // (_WIN32_WINNT >= NTDDI_VISTA)
}

template <typename T>
FORCEINLINE T CAtomics::Exchange(T volatile* ptValue, T tExchange)
{
	return (T)::InterlockedExchange(ptValue, tExchange);
}

template <typename T>
FORCEINLINE T CAtomics::Exchange64(T volatile* ptValue, T tExchange)
{
#if (NTDDI_VERSION >= NTDDI_VISTA)
	return (T)::InterlockedExchange64(ptValue, tExchange);
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
	#pragma message("Windows XP No Implement")
	T tOldValue = *ptValue;
	*ptValue    = tExchange;
	return tOldValue;
#endif // (_WIN32_WINNT >= NTDDI_VISTA)
}

template <typename T>
FORCEINLINE T CAtomics::CompareExchange(T volatile* ptValue, T tExchange, T tComparand)
{
	return (T)::InterlockedCompareExchange(ptValue, tExchange, tComparand);
}

template <typename T>
FORCEINLINE T CAtomics::CompareExchange64(T volatile* ptValue, T tExchange, T tComparand) // 64BITS only
{
#if (NTDDI_VERSION >= NTDDI_VISTA)
	return (T)::InterlockedCompareExchange64(ptValue, tExchange, tComparand);
#else  // (_WIN32_WINNT >= NTDDI_VISTA)
	#pragma message("Windows XP No Implement")
	T tOldValue = *ptValue;
	if (*ptValue == tComparand)
	{
		*ptValue = tExchange;
	}
	return tOldValue;
#endif // (_WIN32_WINNT >= NTDDI_VISTA)
}

FORCEINLINE void* CAtomics::ExchangePtr(void* volatile* ppDst, void* pExchange)
{
#if ((__INTRIN_TARGET__ == INTRIN_TARGET_X64) || (__INTRIN_TARGET__ == INTRIN_TARGET_IA64))
	return ::InterlockedExchangePointer(ppDst, pExchange);
#elif (__INTRIN_TARGET__ == INTRIN_TARGET_X86)
	return (void*)InterlockedExchange((PLong)(ppDst), (Long)(pExchange));
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif
}

FORCEINLINE void* CAtomics::CompareExchangePtr(void* volatile* ppDst, void* pExchange, void* pComparand)
{
	return ::InterlockedCompareExchangePointer(ppDst, pExchange, pComparand);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_ATOMICS_INL__