///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : atomics.h                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : atomics                                                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __ATOMICS_H__
#define __ATOMICS_H__

#pragma once

#include "core.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAtomics
class CAtomics
{
public:
	template <typename T> static T Increment(T volatile* ptValue);
	template <typename T> static T Increment64(T volatile* ptValue);
	template <typename T> static T Decrement(T volatile* ptValue);
	template <typename T> static T Decrement64(T volatile* ptValue);
	template <typename T> static T Add(T volatile* ptValue, T tAdd);
	template <typename T> static T Add64(T volatile* ptValue, T tAdd);
	template <typename T> static T Exchange(T volatile* ptValue, T tExchange);
	template <typename T> static T Exchange64(T volatile* ptValue, T tExchange);

	template <typename T> static T CompareExchange(T volatile* plValue, T tExchange, T tComparand);
	template <typename T> static T CompareExchange64(T volatile* pllValue, T tExchange, T tComparand);

	static void* ExchangePtr(void* volatile* ppDst, void* pExchange);
	static void* CompareExchangePtr(void* volatile* ppDst, void* pExchange, void* pComparand);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetatomics.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetatomics.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetatomics.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetatomics.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetatomics.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetatomics.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __ATOMICS_H__