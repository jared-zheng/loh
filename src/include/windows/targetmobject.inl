///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetmobject.inl                                                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : managed memory alloc/free                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_MOBJECT_INL__
#define __TARGET_MOBJECT_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// MM_SAFE
template <typename T>
INLINE void MM_SAFE::DELETE_PTR(T* pV)
{
	if (pV != nullptr)
	{
		MDELETE pV;
	}
}

template <typename T>
INLINE void MM_SAFE::DELETE_PTRREF(T*& pV)
{
	if (pV != nullptr)
	{
		MDELETE pV;
		pV = nullptr;
	}
}

template <typename X, typename T>
INLINE void MM_SAFE::DELETE_PTR(X* pV)
{
	if (pV != nullptr)
	{
		MDELETE (static_cast<T*>(pV));
	}
}

template <typename X, typename T>
INLINE void MM_SAFE::DELETE_PTRREF(X*& pV)
{
	if (pV != nullptr)
	{
		MDELETE (static_cast<T*>(pV));
		pV = nullptr;
	}
}

INLINE Int MM_SAFE::Memcmp(const void* pDst, const void* pSrc, size_t stSize)
{
	return memcmp(pDst, pSrc, stSize);
}

INLINE void* MM_SAFE::Memset(void* pDst, Int nValue, size_t stSize)
{
	return memset(pDst, nValue, stSize);
}

INLINE void* MM_SAFE::Memcpy(void* pDst, size_t stDst, const void* pSrc, size_t stSrc)
{
	memcpy_s(pDst, stDst, pSrc, stSrc);
	return pDst;
}

INLINE void* MM_SAFE::Memmov(void* pDst, size_t stDst, const void* pSrc, size_t stSrc)
{
	memmove_s(pDst, stDst, pSrc, stSrc);
	return pDst;
}


#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_MOBJECT_INL__