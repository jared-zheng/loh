///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targettraits.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows basic type traits                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TRAITS_INL__
#define __TRAITS_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsBase
template<typename T>
INLINE void CTElementTraitsBase<T>::CopyElements(T* pDst, const T* pSrc, size_t stElements)
{
	for (size_t i = 0; i < stElements; ++i)
	{
		pDst[i] = pSrc[i];
	}
}

template<typename T>
INLINE void CTElementTraitsBase<T>::RelocateElements(T* pDst, T* pSrc, size_t stElements)
{
	// A simple memmove works for nearly all types.
	// You'll have to override this for types that have pointers to their
	// own members.
	MM_SAFE::Memmov(pDst, stElements * sizeof(T), pSrc, stElements * sizeof(T));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsCompare
template <typename T>
INLINE bool CTElementTraitsCompare<T>::CompareElements(const T& t1, const T& t2)
{
	return ((t1 == t2) != 0);  // != 0 to handle overloads of operator== that return BOOL instead of bool
}

template <typename T>
INLINE Int CTElementTraitsCompare<T>::CompareElementsOrdered(const T& t1, const T& t2)
{
	if (t1 < t2)
	{
		return (Int)(-1);
	}
	else if (t1 == t2)
	{
		return (Int)(0);
	}
	else
	{
		assert(t1 > t2);
		return (Int)(1);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTElementTraitsHash
template<typename T>
INLINE size_t CTElementTraitsHash<T>::HashElements(const T& t)
{
	return (CHash::Hash(t));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTXCharTraitsBase
template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::LineChar      = TF('\n');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::ReturnChar    = TF('\r');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::TabChar       = TF('\t');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::SpaceChar     = TF(' ');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::DotChar       = TF('.');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::ColonChar     = TF(':');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::SlashChar     = TF('/');

template <typename T> 
SELECTANY T CTXCharTraitsBase<T>::BackSlashChar = TF('\\');

template <typename T>
INLINE Int CTXCharTraitsBase<T>::Convert(PCStr pszA, Int nSizeA, PWStr pszU, Int nSizeU, size_t stPage)
{
	return ::MultiByteToWideChar((UInt)stPage, 0, pszA, nSizeA, pszU, nSizeU);
}

template <typename T>
INLINE Int CTXCharTraitsBase<T>::Convert(PCWStr pszU, Int nSizeU, PStr pszA, Int nSizeA, size_t stPage)
{
	return ::WideCharToMultiByte((UInt)stPage, 0, pszU, nSizeU, pszA, nSizeA, nullptr, nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CChar
INLINE bool CChar::IsAlpha(TChar ch)
{
	return (isalpha(ch) > 0);
}

INLINE bool CChar::IsDigit(TChar ch)
{
	return (isdigit(ch) > 0);
}

INLINE bool CChar::IsXDigit(TChar ch)
{
	return (isxdigit(ch) > 0);
}

INLINE bool CChar::IsSpace(TChar ch)
{
	return (isspace(ch) > 0);
}

INLINE bool CChar::IsLower(TChar ch)
{
	return (islower(ch) > 0);
}

INLINE bool CChar::IsUpper(TChar ch)
{
	return (isupper(ch) > 0);
}

INLINE CChar::PTStr CChar::Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext)
{
	return strtok_s(pszStr, pszDelimit, &pszContext);
}

INLINE CChar::PTStr CChar::Reverse(PTStr pszStr)
{
	return _strrev(pszStr);
}

INLINE CChar::PCTStr CChar::FindChar(PCTStr pszStr, TChar ch)
{
	return strchr(pszStr, ch);
}

INLINE CChar::PCTStr CChar::FindCharRev(PCTStr pszStr, TChar ch)
{
	return strrchr(pszStr, ch);
}

INLINE CChar::PCTStr CChar::FindString(PCTStr pszStr, PCTStr pszMatch)
{
	return strstr(pszStr, pszMatch);
}

INLINE CChar::PCTStr CChar::FindOneOf(PCTStr pszStr, PCTStr pszMatch)
{
	return strpbrk(pszStr, pszMatch);
}

INLINE size_t CChar::FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch)
{
	return strcspn(pszStr, pszMatch);
}

INLINE size_t CChar::FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch)
{
	return strspn(pszStr, pszMatch);
}

INLINE size_t CChar::Length(PCTStr pszStr, size_t stMax)
{
	return strnlen(pszStr, stMax);
}

INLINE Int CChar::Compare(PCTStr pszA, PCTStr pszB)
{
	return strcmp(pszA, pszB);
}

INLINE Int CChar::CompareIgnore(PCTStr pszA, PCTStr pszB)
{
	return _stricmp(pszA, pszB);
}

INLINE Int CChar::Comparen(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return strncmp(pszA, pszB, stSize);
}

INLINE Int CChar::CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _strnicmp(pszA, pszB, stSize);
}

INLINE Int CChar::Collate(PCTStr pszA, PCTStr pszB)
{
	return strcoll(pszA, pszB);
}

INLINE Int CChar::CollateIgnore(PCTStr pszA, PCTStr pszB)
{
	return _stricoll(pszA, pszB);
}

INLINE Int CChar::Collaten(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _strncoll(pszA, pszB, stSize);
}

INLINE Int CChar::CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _strnicoll(pszA, pszB, stSize);
}

INLINE Int CChar::GetFormattedLength(PCTStr pszFormat, va_list args)
{
	return _vscprintf(pszFormat, args);
}

INLINE Int CChar::VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args)
{
	return _vsnprintf_s(pszBuf, stSize, _TRUNCATE, pszFormat, args);
}

INLINE Int CChar::Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = VFormat(pszBuf, stSize, pszFormat, vl);
	va_end(vl);

	return nRet;
}

INLINE bool CChar::Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount)
{
	return (_strnset_s(pszStr, stSize, ch, stCount) == 0);
}

INLINE bool CChar::Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (strncpy_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CChar::Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (strncat_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CChar::ToLower(PTStr pszStr, size_t stSize)
{
	return (_strlwr_s(pszStr, stSize) == 0);
}

INLINE bool CChar::ToUpper(PTStr pszStr, size_t stSize)
{
	return (_strupr_s(pszStr, stSize) == 0);
}

INLINE CChar::TChar CChar::ToLower(TChar ch)
{
	return (TChar)tolower(ch);
}

INLINE CChar::TChar CChar::ToUpper(TChar ch)
{
	return (TChar)toupper(ch);
}

INLINE Int CChar::ToInt(PCTStr pszStr)
{
	return atoi(pszStr);
}

INLINE Long CChar::ToLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return strtol(pszStr, ppEnd, nRadix);
}

INLINE ULong CChar::ToULong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return strtoul(pszStr, ppEnd, nRadix);
}

INLINE LLong CChar::ToLLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _strtoi64(pszStr, ppEnd, nRadix);
}

INLINE ULLong CChar::ToULLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _strtoui64(pszStr, ppEnd, nRadix);
}

INLINE Double CChar::ToDouble(PCTStr pszStr, PTStr* ppEnd)
{
	return strtod(pszStr, ppEnd);
}

INLINE bool CChar::ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_itoa_s(nValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CChar::ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ltoa_s(lValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CChar::ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ultoa_s(ulValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CChar::ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_i64toa_s(llValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CChar::ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ui64toa_s(ullValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CChar::ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_gcvt_s(pszStr, stSize, dValue, nRadix) == 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMChar
INLINE bool CMChar::IsAlpha(TChar ch)
{
	return (_ismbcalpha((UInt)ch) > 0);
}

INLINE bool CMChar::IsDigit(TChar ch)
{
	return (_ismbcdigit((UInt)ch) > 0);
}

INLINE bool CMChar::IsXDigit(TChar ch)
{
	return (isxdigit(ch) > 0);
}

INLINE bool CMChar::IsSpace(TChar ch)
{
	return (_ismbcspace((UInt)ch) > 0);
}

INLINE bool CMChar::IsLower(TChar ch)
{
	return (_ismbclower((UInt)ch) > 0);
}

INLINE bool CMChar::IsUpper(TChar ch)
{
	return (_ismbcupper((UInt)ch) > 0);
}

INLINE CMChar::PTStr CMChar::Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext)
{
	return (PTStr)_mbstok_s((PUStr)pszStr, (PCUStr)pszDelimit, (PUStr*)&pszContext);
}

INLINE CMChar::PTStr CMChar::Reverse(PTStr pszStr)
{
	return (PTStr)_mbsrev((PUStr)pszStr);
}

INLINE CMChar::PCTStr CMChar::FindChar(PCTStr pszStr, TChar ch)
{
	return (PCTStr)_mbschr((PCUStr)pszStr, (UInt)ch);
}

INLINE CMChar::PCTStr CMChar::FindCharRev(PCTStr pszStr, TChar ch)
{
	return (PCTStr)_mbsrchr((PCUStr)pszStr, (UInt)ch);
}

INLINE CMChar::PCTStr CMChar::FindString(PCTStr pszStr, PCTStr pszMatch)
{
	return (PCTStr)_mbsstr((PCUStr)pszStr, (PCUStr)pszMatch);
}

INLINE CMChar::PCTStr CMChar::FindOneOf(PCTStr pszStr, PCTStr pszMatch)
{
	return (PCTStr)_mbspbrk((PCUStr)pszStr, (PCUStr)pszMatch);
}

INLINE size_t CMChar::FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch)
{
	return _mbscspn((PCUStr)pszStr, (PCUStr)pszMatch);
}

INLINE size_t CMChar::FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch)
{
	return _mbsspn((PCUStr)pszStr, (PCUStr)pszMatch);
}

INLINE size_t CMChar::Length(PCTStr pszStr, size_t stMax)
{
	return strnlen(pszStr, stMax);
}

INLINE Int CMChar::Compare(PCTStr pszA, PCTStr pszB)
{
	return _mbscmp((PCUStr)pszA, (PCUStr)pszB);
}

INLINE Int CMChar::CompareIgnore(PCTStr pszA, PCTStr pszB)
{
	return _mbsicmp((PCUStr)pszA, (PCUStr)pszB);
}

INLINE Int CMChar::Comparen(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _mbsnbcmp((PCUStr)pszA, (PCUStr)pszB, stSize);
}

INLINE Int CMChar::CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _mbsnbicmp((PCUStr)pszA, (PCUStr)pszB, stSize);
}

INLINE Int CMChar::Collate(PCTStr pszA, PCTStr pszB)
{
	return _mbscoll((PCUStr)pszA, (PCUStr)pszB);
}

INLINE Int CMChar::CollateIgnore(PCTStr pszA, PCTStr pszB)
{
	return _mbsicoll((PCUStr)pszA, (PCUStr)pszB);
}

INLINE Int CMChar::Collaten(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _mbsnbcoll((PCUStr)pszA, (PCUStr)pszB, stSize);
}

INLINE Int CMChar::CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _mbsnbicoll((PCUStr)pszA, (PCUStr)pszB, stSize);
}

INLINE Int CMChar::GetFormattedLength(PCTStr pszFormat, va_list args)
{
	return _vscprintf(pszFormat, args);
}

INLINE Int CMChar::VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args)
{
	return _vsnprintf_s(pszBuf, stSize, _TRUNCATE, pszFormat, args);
}

INLINE Int CMChar::Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = VFormat(pszBuf, stSize, pszFormat, vl);
	va_end(vl);

	return nRet;
}

INLINE bool CMChar::Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount)
{
	return (_mbsnbset_s((PUStr)pszStr, stSize, (UInt)ch, stCount) == 0);
}

INLINE bool CMChar::Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (_mbsnbcpy_s((PUStr)pszDst, stDst, (PCUStr)pszSrc, stSrc) == 0);
}

INLINE bool CMChar::Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (_mbsnbcat_s((PUStr)pszDst, stDst, (PCUStr)pszSrc, stSrc) == 0);
}

INLINE bool CMChar::ToLower(PTStr pszStr, size_t stSize)
{
	return (_mbslwr_s((PUStr)pszStr, stSize) == 0);
}

INLINE bool CMChar::ToUpper(PTStr pszStr, size_t stSize)
{
	return (_mbsupr_s((PUStr)pszStr, stSize) == 0);
}

INLINE CMChar::TChar CMChar::ToLower(TChar ch)
{
	return (TChar)_mbctolower((UInt)ch);
}

INLINE CMChar::TChar CMChar::ToUpper(TChar ch)
{
	return (TChar)_mbctoupper((UInt)ch);
}

INLINE Int CMChar::ToInt(PCTStr pszStr)
{
	return atoi(pszStr);
}

INLINE Long CMChar::ToLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return strtol(pszStr, ppEnd, nRadix);
}

INLINE ULong CMChar::ToULong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return strtoul(pszStr, ppEnd, nRadix);
}

INLINE LLong CMChar::ToLLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _strtoi64(pszStr, ppEnd, nRadix);
}

INLINE ULLong CMChar::ToULLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _strtoui64(pszStr, ppEnd, nRadix);
}

INLINE Double CMChar::ToDouble(PCTStr pszStr, PTStr* ppEnd)
{
	return strtod(pszStr, ppEnd);
}

INLINE bool CMChar::ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_itoa_s(nValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CMChar::ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ltoa_s(lValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CMChar::ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ultoa_s(ulValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CMChar::ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_i64toa_s(llValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CMChar::ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ui64toa_s(ullValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CMChar::ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_gcvt_s(pszStr, stSize, dValue, nRadix) == 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CWChar
INLINE bool CWChar::IsAlpha(TChar ch)
{
	return (iswalpha(ch) > 0);
}

INLINE bool CWChar::IsDigit(TChar ch)
{
	return (iswdigit(ch) > 0);
}

INLINE bool CWChar::IsXDigit(TChar ch)
{
	return (iswxdigit(ch) > 0);
}

INLINE bool CWChar::IsSpace(TChar ch)
{
	return (iswspace(ch) > 0);
}

INLINE bool CWChar::IsLower(TChar ch)
{
	return (iswlower(ch) > 0);
}

INLINE bool CWChar::IsUpper(TChar ch)
{
	return (iswupper(ch) > 0);
}

INLINE CWChar::PTStr CWChar::Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext)
{
	return wcstok_s(pszStr, pszDelimit, &pszContext);
}

INLINE CWChar::PTStr CWChar::Reverse(PTStr pszStr)
{
	return _wcsrev(pszStr);
}

INLINE CWChar::PCTStr CWChar::FindChar(PCTStr pszStr, TChar ch)
{
	return wcschr(pszStr, ch);
}

INLINE CWChar::PCTStr CWChar::FindCharRev(PCTStr pszStr, TChar ch)
{
	return wcsrchr(pszStr, ch);
}

INLINE CWChar::PCTStr CWChar::FindString(PCTStr pszStr, PCTStr pszMatch)
{
	return wcsstr(pszStr, pszMatch);
}

INLINE CWChar::PCTStr CWChar::FindOneOf(PCTStr pszStr, PCTStr pszMatch)
{
	return wcspbrk(pszStr, pszMatch);
}

INLINE size_t CWChar::FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch)
{
	return wcscspn(pszStr, pszMatch);
}

INLINE size_t CWChar::FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch)
{
	return wcsspn(pszStr, pszMatch);
}

INLINE size_t CWChar::Length(PCTStr pszStr, size_t stMax)
{
	return wcsnlen(pszStr, stMax);
}

INLINE Int CWChar::Compare(PCTStr pszA, PCTStr pszB)
{
	return wcscmp(pszA, pszB);
}

INLINE Int CWChar::CompareIgnore(PCTStr pszA, PCTStr pszB)
{
	return _wcsicmp(pszA, pszB);
}

INLINE Int CWChar::Comparen(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return wcsncmp(pszA, pszB, stSize);
}

INLINE Int CWChar::CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _wcsnicmp(pszA, pszB, stSize);
}

INLINE Int CWChar::Collate(PCTStr pszA, PCTStr pszB)
{
	return wcscoll(pszA, pszB);
}

INLINE Int CWChar::CollateIgnore(PCTStr pszA, PCTStr pszB)
{
	return _wcsicoll(pszA, pszB);
}

INLINE Int CWChar::Collaten(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _wcsncoll(pszA, pszB, stSize);
}

INLINE Int CWChar::CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _wcsnicoll(pszA, pszB, stSize);
}

INLINE Int CWChar::GetFormattedLength(PCTStr pszFormat, va_list args)
{
	return _vscwprintf(pszFormat, args);
}

INLINE Int CWChar::VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args)
{
	return _vsnwprintf_s(pszBuf, stSize, _TRUNCATE, pszFormat, args);
}

INLINE Int CWChar::Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = VFormat(pszBuf, stSize, pszFormat, vl);
	va_end(vl);

	return nRet;
}

INLINE bool CWChar::Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount)
{
	return (_wcsnset_s(pszStr, stSize, ch, stCount) == 0);
}

INLINE bool CWChar::Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (wcsncpy_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CWChar::Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (wcsncat_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CWChar::ToLower(PTStr pszStr, size_t stSize)
{
	return (_wcslwr_s(pszStr, stSize) == 0);
}

INLINE bool CWChar::ToUpper(PTStr pszStr, size_t stSize)
{
	return (_wcsupr_s(pszStr, stSize) == 0);
}

INLINE CWChar::TChar CWChar::ToLower(TChar ch)
{
	return (TChar)towlower(ch);
}

INLINE CWChar::TChar CWChar::ToUpper(TChar ch)
{
	return (TChar)towupper(ch);
}

INLINE Int CWChar::ToInt(PCTStr pszStr)
{
	return _wtoi(pszStr);
}

INLINE Long CWChar::ToLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return wcstol(pszStr, ppEnd, nRadix);
}

INLINE ULong CWChar::ToULong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return wcstoul(pszStr, ppEnd, nRadix);
}

INLINE LLong CWChar::ToLLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _wcstoi64(pszStr, ppEnd, nRadix);
}

INLINE ULLong CWChar::ToULLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _wcstoui64(pszStr, ppEnd, nRadix);
}

INLINE Double CWChar::ToDouble(PCTStr pszStr, PTStr* ppEnd)
{
	return wcstod(pszStr, ppEnd);
}

INLINE bool CWChar::ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_itow_s(nValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CWChar::ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ltow_s(lValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CWChar::ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ultow_s(ulValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CWChar::ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_i64tow_s(llValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CWChar::ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ui64tow_s(ullValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CWChar::ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix)
{	
	Char cTemp[_CVTBUFSIZE] = { 0 };
	if (_gcvt_s(cTemp, _CVTBUFSIZE, dValue, nRadix) == 0)
	{
		if (stSize > _CVTBUFSIZE)
		{
			stSize = _CVTBUFSIZE;
		}
		for (size_t i = 0; i < stSize; ++i)
		{
			if (cTemp[i] != 0)
			{
				pszStr[i] = (TChar)cTemp[i];
			}
		}
		pszStr[stSize - 1] = 0;
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXChar
INLINE bool CXChar::IsAlpha(TChar ch)
{
	return (_istalpha(ch) > 0);
}

INLINE bool CXChar::IsDigit(TChar ch)
{
	return (_istdigit(ch) > 0);
}

INLINE bool CXChar::IsXDigit(TChar ch)
{
	return (_istxdigit(ch) > 0);
}

INLINE bool CXChar::IsSpace(TChar ch)
{
	return (_istspace(ch) > 0);
}

INLINE bool CXChar::IsLower(TChar ch)
{
	return (_istlower(ch) > 0);
}

INLINE bool CXChar::IsUpper(TChar ch)
{
	return (_istupper(ch) > 0);
}

INLINE CXChar::PTStr CXChar::Token(PTStr pszStr, PCTStr pszDelimit, PTStr& pszContext)
{
	return _tcstok_s(pszStr, pszDelimit, &pszContext);
}

INLINE CXChar::PTStr CXChar::Reverse(PTStr pszStr)
{
	return _tcsrev(pszStr);
}

INLINE CXChar::PCTStr CXChar::FindChar(PCTStr pszStr, TChar ch)
{
	return _tcschr(pszStr, ch);
}

INLINE CXChar::PCTStr CXChar::FindCharRev(PCTStr pszStr, TChar ch)
{
	return _tcsrchr(pszStr, ch);
}

INLINE CXChar::PCTStr CXChar::FindString(PCTStr pszStr, PCTStr pszMatch)
{
	return _tcsstr(pszStr, pszMatch);
}

INLINE CXChar::PCTStr CXChar::FindOneOf(PCTStr pszStr, PCTStr pszMatch)
{
	return _tcspbrk(pszStr, pszMatch);
}

INLINE size_t CXChar::FindOneIndexOf(PCTStr pszStr, PCTStr pszMatch)
{
	return _tcscspn(pszStr, pszMatch);
}

INLINE size_t CXChar::FindOneIndexNotOf(PCTStr pszStr, PCTStr pszMatch)
{
	return _tcsspn(pszStr, pszMatch);
}

INLINE size_t CXChar::Length(PCTStr pszStr, size_t stMax)
{
	return _tcsnlen(pszStr, stMax);
}

INLINE Int CXChar::Compare(PCTStr pszA, PCTStr pszB)
{
	return _tcscmp(pszA, pszB);
}

INLINE Int CXChar::CompareIgnore(PCTStr pszA, PCTStr pszB)
{
	return _tcsicmp(pszA, pszB);
}

INLINE Int CXChar::Comparen(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _tcsncmp(pszA, pszB, stSize);
}

INLINE Int CXChar::CompareIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _tcsnicmp(pszA, pszB, stSize);
}

INLINE Int CXChar::Collate(PCTStr pszA, PCTStr pszB)
{
	return _tcscoll(pszA, pszB);
}

INLINE Int CXChar::CollateIgnore(PCTStr pszA, PCTStr pszB)
{
	return _tcsicoll(pszA, pszB);
}

INLINE Int CXChar::Collaten(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _tcsncoll(pszA, pszB, stSize);
}

INLINE Int CXChar::CollateIgnoren(PCTStr pszA, PCTStr pszB, size_t stSize)
{
	return _tcsnicoll(pszA, pszB, stSize);
}

INLINE Int CXChar::GetFormattedLength(PCTStr pszFormat, va_list args)
{
	return _vsctprintf(pszFormat, args);
}

INLINE Int CXChar::VFormat(PTStr pszBuf, size_t stSize, PCTStr pszFormat, va_list args)
{
	return _vsntprintf_s(pszBuf, stSize, _TRUNCATE, pszFormat, args);
}

INLINE Int CXChar::Format(PTStr pszBuf, size_t stSize, PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = VFormat(pszBuf, stSize, pszFormat, vl);
	va_end(vl);

	return nRet;
}

INLINE bool CXChar::Replace(PTStr pszStr, size_t stSize, TChar ch, size_t stCount)
{
	return (_tcsnset_s(pszStr, stSize, ch, stCount) == 0);
}

INLINE bool CXChar::Copy(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (_tcsncpy_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CXChar::Concat(PTStr pszDst, size_t stDst, PCTStr pszSrc, size_t stSrc)
{
	return (_tcsncat_s(pszDst, stDst, pszSrc, stSrc) == 0);
}

INLINE bool CXChar::ToLower(PTStr pszStr, size_t stSize)
{
	return (_tcslwr_s(pszStr, stSize) == 0);
}

INLINE bool CXChar::ToUpper(PTStr pszStr, size_t stSize)
{
	return (_tcsupr_s(pszStr, stSize) == 0);
}

INLINE CXChar::TChar CXChar::ToLower(TChar ch)
{
	return (TChar)_totlower(ch);
}

INLINE CXChar::TChar CXChar::ToUpper(TChar ch)
{
	return (TChar)_totupper(ch);
}

INLINE Int CXChar::ToInt(PCTStr pszStr)
{
	return _ttoi(pszStr);
}

INLINE Long CXChar::ToLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _tcstol(pszStr, ppEnd, nRadix);
}

INLINE ULong CXChar::ToULong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _tcstoul(pszStr, ppEnd, nRadix);
}

INLINE LLong CXChar::ToLLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _tcstoi64(pszStr, ppEnd, nRadix);
}

INLINE ULLong CXChar::ToULLong(PCTStr pszStr, PTStr* ppEnd, Int nRadix)
{
	return _tcstoui64(pszStr, ppEnd, nRadix);
}

INLINE Double CXChar::ToDouble(PCTStr pszStr, PTStr* ppEnd)
{
	return _tcstod(pszStr, ppEnd);
}

INLINE bool CXChar::ToString(Int nValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_itot_s(nValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CXChar::ToString(Long lValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ltot_s(lValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CXChar::ToString(ULong ulValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ultot_s(ulValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CXChar::ToString(LLong llValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_i64tot_s(llValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CXChar::ToString(ULLong ullValue, PTStr pszStr, size_t stSize, Int nRadix)
{
	return (_ui64tot_s(ullValue, pszStr, stSize, nRadix) == 0);
}

INLINE bool CXChar::ToString(Double dValue, PTStr pszStr, size_t stSize, Int nRadix)
{
#ifdef RUNTIME_UNICODE
	Char cTemp[_CVTBUFSIZE] = { 0 };
	if (_gcvt_s(cTemp, _CVTBUFSIZE, dValue, nRadix) == 0)
	{
		if (stSize > _CVTBUFSIZE)
		{
			stSize = _CVTBUFSIZE;
		}
		for (size_t i = 0; i < stSize; ++i)
		{
			if (cTemp[i] != 0)
			{
				pszStr[i] = (TChar)cTemp[i];
			}
		}
		pszStr[stSize - 1] = 0;
		return true;
	}
	return false;
#else  // RUNTIME_UNICODE
	return (_gcvt_s(pszStr, stSize, dValue, nRadix) == 0);
#endif // RUNTIME_UNICODE
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TRAITS_INL__