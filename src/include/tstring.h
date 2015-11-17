///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : tstring.h                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : string                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TSTRING_H__
#define __TSTRING_H__

#pragma once

#include "traits.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
template <typename T>
class CTStringRef;

template <typename T, size_t stFix>
class CTStringFix;

template <typename T>
class CTString;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStringRef
template <typename T = CXChar>
class CTStringRef : public T
{
public:
	typedef typename T::TChar     TChar;
	typedef typename T::PTStr     PTStr;
	typedef typename T::PCTStr    PCTStr;
	typedef typename T::CPCTStr   CPCTStr;
public:
	class CStringRefTraits
	{
	public:
		typedef const CTStringRef& INARGTYPE;
		typedef CTStringRef& OUTARGTYPE;
	public:
		static void CopyElements(CTStringRef* pDst, const CTStringRef* pSrc, size_t stElements)
		{
			for (size_t i = 0; i < stElements; ++i)
			{
				pDst[i] = pSrc[i];
			}
		}

		static void RelocateElements(CTStringRef* pDst, CTStringRef* pSrc, size_t stElements)
		{
			MM_SAFE::Memmov(pDst, stElements * sizeof(CTStringRef), pSrc, stElements * sizeof(CTStringRef));
		}

		static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return (T::Comparen(t1.m_szBuffer, t2.m_szBuffer, t1.m_stLen) == 0);
			}
			return false;
		}

		static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return T::Comparen(t1.m_szBuffer, t2.m_szBuffer, t1.m_stLen);
			}
			return (t1.m_stLen - t2.m_stLen);
		}

		static size_t HashElements(INARGTYPE t)
		{
			return (CHash::Hash(t.m_pszRef, t.m_stLen));
		}
	};
public:
	CTStringRef(PCTStr psz = nullptr);
	~CTStringRef(void);

	CTStringRef(const CTStringRef& aSrc);
	CTStringRef& operator=(const CTStringRef& aSrc);

	CTStringRef& operator=(PCTStr psz);

	TChar        operator[](size_t stIndex);
	PCTStr       operator*(void) const;

	PCTStr       GetBuffer(void) const;

	bool         IsEmpty(void) const;
	size_t       Length(bool bStream = false) const;
public:
	// Character Classification
	bool         IsDigit(size_t stIndex);
	bool         IsXDigit(size_t stIndex);
	bool         IsSpace(size_t stIndex);
	bool         IsLower(size_t stIndex);
	bool         IsUpper(size_t stIndex);

	PCTStr       FindChar(TChar ch);
	PCTStr       FindCharRev(TChar ch);
	PCTStr       FindString(PCTStr pszMatch);
	PCTStr       FindOneOf(PCTStr pszMatch);

	size_t       FindOneIndexOf(PCTStr pszMatch);
	size_t       FindOneIndexNotOf(PCTStr pszMatch);

	Int          Compare(PCTStr pszCmp);
	Int          CompareIgnore(PCTStr pszCmp);
	Int          Comparen(PCTStr pszCmp, size_t stSize);
	Int          CompareIgnoren(PCTStr pszCmp, size_t stSize);
	// Compare strings using locale-specific information
	Int          Collate(PCTStr pszCmp);
	Int          CollateIgnore(PCTStr pszCmp);
	Int          Collaten(PCTStr pszCmp, size_t stSize);
	Int          CollateIgnoren(PCTStr pszCmp, size_t stSize);

	Int          ToInt(void) const;
	Long         ToLong(Int nRadix = RADIXT_DEC) const;
	ULong        ToULong(Int nRadix = RADIXT_DEC) const;
	LLong        ToLLong(Int nRadix = RADIXT_DEC) const;
	ULLong       ToULLong(Int nRadix = RADIXT_DEC) const;
	Double       ToDouble(void) const;
	// stIndex set to -1 if can not find end, else return end index
	Long         ToLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULong        ToULong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	LLong        ToLLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULLong       ToULLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	Double       ToDouble(size_t& stIndex);
private:
	size_t       m_stLen;
	PCTStr       m_pszRef;
};
typedef CTStringRef<CChar>  CCStringRef;
typedef CTStringRef<CWChar> CWStringRef;
typedef CTStringRef<CXChar> CStringRef;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStringFix
template <typename T = CXChar, size_t stFix = (size_t)LMT_KEY>
class CTStringFix : public T
{
public:
	typedef typename T::TChar     TChar;
	typedef typename T::PTStr     PTStr;
	typedef typename T::PCTStr    PCTStr;
	typedef typename T::CPCTStr   CPCTStr;
public:
	class CStringFixTraits
	{
	public:
		typedef const CTStringFix& INARGTYPE;
		typedef CTStringFix& OUTARGTYPE;
	public:
		static void CopyElements(CTStringFix* pDst, const CTStringFix* pSrc, size_t stElements)
		{
			for (size_t i = 0; i < stElements; ++i)
			{
				pDst[i] = pSrc[i];
			}
		}

		static void RelocateElements(CTStringFix* pDst, CTStringFix* pSrc, size_t stElements)
		{
			MM_SAFE::Memmov(pDst, stElements * sizeof(CTStringFix), pSrc, stElements * sizeof(CTStringFix));
		}

		static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return (T::Comparen(t1.m_szBuffer, t2.m_szBuffer, t1.m_stLen) == 0);
			}
			return false;
		}

		static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return T::Comparen(t1.m_szBuffer, t2.m_szBuffer, t1.m_stLen);
			}
			return (t1.m_stLen - t2.m_stLen);
		}

		static size_t HashElements(INARGTYPE t)
		{
			return (CHash::Hash(t.m_szBuffer, t.m_stLen));
		}
	};
public:
	CTStringFix(PCTStr psz = nullptr, size_t stLen = 0);
	~CTStringFix(void);
	CTStringFix(const CTStringFix& aSrc);
	template <size_t stLenT> CTStringFix(const CTStringFix<T, stLenT>& aSrc);
	CTStringFix(const CTStringRef<T>& aSrc);
	CTStringFix(const CTString<T>& aSrc);

	CTStringFix& operator=(PCTStr psz);
	CTStringFix& operator=(const CTStringFix& aSrc);
	template <size_t stLenT> CTStringFix& operator=(const CTStringFix<T, stLenT>& aSrc);
	CTStringFix& operator=(const CTStringRef<T>& aSrc);
	CTStringFix& operator=(const CTString<T>& aSrc);

	CTStringFix& operator+=(TChar ch);
	CTStringFix& operator+=(PCTStr psz);
	CTStringFix& operator+=(const CTStringFix& aSrc);
	template <size_t stLenT> CTStringFix& operator+=(const CTStringFix<T, stLenT>& aSrc);
	CTStringFix& operator+=(const CTStringRef<T>& aSrc);
	CTStringFix& operator+=(const CTString<T>& aSrc);

	bool         operator<=(PCTStr psz) const;
	bool         operator<(PCTStr psz) const;
	bool         operator>=(PCTStr psz) const;
	bool         operator>(PCTStr psz) const;
	bool         operator==(PCTStr psz) const;
	bool         operator!=(PCTStr psz) const;

	bool         operator<=(const CTStringFix& aSrc) const;
	bool         operator<(const CTStringFix& aSrc) const;
	bool         operator>=(const CTStringFix& aSrc) const;
	bool         operator>(const CTStringFix& aSrc) const;
	bool         operator==(const CTStringFix& aSrc) const;
	bool         operator!=(const CTStringFix& aSrc) const;

	template <size_t stLenT> bool operator<=(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator<(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator>=(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator>(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator==(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator!=(const CTStringFix<T, stLenT>& aSrc) const;

	bool         operator<=(const CTStringRef<T>& aSrc) const;
	bool         operator<(const CTStringRef<T>& aSrc) const;
	bool         operator>=(const CTStringRef<T>& aSrc) const;
	bool         operator>(const CTStringRef<T>& aSrc) const;
	bool         operator==(const CTStringRef<T>& aSrc) const;
	bool         operator!=(const CTStringRef<T>& aSrc) const;

	bool         operator<=(const CTString<T>& aSrc) const;
	bool         operator<(const CTString<T>& aSrc) const;
	bool         operator>=(const CTString<T>& aSrc) const;
	bool         operator>(const CTString<T>& aSrc) const;
	bool         operator==(const CTString<T>& aSrc) const;
	bool         operator!=(const CTString<T>& aSrc) const;

	TChar        operator[](size_t stIndex);
	PTStr        operator*(void); 
	PCTStr       operator*(void) const;

	PCTStr       GetBuffer(void) const;
	TChar        GetAt(size_t stIndex);
	bool         SetAt(size_t stIndex, TChar ch);

	bool         FillBuffer(PCTStr psz, size_t stLen = 0);
	bool         AppendBuffer(PCTStr psz, size_t stLen = 0);
	bool         AppendBuffer(TChar ch);

	bool         IsEmpty(void) const;
	void         Empty(void);
	size_t       ResetLength(size_t stLen = 0);
	size_t       Length(bool bStream = false) const;
	size_t       BufferLength(void) const;

	Int          Find(TChar ch, size_t stStart = 0, bool bRev = false) const;
	Int          Find(PCTStr pszSub, size_t stStart = 0) const;
	Int          FindOneOf(PCTStr pszMatch, size_t stStart = 0, bool bInMatch = true) const;
	Int          Compare(PCTStr psz) const;
	Int          CompareIgnore(PCTStr psz) const;
	Int          Comparen(PCTStr psz, size_t stLen = 0) const;
	Int          CompareIgnoren(PCTStr psz, size_t stLen = 0) const;
	// Compare strings using locale-specific information
	Int          Collate(PCTStr pszCmp) const;
	Int          CollateIgnore(PCTStr pszCmp) const;
	Int          Collaten(PCTStr pszCmp, size_t stLen = 0) const;
	Int          CollateIgnoren(PCTStr pszCmp, size_t stLen = 0) const;

	size_t       Delete(size_t stIndex, size_t stCount = 1);
	size_t       Insert(size_t stIndex, TChar ch);
	size_t       Insert(size_t stIndex, PCTStr psz);
	size_t       Replace(TChar chOld, TChar chNew);
	size_t       Replace(PCTStr pszOld, PCTStr pszNew);
	size_t       Remove(TChar ch);

	size_t       Load(UInt uId, LibRef lrRef = nullptr);
	size_t       Format(PCTStr pszFormat, ...);
	size_t       AppendFormat(PCTStr pszFormat, ...);

	void         Upper(size_t stIndex);
	void         Lower(size_t stIndex);
	void         Upper(void);
	void         Lower(void);
	void	     Reverse(void);
	void	     TrimLeft(TChar ch = T::SpaceChar);
	void	     TrimRight(TChar ch = T::SpaceChar);
	void         Trim(TChar ch = T::SpaceChar);

	CTStringFix  Left(size_t stCount) const;
	CTStringFix  Right(size_t stCount) const;
	CTStringFix  Mid(size_t stStart, size_t stCount) const;
public:
	// Character Classification
	bool         IsDigit(size_t stIndex);
	bool         IsXDigit(size_t stIndex);
	bool         IsSpace(size_t stIndex);
	bool         IsLower(size_t stIndex);
	bool         IsUpper(size_t stIndex);

	Int          ToInt(void) const;
	Long         ToLong(Int nRadix = RADIXT_DEC) const;
	ULong        ToULong(Int nRadix = RADIXT_DEC) const;
	LLong        ToLLong(Int nRadix = RADIXT_DEC) const;
	ULLong       ToULLong(Int nRadix = RADIXT_DEC) const;
	Double       ToDouble(void) const;
	// stIndex set to -1 if can not find end, else return end index
	Long         ToLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULong        ToULong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	LLong        ToLLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULLong       ToULLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	Double       ToDouble(size_t& stIndex);

	bool         ToString(Int nValue, Int nRadix = RADIXT_DEC);
	bool         ToString(Long lValue, Int nRadix = RADIXT_DEC);
	bool         ToString(ULong ulValue, Int nRadix = RADIXT_DEC);
	bool         ToString(LLong llValue, Int nRadix = RADIXT_DEC);
	bool         ToString(ULLong ullValue, Int nRadix = RADIXT_DEC);
	bool         ToString(Double dValue, Int nRadix = RADIXT_DIGITS);
private:
	size_t       m_stLen;
	TChar        m_szBuffer[stFix];
};
typedef CTStringFix<CChar, (size_t)LMT_KEY>       CCStringKey;
typedef CTStringFix<CChar, (size_t)LMT_MAX_PATH>  CCStringFix;
typedef CTStringFix<CWChar, (size_t)LMT_KEY>      CWStringKey;
typedef CTStringFix<CWChar, (size_t)LMT_MAX_PATH> CWStringFix;
typedef CTStringFix<CXChar, (size_t)LMT_KEY>      CStringKey;
typedef CTStringFix<CXChar, (size_t)LMT_MAX_PATH> CStringFix;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTString
template <typename T = CXChar>
class CTString : public T
{
public:
	typedef typename T::TChar     TChar;
	typedef typename T::PTStr     PTStr;
	typedef typename T::PCTStr    PCTStr;
	typedef typename T::CPCTStr   CPCTStr;
public:
	class CStringTraits
	{
	public:
		typedef const CTString& INARGTYPE;
		typedef CTString& OUTARGTYPE;
	public:
		static void CopyElements(CTString* pDst, const CTString* pSrc, size_t stElements)
		{
			for (size_t i = 0; i < stElements; ++i)
			{
				pDst[i] = pSrc[i];
			}
		}

		static void RelocateElements(CTString* pDst, CTString* pSrc, size_t stElements)
		{
			MM_SAFE::Memmov(pDst, stElements * sizeof(CTString), pSrc, stElements * sizeof(CTString));
		}

		static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return (T::Comparen(t1.m_pszBuffer, t2.m_pszBuffer, t1.m_stLen) == 0);
			}
			return false;
		}

		static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return T::Comparen(t1.m_pszBuffer, t2.m_pszBuffer, t1.m_stLen);
			}
			return (t1.m_stLen - t2.m_stLen);
		}

		static size_t HashElements(INARGTYPE t)
		{
			return (CHash::Hash(t.m_pszBuffer, t.m_stLen));
		}
	};
public:
	CTString(void);
	~CTString(void);
	CTString(PCTStr psz, size_t stLen = 0);
	CTString(const CTString& aSrc);
	CTString(const CTStringRef<T>& aSrc);
	template <size_t stLenT> CTString(const CTStringFix<T, stLenT>& aSrc);

#ifdef __CXX11_IMP__
	CTString(CTString&& aSrc);
#endif

	CTString& operator=(PCTStr psz);
	CTString& operator=(const CTString& aSrc);
	CTString& operator=(const CTStringRef<T>& aSrc);
	template <size_t stLenT> CTString& operator=(const CTStringFix<T, stLenT>& aSrc);

	CTString& operator+=(TChar ch);
	CTString& operator+=(PCTStr psz);
	CTString& operator+=(const CTString& aSrc);
	CTString& operator+=(const CTStringRef<T>& aSrc);
	template <size_t stLenT> CTString& operator+=(const CTStringFix<T, stLenT>& aSrc);

	CTString  operator+(TChar ch);
	CTString  operator+(PCTStr psz);
	CTString  operator+(const CTString& aSrc);
	CTString  operator+(const CTStringRef<T>& aSrc);

	bool      operator<=(PCTStr psz) const;
	bool      operator<(PCTStr psz) const;
	bool      operator>=(PCTStr psz) const;
	bool      operator>(PCTStr psz) const;
	bool      operator==(PCTStr psz) const;
	bool      operator!=(PCTStr psz) const;

	bool      operator<=(const CTString& aSrc) const;
	bool      operator<(const CTString& aSrc) const;
	bool      operator>=(const CTString& aSrc) const;
	bool      operator>(const CTString& aSrc) const;
	bool      operator==(const CTString& aSrc) const;
	bool      operator!=(const CTString& aSrc) const;

	bool      operator<=(const CTStringRef<T>& aSrc) const;
	bool      operator<(const CTStringRef<T>& aSrc) const;
	bool      operator>=(const CTStringRef<T>& aSrc) const;
	bool      operator>(const CTStringRef<T>& aSrc) const;
	bool      operator==(const CTStringRef<T>& aSrc) const;
	bool      operator!=(const CTStringRef<T>& aSrc) const;

	template <size_t stLenT> bool operator<=(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator<(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator>=(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator>(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator==(const CTStringFix<T, stLenT>& aSrc) const;
	template <size_t stLenT> bool operator!=(const CTStringFix<T, stLenT>& aSrc) const;

	TChar     operator[](size_t stIndex);
	PTStr     operator*(void); 
	PCTStr    operator*(void) const; 

	PCTStr    GetBuffer(void) const;
	TChar     GetAt(size_t stIndex);
	bool      SetAt(size_t stIndex, TChar ch);

	bool      FillBuffer(PCTStr psz, size_t stLen = 0);
	bool      AppendBuffer(PCTStr psz, size_t stLen = 0);
	bool      AppendBuffer(TChar ch);
	bool      SetBufferLength(size_t stLen); // stLen include '\0'

	bool      IsEmpty(void) const;
	void      Empty(bool bRelease = false);
	size_t    ResetLength(void);
	size_t    Length(bool bStream = false) const;
	size_t    BufferLength(void) const;

	Int       Find(TChar ch, size_t stStart = 0, bool bRev = false) const;
	Int       Find(PCTStr pszSub, size_t stStart = 0) const;
	Int       FindOneOf(PCTStr pszMatch, size_t stStart = 0, bool bInMatch = true) const;
	Int       Compare(PCTStr psz) const;
	Int       CompareIgnore(PCTStr psz) const;
	Int       Comparen(PCTStr psz, size_t stLen = 0) const;
	Int       CompareIgnoren(PCTStr psz, size_t stLen = 0) const;
	// Compare strings using locale-specific information
	Int       Collate(PCTStr pszCmp) const;
	Int       CollateIgnore(PCTStr pszCmp) const;
	Int       Collaten(PCTStr pszCmp, size_t stLen = 0) const;
	Int       CollateIgnoren(PCTStr pszCmp, size_t stLen = 0) const;

	size_t    Delete(size_t stIndex, size_t stCount = 1);
	size_t    Insert(size_t stIndex, TChar ch);
	size_t    Insert(size_t stIndex, PCTStr psz);
	size_t    Replace(TChar chOld, TChar chNew);
	size_t    Replace(PCTStr pszOld, PCTStr pszNew);
	size_t    Remove(TChar ch);
	void      Shrink(void);

	size_t    Load(UInt uId, LibRef lrRef = nullptr);
	size_t    Format(PCTStr pszFormat, ...);
	size_t    AppendFormat(PCTStr pszFormat, ...);

	void      Upper(size_t stIndex);//
	void      Lower(size_t stIndex);//
	void	  Upper(void);
	void	  Lower(void);
	void	  Reverse(void);
	void	  TrimLeft(TChar ch = T::SpaceChar);
	void	  TrimRight(TChar ch = T::SpaceChar);
	void      Trim(TChar ch = T::SpaceChar);

	CTString  Left(size_t stCount) const;
	CTString  Right(size_t stCount) const;
	CTString  Mid(size_t stStart, size_t stCount) const;
public:
	// Character Classification
	bool      IsDigit(size_t stIndex);
	bool      IsXDigit(size_t stIndex);
	bool      IsSpace(size_t stIndex);
	bool      IsLower(size_t stIndex);
	bool      IsUpper(size_t stIndex);

	Int       ToInt(void) const;
	Long      ToLong(Int nRadix = RADIXT_DEC) const;
	ULong     ToULong(Int nRadix = RADIXT_DEC) const;
	LLong     ToLLong(Int nRadix = RADIXT_DEC) const;
	ULLong    ToULLong(Int nRadix = RADIXT_DEC) const;
	Double    ToDouble(void) const;
	// stIndex set to -1 if can not find end, else return end index
	Long      ToLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULong     ToULong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	LLong     ToLLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULLong    ToULLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	Double    ToDouble(size_t& stIndex);

	bool      ToString(Int nValue, Int nRadix = RADIXT_DEC);
	bool      ToString(Long lValue, Int nRadix = RADIXT_DEC);
	bool      ToString(ULong ulValue, Int nRadix = RADIXT_DEC);
	bool      ToString(LLong llValue, Int nRadix = RADIXT_DEC);
	bool      ToString(ULLong ullValue, Int nRadix = RADIXT_DEC);
	bool      ToString(Double dValue, Int nRadix = RADIXT_DIGITS);
private:
	bool      AllocBuffer(size_t stBufferLen);
	bool      AllocBuffer(size_t stBufferLen, PTStr& pszBuffer);
private:
	size_t    m_stLen;
	size_t    m_stBufferLen;
	PTStr     m_pszBuffer;
};
typedef CTString<CChar>  CCString;
typedef CTString<CWChar> CWString;
typedef CTString<CXChar> CString;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "tstring.inl"

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targettstring.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targettstring.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targettstring.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targettstring.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targettstring.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targettstring.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __TSTRING_H__