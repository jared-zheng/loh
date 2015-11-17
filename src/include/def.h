///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : def.h                                                                                 //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : basic type define file                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DEF_H__
#define __DEF_H__

#pragma once

#include "commondef.h"

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetdef.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetdef.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetdef.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetdef.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetdef.h"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetdef.h"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef   Char*                                     PStr;
typedef   const Char*                               PCStr;
typedef   const Char* const                         CPCStr;
typedef   UChar*                                    PUStr;
typedef   const UChar*                              PCUStr;
typedef   const UChar* const                        CPCUStr;
typedef   WChar*                                    PWStr;
typedef   const WChar*                              PCWStr;
typedef   const WChar* const                        CPCWStr;
typedef   XChar*                                    PXStr;
typedef   const XChar*                              PCXStr;
typedef   const XChar* const                        CPCXStr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// msic

#define CASE_TOSTR( x )                             case x: return TF(#x);
#define CASE_RET( x, desc )                         case x: return desc;
#define IFAND_TOSTR( x, v )                         if ( x & v) return TF(#v);
#define IFAND_RET( x, v, desc )                     if ( x & v) return desc;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// global INLINE functions
class DEF_BASE
{
public:
	template <typename T> static T Abs(const T t);
	template <typename T> static T Max(const T a, const T b);
	template <typename T> static T Min(const T a, const T b);
	template <typename T> static T Maxmin(const T maxa, const T maxb, const T minc);
	template <typename T> static T Align(const T t, const T align);
	template <typename T> static T Swap(T& a, T& b);

	static CPCXStr StringVarType(size_t stType);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// convertible detection
template <typename X, typename T>
struct TConvertible
{
public:
    typedef Char  YES;
    typedef Short NO;

    static  YES   Convert(T*);
    static  NO    Convert(...);
public:
    enum CONVERTIBLE
	{ 
		VALUE = sizeof( (Convert)(static_cast<X*>(nullptr)) ) == sizeof(YES) 
	};
};

struct ConvertibleTag { };

template <bool> struct TConvertibleDetectionImp;
template <>     struct TConvertibleDetectionImp<true>  { typedef ConvertibleTag TAG; };
template <>     struct TConvertibleDetectionImp<false> {                             };

template <typename X, typename T>
struct TConvertibleDetection : public TConvertibleDetectionImp<TConvertible<X, T>::VALUE> { };
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "def.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __DEF_H__