///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : traits.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : basic type traits                                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TRAITS_H__
#define __TRAITS_H__

#pragma once

#include "hash.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsBase
template <typename T>
class CTElementTraitsBase
{
public:
	typedef const T& INARGTYPE;
	typedef T& OUTARGTYPE;
public:
	static void CopyElements(T* pDst, const T* pSrc, size_t stElements);
	static void RelocateElements(T* pDst, T* pSrc, size_t stElements);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsCompare
template <typename T>
class CTElementTraitsCompare
{
public:
	static bool CompareElements(const T& t1, const T& t2);
	static Int  CompareElementsOrdered(const T& t1, const T& t2);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsHash
template <typename T>
class CTElementTraitsHash
{
public:
	static size_t HashElements(const T& t);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTDefaultElementTraits
template <typename T>
class CTDefaultElementTraits : 
	public CTElementTraitsBase<T>, 
	public CTElementTraitsCompare<T>,
	public CTElementTraitsHash<T>
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraits
template <typename T>
class CTElementTraits : public CTDefaultElementTraits<T>
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTPrimitiveElementTraits
template <typename T>
class CTPrimitiveElementTraits : public CTDefaultElementTraits<T>
{
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Primitive Element Traits Define
#define _DECLARE_PRIMITIVE_TRAITS( T )           \
template<>                                       \
class CTElementTraits<T> :                       \
	public CTPrimitiveElementTraits<T>           \
	{                                            \
	};

_DECLARE_PRIMITIVE_TRAITS( Char )
_DECLARE_PRIMITIVE_TRAITS( Short )
_DECLARE_PRIMITIVE_TRAITS( Int )
_DECLARE_PRIMITIVE_TRAITS( Long )
_DECLARE_PRIMITIVE_TRAITS( LLong )

_DECLARE_PRIMITIVE_TRAITS( UChar )
_DECLARE_PRIMITIVE_TRAITS( UShort )
_DECLARE_PRIMITIVE_TRAITS( UInt )
_DECLARE_PRIMITIVE_TRAITS( ULong )
_DECLARE_PRIMITIVE_TRAITS( ULLong )

_DECLARE_PRIMITIVE_TRAITS( Float )
_DECLARE_PRIMITIVE_TRAITS( Double )

_DECLARE_PRIMITIVE_TRAITS( WChar )

_DECLARE_PRIMITIVE_TRAITS( bool )
_DECLARE_PRIMITIVE_TRAITS( void* )

//_DECLARE_PRIMITIVE_TRAITS( KeyRef ) == _DECLARE_PRIMITIVE_TRAITS( void* )
_DECLARE_PRIMITIVE_TRAITS( LibRef )
_DECLARE_PRIMITIVE_TRAITS( WndRef )

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXCharTraitsBase
template <typename T = XChar>
class CTXCharTraitsBase : public MObject
{
public:
	static T LineChar;
	static T ReturnChar;
	static T TabChar;
	static T SpaceChar;
	static T DotChar;
	static T ColonChar;
	static T SlashChar;
	static T BackSlashChar;
public:
	static Int    Convert(PCStr  pszA, Int nSizeA, PWStr pszU, Int nSizeU, size_t stPage = TRAITS_PAGE_ACP);
	static Int    Convert(PCWStr pszU, Int nSizeU, PStr  pszA, Int nSizeA, size_t stPage = TRAITS_PAGE_ACP);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChar
class CChar : public CTXCharTraitsBase<Char>
{
public:
	typedef Char     TChar;
	typedef PStr     PTStr;
	typedef PCStr    PCTStr;
	typedef CPCStr   CPCTStr;
public:
	// Character Classification
	static bool   IsAlpha(TChar ch);
	static bool   IsDigit(TChar ch);
	static bool   IsXDigit(TChar ch);
	static bool   IsSpace(TChar ch);
	static bool   IsLower(TChar ch);
	static bool   IsUpper(TChar ch);

	static PTStr  Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext);
	static PTStr  Reverse(PTStr pszStr);

	static PCTStr FindChar(PCTStr pszStr, TChar ch);
	static PCTStr FindCharRev(PCTStr pszStr, TChar ch);
	static PCTStr FindString(PCTStr pszStr, PCTStr pszMatch);
	static PCTStr FindOneOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch);
	static size_t FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t Length(PCTStr pszStr, size_t stMax = (size_t)LMT_MAX);

	static Int    Compare(PCTStr pszA, PCTStr pszB);
	static Int    CompareIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Comparen(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);
	// Compare strings using locale-specific information
	static Int    Collate(PCTStr pszA, PCTStr pszB);
	static Int    CollateIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Collaten(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);

	static Int    GetFormattedLength(PCTStr pszFormat, va_list args);
	static Int    VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args);
	static Int    Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...);

	static bool   Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount);
	static bool   Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);
	static bool   Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);

	static bool   ToLower(PTStr pszStr, size_t stSize);
	static bool   ToUpper(PTStr pszStr, size_t stSize);

	static TChar  ToLower(TChar ch);
	static TChar  ToUpper(TChar ch);

	static Int    ToInt(PCTStr pszStr);
	static Long   ToLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULong  ToULong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static LLong  ToLLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULLong ToULLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static Double ToDouble(PCTStr pszStr, PTStr* ppEnd = nullptr);

	static bool   ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DIGITS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMChar : Multi-Byte Chactacter
class CMChar : public CTXCharTraitsBase<Char>
{
public:
	typedef Char     TChar;
	typedef PStr     PTStr;
	typedef PCStr    PCTStr;
	typedef CPCStr   CPCTStr;
public:
	// Character Classification
	static bool   IsAlpha(TChar ch);
	static bool   IsDigit(TChar ch);
	static bool   IsXDigit(TChar ch);
	static bool   IsSpace(TChar ch);
	static bool   IsLower(TChar ch);
	static bool   IsUpper(TChar ch);

	static PTStr  Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext);
	static PTStr  Reverse(PTStr pszStr);

	static PCTStr FindChar(PCTStr pszStr, TChar ch);
	static PCTStr FindCharRev(PCTStr pszStr, TChar ch);
	static PCTStr FindString(PCTStr pszStr, PCTStr pszMatch);
	static PCTStr FindOneOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch);
	static size_t FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t Length(PCTStr pszStr, size_t stMax = (size_t)LMT_MAX);

	static Int    Compare(PCTStr pszA, PCTStr pszB);
	static Int    CompareIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Comparen(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);
	// Compare strings using locale-specific information
	static Int    Collate(PCTStr pszA, PCTStr pszB);
	static Int    CollateIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Collaten(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);

	static Int    GetFormattedLength(PCTStr pszFormat, va_list args);
	static Int    VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args);
	static Int    Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...);

	static bool   Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount);
	static bool   Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);
	static bool   Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);

	static bool   ToLower(PTStr pszStr, size_t stSize);
	static bool   ToUpper(PTStr pszStr, size_t stSize);

	static TChar  ToLower(TChar ch);
	static TChar  ToUpper(TChar ch);

	static Int    ToInt(PCTStr pszStr);
	static Long   ToLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULong  ToULong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static LLong  ToLLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULLong ToULLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static Double ToDouble(PCTStr pszStr, PTStr* ppEnd = nullptr);

	static bool   ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DIGITS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWChar
class CWChar : public CTXCharTraitsBase<WChar>
{
public:
	typedef WChar     TChar;
	typedef PWStr     PTStr;
	typedef PCWStr    PCTStr;
	typedef CPCWStr   CPCTStr;
public:
	// Character Classification
	static bool   IsAlpha(TChar ch);
	static bool   IsDigit(TChar ch);
	static bool   IsXDigit(TChar ch);
	static bool   IsSpace(TChar ch);
	static bool   IsLower(TChar ch);
	static bool   IsUpper(TChar ch);

	static PTStr  Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext);
	static PTStr  Reverse(PTStr pszStr);

	static PCTStr FindChar(PCTStr pszStr, TChar ch);
	static PCTStr FindCharRev(PCTStr pszStr, TChar ch);
	static PCTStr FindString(PCTStr pszStr, PCTStr pszMatch);
	static PCTStr FindOneOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch);
	static size_t FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t Length(PCTStr pszStr, size_t stMax = (size_t)LMT_MAX);

	static Int    Compare(PCTStr pszA, PCTStr pszB);
	static Int    CompareIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Comparen(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);
	// Compare strings using locale-specific information
	static Int    Collate(PCTStr pszA, PCTStr pszB);
	static Int    CollateIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Collaten(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);

	static Int    GetFormattedLength(PCTStr pszFormat, va_list args);
	static Int    VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args);
	static Int    Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...);

	static bool   Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount);
	static bool   Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);
	static bool   Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);

	static bool   ToLower(PTStr pszStr, size_t stSize);
	static bool   ToUpper(PTStr pszStr, size_t stSize);

	static TChar  ToLower(TChar ch);
	static TChar  ToUpper(TChar ch);

	static Int    ToInt(PCTStr pszStr);
	static Long   ToLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULong  ToULong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static LLong  ToLLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULLong ToULLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static Double ToDouble(PCTStr pszStr, PTStr* ppEnd = nullptr);

	static bool   ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DIGITS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXChar
class CXChar : public CTXCharTraitsBase<XChar>
{
public:
	typedef XChar     TChar;
	typedef PXStr     PTStr;
	typedef PCXStr    PCTStr;
	typedef CPCXStr   CPCTStr;
public:
	// Character Classification
	static bool   IsAlpha(TChar ch);
	static bool   IsDigit(TChar ch);
	static bool   IsXDigit(TChar ch);
	static bool   IsSpace(TChar ch);
	static bool   IsLower(TChar ch);
	static bool   IsUpper(TChar ch);

	static PTStr  Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext);
	static PTStr  Reverse(PTStr pszStr);

	static PCTStr FindChar(PCTStr pszStr, TChar ch);
	static PCTStr FindCharRev(PCTStr pszStr, TChar ch);
	static PCTStr FindString(PCTStr pszStr, PCTStr pszMatch);
	static PCTStr FindOneOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch);
	static size_t FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch);

	static size_t Length(PCTStr pszStr, size_t stMax = (size_t)LMT_MAX);

	static Int    Compare(PCTStr pszA, PCTStr pszB);
	static Int    CompareIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Comparen(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);
	// Compare strings using locale-specific information
	static Int    Collate(PCTStr pszA, PCTStr pszB);
	static Int    CollateIgnore(PCTStr pszA, PCTStr pszB);
	static Int    Collaten(PCTStr pszA, PCTStr pszB, size_t stSize);
	static Int    CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize);

	static Int    GetFormattedLength(PCTStr pszFormat, va_list args);
	static Int    VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args);
	static Int    Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...);

	static bool   Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount);
	static bool   Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);
	static bool   Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc = _TRUNCATE);

	static bool   ToLower(PTStr pszStr, size_t stSize);
	static bool   ToUpper(PTStr pszStr, size_t stSize);

	static TChar  ToLower(TChar ch);
	static TChar  ToUpper(TChar ch);

	static Int    ToInt(PCTStr pszStr);
	static Long   ToLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULong  ToULong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static LLong  ToLLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static ULLong ToULLong(PCTStr pszStr, PTStr* ppEnd = nullptr, Int nRadix = RADIXT_DEC);
	static Double ToDouble(PCTStr pszStr, PTStr* ppEnd = nullptr);

	static bool   ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DEC);
	static bool   ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix = RADIXT_DIGITS);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targettraits.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targettraits.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targettraits.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targettraits.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targettraits.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targettraits.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __TRAITS_H__