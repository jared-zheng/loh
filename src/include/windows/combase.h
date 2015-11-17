///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : combase.h                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows com base                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __COM_BASE_H__
#define __COM_BASE_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "traits.h"
#include <Unknwn.h>

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComPtr
template <typename T>
class CComPtr : public MObject
{
public:
	CComPtr(void);
	~CComPtr(void);

	CComPtr(T* pT, bool bAddRef = true);
	CComPtr(const CComPtr& aSrc);
	CComPtr& operator=(const CComPtr& aSrc);
	CComPtr& operator=(T* pT);

	operator T*(void)   const;
	T& operator*(void)  const;
	T* operator->(void) const;
	T* Get(void)        const;

	bool operator==(T* pT) const;
	bool operator!=(T* pT) const;
	bool operator==(const CComPtr& aSrc) const;
	bool operator!=(const CComPtr& aSrc) const;

	void Attach(T* pT);
	T* Detach(void);

	template <typename X>
	Long QueryInterface(X** ppX) const;
	void Release(void);
protected:
	T*   m_pPtr;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComQIPtr
template <typename T, const PUID puid = &__uuidof(T)>
class CComQIPtr : public CComPtr<T>
{
public:
	CComQIPtr(void);
	~CComQIPtr(void);

	CComQIPtr(T* pT, bool bAddRef = true);
	CComQIPtr(IUnknown* pUnk);
	CComQIPtr(const CComQIPtr& aSrc);

	CComQIPtr& operator=(const CComQIPtr& aSrc);
	CComQIPtr& operator=(T* pT);
	CComQIPtr& operator=(IUnknown* pUnk);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComStr
class CComStr : public CWChar
{
public:
	class CComStrTraits
	{
	public:
		typedef const CComStr& INARGTYPE;
		typedef CComStr& OUTARGTYPE;
	public:
		static void CopyElements(CComStr* pDst, const CComStr* pSrc, size_t stElements)
		{
			for (size_t i = 0; i < stElements; ++i)
			{
				pDst[i] = pSrc[i];
			}
		}

		static void RelocateElements(CComStr* pDst, CComStr* pSrc, size_t stElements)
		{
			MM_SAFE::Memmov(pDst, stElements * sizeof(CComStr), pSrc, stElements * sizeof(CComStr));
		}

		static bool CompareElements(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return (CWChar::Comparen(t1.m_pszBuffer, t2.m_pszBuffer, t1.m_stLen) == 0);
			}
			return false;
		}

		static Int CompareElementsOrdered(INARGTYPE t1, INARGTYPE t2)
		{
			if (t1.m_stLen == t2.m_stLen)
			{
				return CWChar::Comparen(t1.m_pszBuffer, t2.m_pszBuffer, t1.m_stLen);
			}
			return (t1.m_stLen - t2.m_stLen);
		}

		static size_t HashElements(INARGTYPE t)
		{
			return (CHash::Hash(t.m_pszBuffer, t.m_stLen));
		}
	};
public:
	CComStr(void);
	~CComStr(void);
	CComStr(PCTStr psz, size_t stLen = 0);
	CComStr(const CComStr& aSrc);

#ifdef __CXX11_IMP__
	CComStr(CComStr&& aSrc);
#endif

	CComStr& operator=(PCTStr psz);
	CComStr& operator=(const CComStr& aSrc);

	CComStr& operator+=(TChar ch);
	CComStr& operator+=(PCTStr psz);
	CComStr& operator+=(const CComStr& aSrc);

	CComStr  operator+(TChar ch);
	CComStr  operator+(PCTStr psz);
	CComStr  operator+(const CComStr& aSrc);

	bool     operator<=(PCTStr psz) const;
	bool     operator<(PCTStr psz) const;
	bool     operator>=(PCTStr psz) const;
	bool     operator>(PCTStr psz) const;
	bool     operator==(PCTStr psz) const;
	bool     operator!=(PCTStr psz) const;

	bool     operator<=(const CComStr& aSrc) const;
	bool     operator<(const CComStr& aSrc) const;
	bool     operator>=(const CComStr& aSrc) const;
	bool     operator>(const CComStr& aSrc) const;
	bool     operator==(const CComStr& aSrc) const;
	bool     operator!=(const CComStr& aSrc) const;

	TChar    operator[](size_t stIndex);
	PTStr    operator*(void); 
	PCTStr   operator*(void) const; 

	void     Attach(PCTStr psz);
	PTStr    Detach(void);
	PTStr    Copy(void) const;
	Long     CopyTo(PTStr& pszRef) const;
	Long     CopyTo(VARIANT* pVar) const;

	PCTStr   GetBuffer(void) const;
	TChar    GetAt(size_t stIndex);
	bool     SetAt(size_t stIndex, TChar ch);

	bool     FillBuffer(PCTStr psz, size_t stLen = 0);
	bool     AppendBuffer(PCTStr psz, size_t stLen = 0);
	bool     AppendBuffer(TChar ch);
	bool     SetBufferLength(size_t stLen); // stLen include '\0'

	bool     IsEmpty(void) const;
	void     Empty(bool bRelease = false);
	size_t   ResetLength(void);
	size_t   Length(bool bStream = false) const;
	size_t   BufferLength(void) const;

	Int      Find(TChar ch, size_t stStart = 0, bool bRev = false) const;
	Int      Find(PCTStr pszSub, size_t stStart = 0) const;
	Int      FindOneOf(PCTStr pszMatch, size_t stStart = 0, bool bInMatch = true) const;
	Int      Compare(PCTStr psz) const;
	Int      CompareIgnore(PCTStr psz) const;
	Int      Comparen(PCTStr psz, size_t stLen = 0) const;
	Int      CompareIgnoren(PCTStr psz, size_t stLen = 0) const;
	// Compare strings using locale-specific information
	Int      Collate(PCTStr pszCmp) const;
	Int      CollateIgnore(PCTStr pszCmp) const;
	Int      Collaten(PCTStr pszCmp, size_t stLen = 0) const;
	Int      CollateIgnoren(PCTStr pszCmp, size_t stLen = 0) const;

	size_t   Delete(size_t stIndex, size_t stCount = 1);
	size_t   Insert(size_t stIndex, TChar ch);
	size_t   Insert(size_t stIndex, PCTStr psz);
	size_t   Replace(TChar chOld, TChar chNew);
	size_t   Replace(PCTStr pszOld, PCTStr pszNew);
	size_t   Remove(TChar ch);
	void     Shrink(void);

	size_t   Load(UInt uId, LibRef lrRef = nullptr);
	size_t   Format(PCTStr pszFormat, ...);
	size_t   AppendFormat(PCTStr pszFormat, ...);

	void     Upper(size_t stIndex);//
	void     Lower(size_t stIndex);//
	void	 Upper(void);
	void	 Lower(void);
	void	 Reverse(void);
	void	 TrimLeft(TChar ch = T::SpaceChar);
	void	 TrimRight(TChar ch = T::SpaceChar);
	void     Trim(TChar ch = T::SpaceChar);

	CComStr  Left(size_t stCount) const;
	CComStr  Right(size_t stCount) const;
	CComStr  Mid(size_t stStart, size_t stCount) const;
public:
	// Character Classification
	bool     IsDigit(size_t stIndex);
	bool     IsXDigit(size_t stIndex);
	bool     IsSpace(size_t stIndex);
	bool     IsLower(size_t stIndex);
	bool     IsUpper(size_t stIndex);

	Int      ToInt(void) const;
	Long     ToLong(Int nRadix = RADIXT_DEC) const;
	ULong    ToULong(Int nRadix = RADIXT_DEC) const;
	LLong    ToLLong(Int nRadix = RADIXT_DEC) const;
	ULLong   ToULLong(Int nRadix = RADIXT_DEC) const;
	Double   ToDouble(void) const;
	// stIndex set to -1 if can not find end, else return end index
	Long     ToLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULong    ToULong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	LLong    ToLLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	ULLong   ToULLong(size_t& stIndex, Int nRadix = RADIXT_DEC);
	Double   ToDouble(size_t& stIndex);

	bool     ToString(Int nValue, Int nRadix = RADIXT_DEC);
	bool     ToString(Long lValue, Int nRadix = RADIXT_DEC);
	bool     ToString(ULong ulValue, Int nRadix = RADIXT_DEC);
	bool     ToString(LLong llValue, Int nRadix = RADIXT_DEC);
	bool     ToString(ULLong ullValue, Int nRadix = RADIXT_DEC);
	bool     ToString(Double dValue, Int nRadix = RADIXT_DIGITS);
private:
	bool     AllocBuffer(size_t stBufferLen);
	bool     AllocBuffer(size_t stBufferLen, PTStr& pszBuffer);
private:
	size_t   m_stLen;
	size_t   m_stBufferLen;
	PTStr    m_pszBuffer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComVar
class CComVar : public MObject, public VARIANT
{
public:
	CComVar(void);
	~CComVar(void);

	CComVar(const CComVar& aSrc);
	CComVar(const VARIANT& aSrc);
	// spec constructors
	CComVar(Char   c);            // VT_I1
	CComVar(Short  s);            // VT_I2
	CComVar(Int    n, VARTYPE vtType = VT_I4);   // VT_I4 or VT_INT
	CComVar(Long   l, VARTYPE vtType = VT_I4);   // VT_I4 or VT_ERROR
	CComVar(LLong  ll);           // VT_I8

	CComVar(UChar  uc);           // VT_UI1
	CComVar(UShort us);           // VT_UI2
	CComVar(UInt   u, VARTYPE vtType = VT_UI4);	// VT_UI4 or VT_UINT
	CComVar(ULong  ul);                          // VT_UI4
	CComVar(ULLong ull);          // VT_UI8

	CComVar(Float  f);            // VT_R4
	CComVar(Double d, VARTYPE vtType = VT_R8);   // VT_R8 or VT_DATE

	CComVar(bool   b);            // VT_BOOL
	CComVar(CY     cy);           // VT_CY

	CComVar(PCCStr pszC);         // VT_BSTR
	CComVar(PCWStr pszW);         // VT_BSTR
	CComVar(const  CComStr& str); // VT_BSTR

	CComVar(IDispatch* p);        // VT_DISPATCH
	CComVar(IUnknown*  p);        //	VT_UNKNOWN
	CComVar(const SAFEARRAY* p);  //	VT_ARRAY
	//
	CComVar& operator=(const CComVar& aSrc);
	CComVar& operator=(const VARIANT& aSrc);
	// spec assignment operators
	CComVar& operator=(Char    c);
	CComVar& operator=(Short   s);
	CComVar& operator=(PShort  ps);
	CComVar& operator=(Int     n);
	CComVar& operator=(PInt    pn);
	CComVar& operator=(Long    l);
	CComVar& operator=(PLong   pl);
	CComVar& operator=(LLong   ll);
	CComVar& operator=(PLLong  pll);

	CComVar& operator=(UChar   uc);
	CComVar& operator=(PUChar  puc);
	CComVar& operator=(UShort  us);
	CComVar& operator=(PUShort pus);
	CComVar& operator=(UInt    u);
	CComVar& operator=(PUInt   pu);
	CComVar& operator=(ULong   ul);
	CComVar& operator=(PULong  pul);
	CComVar& operator=(ULLong  ull);
	CComVar& operator=(PULLong pull);

	CComVar& operator=(Float   f);
	CComVar& operator=(PFloat  pf);
	CComVar& operator=(Double  d);
	CComVar& operator=(PDouble pd);

	CComVar& operator=(bool   b);
	CComVar& operator=(CY     cy);

	CComVar& operator=(PCCStr pszC);
	CComVar& operator=(PCWStr pszW);
	CComVar& operator=(const  CComStr& str);

	CComVar& operator=(IDispatch* p);
	CComVar& operator=(IUnknown*  p);
	CComVar& operator=(const SAFEARRAY* p);
	// comparison operators
	bool operator==(const CComVar& aSrc) const;
	bool operator!=(const CComVar& aSrc) const;
	bool operator<(const CComVar& aSrc) const;
	bool operator>(const CComVar& aSrc) const;

	bool operator==(const VARIANT& aSrc) const;
	bool operator!=(const VARIANT& aSrc) const;
	bool operator<(const VARIANT& aSrc) const;
	bool operator>(const VARIANT& aSrc) const;
public:
	Long  Attach(VARIANT* pVar);
	Long  Detach(VARIANT* pVar);

	Long  Clear(bool bZero = false);
	Long  ClearScode(void);

	Long  Copy(const VARIANT* pVar);
	void  CopyScode(const VARIANT* pVar);

	Long  ChangeType(VARTYPE vtNew, const VARIANT* pVar = nullptr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "combase.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __COM_BASE_H__