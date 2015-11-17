///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : tstring.inl                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : string                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TSTRING_INL__
#define __TSTRING_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStringRef
template <typename T>
INLINE CTStringRef<T>::CTStringRef(PCTStr psz)
: m_stLen(0)
, m_pszRef(psz)
{
	if (m_pszRef != nullptr)
	{
		m_stLen = T::Length(m_pszRef);
	}
}

template <typename T>
INLINE CTStringRef<T>::~CTStringRef(void)
{
}

template <typename T>
INLINE CTStringRef<T>::CTStringRef(const CTStringRef<T>& aSrc)
: m_stLen(aSrc.m_stLen)
, m_pszRef(aSrc.m_pszRef)
{
}

template <typename T>
INLINE CTStringRef<T>& CTStringRef<T>::operator=(const CTStringRef<T>& aSrc)
{
	if (&aSrc != this)
	{
		m_stLen  = aSrc.m_stLen;
		m_pszRef = aSrc.m_pszRef;
	}
	return (*this);
}

template <typename T>
INLINE CTStringRef<T>& CTStringRef<T>::operator=(PCTStr psz)
{
	m_pszRef = psz;
	if (m_pszRef != nullptr)
	{
		m_stLen = T::Length(m_pszRef);
	}
	return (*this);
}

template <typename T>
INLINE typename CTStringRef<T>::TChar CTStringRef<T>::operator[](size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_szBuffer[stIndex];
	}
	return 0;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::operator*(void)const
{
	return m_pszRef;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::GetBuffer(void) const
{
	return m_pszRef;
}

template <typename T>
INLINE bool CTStringRef<T>::IsEmpty(void) const
{
	return (m_stLen == 0);
}

template <typename T>
INLINE size_t CTStringRef<T>::Length(bool bStream) const
{
	if (bStream == false)
	{
		return m_stLen;
	}
	else
	{
		return (m_stLen * sizeof(TChar) + sizeof(UInt));
	}
}

template <typename T>
INLINE bool CTStringRef<T>::IsDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsDigit(m_pszRef[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTStringRef<T>::IsXDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsXDigit(m_pszRef[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTStringRef<T>::IsSpace(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsSpace(m_pszRef[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTStringRef<T>::IsLower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsLower(m_pszRef[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTStringRef<T>::IsUpper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsUpper(m_pszRef[stIndex]);
	}
	return false;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::FindChar(TChar ch)
{
	if (m_stLen > 0)
	{
		return T::FindChar(m_pszRef, ch);
	}
	return nullptr;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::FindCharRev(TChar ch)
{
	if (m_stLen > 0)
	{
		return T::FindCharRev(m_pszRef, ch);
	}
	return nullptr;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::FindString(PCTStr pszMatch)
{
	if (m_stLen > 0)
	{
		return T::FindString(m_pszRef, pszMatch);
	}
	return nullptr;
}

template <typename T>
INLINE typename CTStringRef<T>::PCTStr CTStringRef<T>::FindOneOf(PCTStr pszMatch)
{
	if (m_stLen > 0)
	{
		return T::FindOneOf(m_pszRef, pszMatch);
	}
	return nullptr;
}

template <typename T>
INLINE size_t CTStringRef<T>::FindOneIndexOf(PCTStr pszMatch)
{
	if (m_stLen > 0)
	{
		return T::FindOneIndexOf(m_pszRef, pszMatch);
	}
	return nullptr;
}

template <typename T>
INLINE size_t CTStringRef<T>::FindOneIndexNotOf(PCTStr pszMatch)
{
	if (m_stLen > 0)
	{
		return T::FindOneIndexNotOf(m_pszRef, pszMatch);
	}
	return nullptr;
}

template <typename T>
INLINE Int CTStringRef<T>::Compare(PCTStr pszCmp)
{
	return T::Compare(m_pszRef, pszCmp);
}

template <typename T>
INLINE Int CTStringRef<T>::CompareIgnore(PCTStr pszCmp)
{
	return T::CompareIgnore(m_pszRef, pszCmp);
}

template <typename T>
INLINE Int CTStringRef<T>::Comparen(PCTStr pszCmp, size_t stSize)
{
	return T::Comparen(m_pszRef, pszCmp, stSize);
}

template <typename T>
INLINE Int CTStringRef<T>::CompareIgnoren(PCTStr pszCmp, size_t stSize)
{
	return T::CompareIgnoren(m_pszRef, pszCmp, stSize);
}

template <typename T>
INLINE Int CTStringRef<T>::Collate(PCTStr pszCmp)
{
	return T::Collate(m_pszRef, pszCmp);
}

template <typename T>
INLINE Int CTStringRef<T>::CollateIgnore(PCTStr pszCmp)
{
	return T::CollateIgnore(m_pszRef, pszCmp);
}

template <typename T>
INLINE Int CTStringRef<T>::Collaten(PCTStr pszCmp, size_t stSize)
{
	return T::Collaten(m_pszRef, pszCmp, stSize);
}

template <typename T>
INLINE Int CTStringRef<T>::CollateIgnoren(PCTStr pszCmp, size_t stSize)
{
	return T::CollateIgnoren(m_pszRef, pszCmp, stSize);
}

template <typename T>
INLINE Int CTStringRef<T>::ToInt(void) const
{
	if (m_stLen > 0)
	{
		return T::ToInt(m_pszRef);
	}
	return 0;
}

template <typename T>
INLINE Long CTStringRef<T>::ToLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLong(m_pszRef, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE ULong CTStringRef<T>::ToULong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULong(m_pszRef, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE LLong CTStringRef<T>::ToLLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLLong(m_pszRef, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE ULLong CTStringRef<T>::ToULLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULLong(m_pszRef, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE Double CTStringRef<T>::ToDouble(void) const
{
	if (m_stLen > 0)
	{
		return T::ToDouble(m_pszRef, nullptr);
	}
	return 0;
}

template <typename T>
INLINE Long CTStringRef<T>::ToLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd = nullptr;
		Long  lRet = T::ToLong((m_pszRef + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszRef + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszRef);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return lRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE ULong CTStringRef<T>::ToULong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		ULong ulRet = T::ToULong((m_pszRef + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszRef + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszRef);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ulRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE LLong CTStringRef<T>::ToLLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		LLong llRet = T::ToLLong((m_pszRef + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszRef + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszRef);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return llRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE ULLong CTStringRef<T>::ToULLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd   = nullptr;
		ULLong ullRet = T::ToULLong((m_pszRef + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszRef + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszRef);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ullRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE Double CTStringRef<T>::ToDouble(size_t& stIndex)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd = nullptr;
		Double dRet = T::ToDouble((m_pszRef + stIndex), &pEnd);
		if ((pEnd != (m_pszRef + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszRef);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return dRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStringFix
template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>::CTStringFix(PCTStr psz, size_t stLen)
: m_stLen(0)
{
	m_szBuffer[0] = 0;
	FillBuffer(psz, stLen);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>::~CTStringFix(void)
{
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>::CTStringFix(const CTStringFix<T, stFix>& aSrc)
: m_stLen(0)
{
	m_szBuffer[0] = 0;
	FillBuffer(aSrc.m_szBuffer, aSrc.m_stLen);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE CTStringFix<T, stFix>::CTStringFix(const CTStringFix<T, stLenT>& aSrc)
: m_stLen(0)
{
	m_szBuffer[0] = 0;
	FillBuffer(*aSrc, aSrc.Length());
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>::CTStringFix(const CTStringRef<T>& aSrc)
: m_stLen(0)
{
	m_szBuffer[0] = 0;
	FillBuffer(*aSrc, aSrc.Length());
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>::CTStringFix(const CTString<T>& aSrc)
: m_stLen(0)
{
	m_szBuffer[0] = 0;
	FillBuffer(*aSrc, aSrc.Length());
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator=(PCTStr psz)
{
	assert(psz != nullptr);
	assert(psz != m_szBuffer);
	FillBuffer(psz);
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator=(const CTStringFix<T, stFix>& aSrc)
{
	if (&aSrc != this)
	{
		FillBuffer(aSrc.m_szBuffer, aSrc.m_stLen);
	}
	return (*this);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator=(const CTStringFix<T, stLenT>& aSrc)
{
	FillBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator=(const CTStringRef<T>& aSrc)
{
	FillBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator=(const CTString<T>& aSrc)
{
	FillBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(TChar ch)
{
	assert(ch != 0);
	AppendBuffer(ch);
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(PCTStr psz)
{
	assert(psz != nullptr);
	AppendBuffer(psz);
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(const CTStringFix<T, stFix>& aSrc)
{
	AppendBuffer(aSrc.m_szBuffer, aSrc.m_stLen);
	return (*this);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(const CTStringFix<T, stLenT>& aSrc)
{
	AppendBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(const CTStringRef<T>& aSrc)
{
	AppendBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix>& CTStringFix<T, stFix>::operator+=(const CTString<T>& aSrc)
{
	AppendBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) <= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) < 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) >= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) > 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator==(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) == 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator!=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_szBuffer, psz) != 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<=(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) <= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) < 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>=(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) >= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) > 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator==(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) == 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator!=(const CTStringFix<T, stFix>& aSrc) const
{
	return (T::Compare(m_szBuffer, aSrc.m_szBuffer) != 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator<=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) <= 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator<(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) < 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator>=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) >= 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator>(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) > 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator==(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) == 0);
}

template <typename T, size_t stFix>
template <size_t stLenT>
INLINE bool CTStringFix<T, stFix>::operator!=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) != 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) <= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) < 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) >= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) > 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator==(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) == 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator!=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) != 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) <= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator<(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) < 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) >= 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator>(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) > 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator==(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) == 0);
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::operator!=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_szBuffer, *aSrc) != 0);
}

template <typename T, size_t stFix>
INLINE typename CTStringFix<T, stFix>::TChar CTStringFix<T, stFix>::operator[](size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_szBuffer[stIndex];
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE typename CTStringFix<T, stFix>::PTStr CTStringFix<T, stFix>::operator*(void)
{
	return m_szBuffer;
}

template <typename T, size_t stFix>
INLINE typename CTStringFix<T, stFix>::PCTStr CTStringFix<T, stFix>::operator*(void)const
{
	return m_szBuffer;
}

//template <typename T, size_t stFix>
//INLINE CTStringFix<T, stFix>::operator typename CTStringFix<T, stFix>::PCTStr(void) const
//{
//	return m_szBuffer;
//}

template <typename T, size_t stFix>
INLINE typename CTStringFix<T, stFix>::PCTStr CTStringFix<T, stFix>::GetBuffer(void) const
{
	return m_szBuffer;
}

template <typename T, size_t stFix>
INLINE typename T::TChar CTStringFix<T, stFix>::GetAt(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_szBuffer[stIndex];
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::SetAt(size_t stIndex, TChar ch)
{
	if ((stIndex < m_stLen) && (ch > 0))
	{
		m_szBuffer[stIndex] = ch;
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::FillBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = T::Length(psz, stFix);
	}
	if (stLen == 0)
	{
		return false;
	}
	else if (stLen >= stFix)
	{
		stLen = stFix - 1;
	}
	T::Copy(m_szBuffer, stFix, psz, stLen);
	m_szBuffer[stLen] = 0;
	m_stLen = stLen;
	return true;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::AppendBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = T::Length(psz, stFix);
	}
	if (stLen == 0)
	{
		return false;
	}
	else if ((m_stLen + stLen) >= stFix)
	{
		stLen = stFix - 1 - m_stLen;
	}
	T::Copy(m_szBuffer + m_stLen, stFix - m_stLen, psz, stLen);
	m_stLen += stLen;
	m_szBuffer[m_stLen] = 0;
	return true;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::AppendBuffer(TChar ch)
{
	if ((m_stLen < (stFix - 1)) && (ch > 0))
	{
		m_szBuffer[m_stLen] = ch;
		++m_stLen;
		m_szBuffer[m_stLen] = 0;
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsEmpty(void) const
{
	return (m_stLen == 0);
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Empty(void)
{
	m_szBuffer[0] = 0;
	m_stLen = 0;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::ResetLength(size_t stLen)
{
	if (stLen == 0)
	{
		m_stLen = T::Length(m_szBuffer, stFix);
	}
	else
	{
		m_stLen = stLen;
	}
	if (m_stLen >= stFix)
	{
		m_stLen = 0;
	}
	m_szBuffer[m_stLen] = 0;
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Length(bool bStream) const
{
	if (bStream == false)
	{
		return m_stLen;
	}
	else
	{
		return (m_stLen * sizeof(TChar) + sizeof(UInt));
	}
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::BufferLength(void) const
{
	return stFix;
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Find(TChar ch, size_t stStart, bool bRev) const
{
	assert(stStart < m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = nullptr;
		if (bRev == false)
		{
			p = T::FindChar(GetBuffer() + stStart, ch);
		}
		else
		{
			p = T::FindCharRev(GetBuffer() + stStart, ch);
		}
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_szBuffer);
		}
	}
	return -1;
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Find(PCTStr pszSub, size_t stStart) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = T::FindString(GetBuffer() + stStart, pszSub);
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_szBuffer);
		}
	}
	return -1;
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::FindOneOf(PCTStr pszMatch, size_t stStart, bool bInMatch) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		size_t stRet = 0;
		if (bInMatch)
		{
			stRet = T::FindOneIndexOf(GetBuffer() + stStart, pszMatch);
		}
		else
		{
			stRet = T::FindOneIndexNotOf(GetBuffer() + stStart, pszMatch);
		}
		if (stRet < m_stLen)
		{
			return ((Int)stRet);
		}
	}
	return -1;
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Compare(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::Compare(m_szBuffer, psz);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::CompareIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::CompareIgnore(m_szBuffer, psz);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Comparen(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::Comparen(m_szBuffer, psz, stLen);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::CompareIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::CompareIgnoren(m_szBuffer, psz, stLen);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Collate(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::Collate(m_szBuffer, psz);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::CollateIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::CollateIgnore(m_szBuffer, psz);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::Collaten(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::Collaten(m_szBuffer, psz, stLen);
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::CollateIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::CollateIgnoren(m_szBuffer, psz, stLen);
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Delete(size_t stIndex, size_t stCount)
{
	if ((stIndex < m_stLen) && (stCount > 0))
	{
		if ((stIndex + stCount) > m_stLen)
		{
			stCount = m_stLen - stIndex;
		}
		size_t stMove = (m_stLen - (stIndex + stCount) + 1) * sizeof(TChar);
		m_stLen -= stCount;
		MM_SAFE::Memmov((m_szBuffer + stIndex), stMove, (m_szBuffer + stIndex + stCount), stMove);
		m_szBuffer[m_stLen] = 0;
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Insert(size_t stIndex, TChar ch)
{
	if ((m_stLen < (stFix - 1)) && (ch > 0))
	{
		if (stIndex < m_stLen)
		{
			++m_stLen;
			size_t stMove = (m_stLen - stIndex) * sizeof(TChar); // include 0
			MM_SAFE::Memmov((m_szBuffer + stIndex + 1), stMove, (m_szBuffer + stIndex), stMove);
			m_szBuffer[stIndex] = ch;
		}
		else
		{
			m_szBuffer[m_stLen] = ch;
			++m_stLen;
			m_szBuffer[m_stLen] = 0;
		}
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Insert(size_t stIndex, PCTStr psz)
{
	assert(psz != nullptr);
	size_t stLen = T::Length(psz);
	if ((stLen + m_stLen) <= (stFix - 1))
	{
		if (stIndex < m_stLen)
		{
			size_t stMove = (m_stLen - stIndex + 1) * sizeof(TChar); // include 0
			MM_SAFE::Memmov((m_szBuffer + stIndex + stLen), stMove, (m_szBuffer + stIndex), stMove);
			MM_SAFE::Memcpy((m_szBuffer + stIndex), stLen * sizeof(TChar), psz, stLen * sizeof(TChar));

			m_stLen += stLen;
		}
		else
		{
			MM_SAFE::Memcpy((m_szBuffer + m_stLen), stLen * sizeof(TChar), psz, stLen * sizeof(TChar));
			m_stLen += stLen;
			m_szBuffer[m_stLen] = 0;
		}
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Replace(TChar chOld, TChar chNew)
{
	size_t stReplace = 0;
	if ((chOld != chNew) && (chNew > 0))
	{
		for (size_t i = 0; i < m_stLen; ++i)
		{
			if (m_szBuffer[i] == chOld)
			{
				m_szBuffer[i] = chNew;
				++stReplace;
			}
		}
	}
	return stReplace;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Replace(PCTStr pszOld, PCTStr pszNew)
{
	size_t stOldLen = T::Length(pszOld);
	if (stOldLen > 0)
	{
		size_t stNewLen = T::Length(pszNew);
		if ((stNewLen != stOldLen) || (T::Comparen(pszOld, pszNew, stNewLen) != 0))
		{
			size_t stCount = 0;
			PCTStr pFind   = nullptr;
			for (size_t i = 0; i < m_stLen; )
			{
				pFind = T::FindString((m_szBuffer + i), pszOld);
				if (pFind != nullptr)
				{
					++stCount;
					i = (pFind - m_szBuffer) + stOldLen;
				}
				else
				{
					break;
				}
			}

			if ((stCount > 0) && ((m_stLen + stNewLen * stCount - stOldLen * stCount) <= (stFix - 1)))
			{
				size_t   stCopy          = 0;
				size_t   stLen           = 0;
				TChar    szBuffer[stFix] = { 0 };
				for (PCTStr pFindOld = m_szBuffer; pFindOld != nullptr; )
				{
					pFind = T::FindString(pFindOld, pszOld);
					if (pFind != nullptr)
					{
						stCopy = (pFind - pFindOld);
						MM_SAFE::Memcpy((szBuffer + stLen), stCopy * sizeof(TChar), pFindOld, stCopy * sizeof(TChar));
						stLen += stCopy;
						MM_SAFE::Memcpy((szBuffer + stLen), stNewLen * sizeof(TChar), pszNew, stNewLen * sizeof(TChar));
						stLen += stNewLen;
						
						pFindOld = pFind + stOldLen;
					}
					else
					{
						stCopy = (m_stLen - (pFindOld - m_szBuffer));
						MM_SAFE::Memcpy((szBuffer + stLen), stCopy * sizeof(TChar), pFindOld, stCopy * sizeof(TChar));
						stLen += stCopy;
						MM_SAFE::Memcpy(m_szBuffer, stLen * sizeof(TChar), szBuffer, stLen * sizeof(TChar));
						m_szBuffer[stLen] = 0;
						m_stLen = stLen;
						break;
					}
				}
				return stCount;
			}
		}
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Remove(TChar ch)
{
	if (ch != 0)
	{
		size_t stIndex = 0;
		for (size_t i = 0; i < m_stLen; ++i)
		{
			if (m_szBuffer[i] != ch)
			{
				m_szBuffer[stIndex] = m_szBuffer[i];
				++stIndex;
			}
		}
		if (stIndex > 0)
		{
			m_szBuffer[stIndex] = 0;
			m_stLen = stIndex;
		}
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::Format(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = T::VFormat(m_szBuffer, stFix, pszFormat, vl);
	va_end(vl);

	if (nRet > 0)
	{
		m_stLen = (size_t)nRet;
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE size_t CTStringFix<T, stFix>::AppendFormat(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = T::VFormat((m_szBuffer + m_stLen), stFix - m_stLen, pszFormat, vl);
	va_end(vl);

	if (nRet > 0)
	{
		m_stLen += (size_t)nRet;
	}
	return m_stLen;
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Upper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_szBuffer[stIndex] = T::ToUpper(m_szBuffer[stIndex]);
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Lower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_szBuffer[stIndex] = T::ToLower(m_szBuffer[stIndex]);
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Upper(void)
{
	if (m_stLen > 0)
	{
		T::ToUpper(m_szBuffer, m_stLen + 1);
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Lower(void)
{
	if (m_stLen > 0)
	{
		T::ToLower(m_szBuffer, m_stLen + 1);
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Reverse(void)
{
	if (m_stLen > 0)
	{
		T::Reverse(m_szBuffer);
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::TrimLeft(TChar ch)
{
	size_t stTrim = 0;
	for (size_t i = 0; i < m_stLen; ++i)
	{
		if (m_szBuffer[i] == ch)
		{
			++stTrim;
		}
		else
		{
			assert(stTrim <= m_stLen);
			if (stTrim > 0)
			{
				MM_SAFE::Memmov(m_szBuffer,          (m_stLen - stTrim) * sizeof(TChar),
						        m_szBuffer + stTrim, (m_stLen - stTrim) * sizeof(TChar));
				m_stLen -= stTrim;
				m_szBuffer[m_stLen] = 0;
			}
			break;
		}
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::TrimRight(TChar ch)
{
	size_t stTrim = m_stLen;
	for (size_t i = m_stLen ; i > 0; --i)
	{
		if (m_szBuffer[i - 1] == ch)
		{
			--stTrim;
		}
		else
		{
			if (stTrim < m_stLen)
			{
				m_szBuffer[stTrim] = 0;
				m_stLen = stTrim;
			}
			break;
		}
	}
}

template <typename T, size_t stFix>
INLINE void CTStringFix<T, stFix>::Trim(TChar ch)
{
	TrimRight(ch);
	TrimLeft(ch);
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix> CTStringFix<T, stFix>::Left(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CTStringFix<T, stFix>(m_szBuffer, stCount));
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix> CTStringFix<T, stFix>::Right(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CTStringFix<T, stFix>(m_szBuffer + m_stLen - stCount, stCount));
}

template <typename T, size_t stFix>
INLINE CTStringFix<T, stFix> CTStringFix<T, stFix>::Mid(size_t stStart, size_t stCount) const
{
	assert(stStart <= m_stLen);

	stStart = DEF_BASE::Min<size_t>(stStart, m_stLen);
	stCount = DEF_BASE::Min<size_t>(stCount, (m_stLen - stStart));
	return (CTStringFix<T, stFix>(m_szBuffer + stStart, stCount));
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsDigit(m_szBuffer[stIndex]);
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsXDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsXDigit(m_szBuffer[stIndex]);
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsSpace(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsSpace(m_szBuffer[stIndex]);
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsLower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsLower(m_szBuffer[stIndex]);
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::IsUpper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsUpper(m_szBuffer[stIndex]);
	}
	return false;
}

template <typename T, size_t stFix>
INLINE Int CTStringFix<T, stFix>::ToInt(void) const
{
	if (m_stLen > 0)
	{
		return T::ToInt(m_szBuffer);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE Long CTStringFix<T, stFix>::ToLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLong(m_szBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE ULong CTStringFix<T, stFix>::ToULong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULong(m_szBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE LLong CTStringFix<T, stFix>::ToLLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLLong(m_szBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE ULLong CTStringFix<T, stFix>::ToULLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULLong(m_szBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE Double CTStringFix<T, stFix>::ToDouble(void) const
{
	if (m_stLen > 0)
	{
		return T::ToDouble(m_szBuffer, nullptr);
	}
	return 0;
}

template <typename T, size_t stFix>
INLINE Long CTStringFix<T, stFix>::ToLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd = nullptr;
		Long  lRet = T::ToLong((m_szBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_szBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_szBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return lRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T, size_t stFix>
INLINE ULong CTStringFix<T, stFix>::ToULong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		ULong ulRet = T::ToULong((m_szBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_szBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_szBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ulRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T, size_t stFix>
INLINE LLong CTStringFix<T, stFix>::ToLLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		LLong llRet = T::ToLLong((m_szBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_szBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_szBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return llRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T, size_t stFix>
INLINE ULLong CTStringFix<T, stFix>::ToULLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd   = nullptr;
		ULLong ullRet = T::ToULLong((m_szBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_szBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_szBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ullRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T, size_t stFix>
INLINE Double CTStringFix<T, stFix>::ToDouble(size_t& stIndex)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd = nullptr;
		Double dRet = T::ToDouble((m_szBuffer + stIndex), &pEnd);
		if ((pEnd != (m_szBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_szBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return dRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(Int nValue, Int nRadix)
{
	if (T::ToString(nValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(Long lValue, Int nRadix)
{
	if (T::ToString(lValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(ULong ulValue, Int nRadix)
{
	if (T::ToString(ulValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(LLong llValue, Int nRadix)
{
	if (T::ToString(llValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(ULLong ullValue, Int nRadix)
{
	if (T::ToString(ullValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T, size_t stFix>
INLINE bool CTStringFix<T, stFix>::ToString(Double dValue, Int nRadix)
{
	if (T::ToString(dValue, m_szBuffer, stFix, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTString
template <typename T>
INLINE CTString<T>::CTString(void)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
}

template <typename T>
INLINE CTString<T>::~CTString(void)
{
	if (m_pszBuffer != nullptr)
	{
		FREE( m_pszBuffer );
		m_pszBuffer = nullptr;
	}
}

template <typename T>
INLINE CTString<T>::CTString(PCTStr psz, size_t stLen)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	assert(psz != nullptr);
	FillBuffer(psz, stLen);
}

template <typename T>
INLINE CTString<T>::CTString(const CTString<T>& aSrc)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	FillBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
}

template <typename T>
INLINE CTString<T>::CTString(const CTStringRef<T>& aSrc)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	FillBuffer(*aSrc, aSrc.Length());
}

template <typename T>
template <size_t stLenT>
INLINE CTString<T>::CTString(const CTStringFix<T, stLenT>& aSrc)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	FillBuffer(*aSrc, aSrc.Length());
}

#ifdef __CXX11_IMP__
template <typename T>
INLINE CTString<T>::CTString(CTString<T>&& aSrc)
: m_stLen(aSrc.m_stLen)
, m_stBufferLen(aSrc.m_stBufferLen)
, m_pszBuffer(aSrc.m_pszBuffer)
{
	aSrc.m_stLen       = 0;
	aSrc.m_stBufferLen = 0;
	aSrc.m_pszBuffer   = nullptr;
}
#endif

template <typename T>
INLINE CTString<T>& CTString<T>::operator=(PCTStr psz)
{
	assert(psz != nullptr);
	assert(psz != m_pszBuffer);
	FillBuffer(psz);
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator=(const CTString<T>& aSrc)
{
	assert(this != &aSrc);
	if (this != &aSrc)
	{
		FillBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
	}
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator=(const CTStringRef<T>& aSrc)
{
	FillBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T>
template <size_t stLenT>
INLINE CTString<T>& CTString<T>::operator=(const CTStringFix<T, stLenT>& aSrc)
{
	FillBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator+=(TChar ch)
{
	assert(ch != 0);
	AppendBuffer(ch);
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator+=(PCTStr psz)
{
	assert(psz != nullptr);
	AppendBuffer(psz);
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator+=(const CTString<T>& aSrc)
{
	AppendBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
	return (*this);
}

template <typename T>
INLINE CTString<T>& CTString<T>::operator+=(const CTStringRef<T>& aSrc)
{
	AppendBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T>
template <size_t stLenT>
INLINE CTString<T>& CTString<T>::operator+=(const CTStringFix<T, stLenT>& aSrc)
{
	AppendBuffer(*aSrc, aSrc.Length());
	return (*this);
}

template <typename T>
INLINE CTString<T> CTString<T>::operator+(TChar ch)
{
	return (CTString(*this) += ch);
}

template <typename T>
INLINE CTString<T> CTString<T>::operator+(PCTStr psz)
{
	return (CTString(*this) += psz);
}

template <typename T>
INLINE CTString<T> CTString<T>::operator+(const CTString<T>& aSrc)
{
	return (CTString(*this) += aSrc);
}

template <typename T>
INLINE CTString<T> CTString<T>::operator+(const CTStringRef<T>& aSrc)
{
	return (CTString(*this) += aSrc);
}

template <typename T>
INLINE bool CTString<T>::operator<=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) <= 0);
}

template <typename T>
INLINE bool CTString<T>::operator<(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) < 0);
}

template <typename T>
INLINE bool CTString<T>::operator>=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) >= 0);
}

template <typename T>
INLINE bool CTString<T>::operator>(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) > 0);
}

template <typename T>
INLINE bool CTString<T>::operator==(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) == 0);
}

template <typename T>
INLINE bool CTString<T>::operator!=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (T::Compare(m_pszBuffer, psz) != 0);
}

template <typename T>
INLINE bool CTString<T>::operator<=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) <= 0);
}

template <typename T>
INLINE bool CTString<T>::operator<(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) < 0);
}

template <typename T>
INLINE bool CTString<T>::operator>=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) >= 0);
}

template <typename T>
INLINE bool CTString<T>::operator>(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) > 0);
}

template <typename T>
INLINE bool CTString<T>::operator==(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) == 0);
}

template <typename T>
INLINE bool CTString<T>::operator!=(const CTString<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, aSrc.m_pszBuffer) != 0);
}

template <typename T>
INLINE bool CTString<T>::operator<=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) <= 0);
}

template <typename T>
INLINE bool CTString<T>::operator<(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) < 0);
}

template <typename T>
INLINE bool CTString<T>::operator>=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) >= 0);
}

template <typename T>
INLINE bool CTString<T>::operator>(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) > 0);
}

template <typename T>
INLINE bool CTString<T>::operator==(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) == 0);
}

template <typename T>
INLINE bool CTString<T>::operator!=(const CTStringRef<T>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) != 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator<=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) <= 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator<(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) < 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator>=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) >= 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator>(const CTStringFix<T, stLenT>&aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) > 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator==(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) == 0);
}

template <typename T>
template <size_t stLenT>
INLINE bool CTString<T>::operator!=(const CTStringFix<T, stLenT>& aSrc) const
{
	return (T::Compare(m_pszBuffer, *aSrc) != 0);
}

template <typename T>
INLINE typename CTString<T>::TChar CTString<T>::operator[](size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_pszBuffer[stIndex];
	}
	return 0;
}

template <typename T>
INLINE typename CTString<T>::PTStr CTString<T>::operator*(void)
{
	return m_pszBuffer;
}

template <typename T>
INLINE typename CTString<T>::PCTStr CTString<T>::operator*(void)const
{
	return m_pszBuffer;
}

//template <typename T>
//INLINE CTString<T>::operator typename CTString<T>::PCTStr(void) const
//{
//	return m_pszBuffer;
//}

template <typename T>
INLINE typename CTString<T>::PCTStr CTString<T>::GetBuffer(void) const
{
	return m_pszBuffer;
}

template <typename T>
INLINE typename CTString<T>::TChar CTString<T>::GetAt(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_pszBuffer[stIndex];
	}
	return 0;
}

template <typename T>
INLINE bool CTString<T>::SetAt(size_t stIndex, TChar ch)
{
	if ((stIndex < m_stLen) && (ch > 0))
	{
		m_pszBuffer[stIndex] = ch;
		return true;
	}
	return false;
}

template <typename T>
bool CTString<T>::FillBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = (Int)T::Length(psz);
	}
	if (stLen == 0)
	{
		return false;
	}
	++stLen; // add 1 for '\0'
	if (DEF_BASE::Align<size_t>(stLen, (size_t)LMT_KEY) > m_stBufferLen)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(stLen, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	m_stLen = stLen - 1;
	T::Copy(m_pszBuffer, m_stBufferLen, psz, m_stLen);
	m_pszBuffer[m_stLen] = 0;
	return true;
}

template <typename T>
bool CTString<T>::AppendBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = (Int)T::Length(psz);
	}
	if (stLen == 0)
	{
		return false;
	}
	++stLen; // add 1 for '\0'
	if (stLen + m_stLen > m_stBufferLen)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(stLen + m_stLen, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	--stLen;
	T::Copy(m_pszBuffer + m_stLen, (m_stBufferLen - m_stLen), psz, stLen);
	m_stLen += stLen;
	m_pszBuffer[m_stLen] = 0;
	return true;
}

template <typename T>
INLINE bool CTString<T>::AppendBuffer(TChar ch)
{
	if (ch > 0)
	{
		if ((m_stLen + 1) >= m_stBufferLen)
		{
			if (AllocBuffer(DEF_BASE::Align<size_t>((m_stLen + 1 + 1), (size_t)LMT_KEY)) == false) // add 1 for '\0' 
			{
				return false;
			}
		}
		m_pszBuffer[m_stLen] = ch;
		++m_stLen;
		m_pszBuffer[m_stLen] = 0;
	}
	return true;
}

template <typename T>
INLINE bool CTString<T>::SetBufferLength(size_t stLen)
{
	if (stLen == 0)
	{
		return false;
	}
	if (DEF_BASE::Align<size_t>(stLen, (size_t)LMT_KEY) > m_stBufferLen)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(stLen, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	m_pszBuffer[0] = 0;
	m_pszBuffer[stLen - 1] = 0;
	m_stLen = 0;
	return true;
}

template <typename T>
INLINE bool CTString<T>::IsEmpty(void) const
{
	return (m_stLen == 0);
}

template <typename T>
INLINE void CTString<T>::Empty(bool bRelease)
{
	if (m_pszBuffer != nullptr)
	{
		if (bRelease)
		{
			FREE(m_pszBuffer);
			m_pszBuffer = nullptr;
		}
		else
		{
			m_pszBuffer[0] = 0;
		}
	}
	m_stLen = 0;
	m_stBufferLen = 0;
}

template <typename T>
INLINE size_t CTString<T>::ResetLength(void)
{
	if (m_pszBuffer != nullptr)
	{
		m_stLen = T::Length(m_pszBuffer, m_stBufferLen);
		if (m_stLen >= m_stBufferLen)
		{
			m_stLen = 0;
		}
	}
	return m_stLen;
}

template <typename T>
INLINE size_t CTString<T>::Length(bool bStream) const
{
	if (bStream == false)
	{
		return m_stLen;
	}
	else
	{
		return (m_stLen * sizeof(TChar) + sizeof(UInt));
	}
}

template <typename T>
INLINE size_t CTString<T>::BufferLength(void) const
{
	return m_stBufferLen;
}

template <typename T>
INLINE Int CTString<T>::Find(TChar ch, size_t stStart, bool bRev) const
{
	assert(stStart < m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = nullptr;
		if (bRev == false)
		{
			p = T::FindChar(GetBuffer() + stStart, ch);
		}
		else
		{
			p = T::FindCharRev(GetBuffer() + stStart, ch);
		}
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_pszBuffer);
		}
	}
	return -1;
}

template <typename T>
INLINE Int CTString<T>::Find(PCTStr pszSub, size_t stStart) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = T::FindString(GetBuffer() + stStart, pszSub);
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_pszBuffer);
		}
	}
	return -1;
}

template <typename T>
INLINE Int CTString<T>::FindOneOf(PCTStr pszMatch, size_t stStart, bool bInMatch) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		size_t stRet = 0;
		if (bInMatch)
		{
			stRet = T::FindOneIndexOf(GetBuffer() + stStart, pszMatch);
		}
		else
		{
			stRet = T::FindOneIndexNotOf(GetBuffer() + stStart, pszMatch);
		}
		if (stRet < m_stLen)
		{
			return ((Int)stRet);
		}
	}
	return -1;
}

template <typename T>
INLINE Int CTString<T>::Compare(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::Compare(m_pszBuffer, psz);
}

template <typename T>
INLINE Int CTString<T>::CompareIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::CompareIgnore(m_pszBuffer, psz);
}

template <typename T>
INLINE Int CTString<T>::Comparen(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::Comparen(m_pszBuffer, psz, stLen);
}

template <typename T>
INLINE Int CTString<T>::CompareIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::CompareIgnoren(m_pszBuffer, psz, stLen);
}

template <typename T>
INLINE Int CTString<T>::Collate(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::Collate(m_pszBuffer, psz);
}

template <typename T>
INLINE Int CTString<T>::CollateIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return T::CollateIgnore(m_pszBuffer, psz);
}

template <typename T>
INLINE Int CTString<T>::Collaten(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::Collaten(m_pszBuffer, psz, stLen);
}

template <typename T>
INLINE Int CTString<T>::CollateIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = T::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return T::CollateIgnoren(m_pszBuffer, psz, stLen);
}

template <typename T>
INLINE size_t CTString<T>::Delete(size_t stIndex, size_t stCount)
{
	if ((stIndex < m_stLen) && (stCount > 0))
	{
		if ((stIndex + stCount) > m_stLen)
		{
			stCount = m_stLen - stIndex;
		}
		size_t stMove = (m_stLen - (stIndex + stCount) + 1) * sizeof(TChar);
		m_stLen -= stCount;
		MM_SAFE::Memmov((m_pszBuffer + stIndex), stMove, (m_pszBuffer + stIndex + stCount), stMove);
		m_pszBuffer[m_stLen] = 0;
	}
	return m_stLen;
}

template <typename T>
INLINE size_t CTString<T>::Insert(size_t stIndex, TChar ch)
{
	if (ch > 0)
	{
		if ((m_stLen + 1) >= m_stBufferLen)
		{
			if (AllocBuffer(DEF_BASE::Align<size_t>((m_stLen + 1 + 1), (size_t)LMT_KEY)) == false) // add 1 for '\0'
			{
				return m_stLen;
			}
		}
		if (stIndex < m_stLen)
		{
			++m_stLen;
			size_t stMove = (m_stLen - stIndex) * sizeof(TChar); // include 0
			MM_SAFE::Memmov((m_pszBuffer + stIndex + 1), stMove, (m_pszBuffer + stIndex), stMove);
			m_pszBuffer[stIndex] = ch;
		}
		else
		{
			m_pszBuffer[m_stLen] = ch;
			++m_stLen;
			m_pszBuffer[m_stLen] = 0;
		}
	}
	return m_stLen;
}

template <typename T>
INLINE size_t CTString<T>::Insert(size_t stIndex, PCTStr psz)
{
	assert(psz != nullptr);
	size_t stLen = T::Length(psz);
	if (stLen > 0)
	{
		if ((m_stLen + stLen) >= m_stBufferLen)
		{
			if (AllocBuffer(DEF_BASE::Align<size_t>((m_stLen + stLen + 1), (size_t)LMT_KEY)) == false) // add 1 for '\0'
			{
				return m_stLen;
			}
		}
		if (stIndex < m_stLen)
		{
			size_t stMove = (m_stLen - stIndex + 1) * sizeof(TChar); // include 0
			MM_SAFE::Memmov((m_pszBuffer + stIndex + stLen), stMove, (m_pszBuffer + stIndex), stMove);
			MM_SAFE::Memcpy((m_pszBuffer + stIndex), stLen * sizeof(TChar), psz, stLen * sizeof(TChar));

			m_stLen += stLen;
		}
		else
		{
			MM_SAFE::Memcpy((m_pszBuffer + m_stLen), stLen * sizeof(TChar), psz, stLen * sizeof(TChar));
			m_stLen += stLen;
			m_pszBuffer[m_stLen] = 0;
		}
	}
	return m_stLen;
}

template <typename T>
INLINE size_t CTString<T>::Replace(TChar chOld, TChar chNew)
{
	size_t stReplace = 0;
	if ((chOld != chNew) && (chNew > 0))
	{
		for (size_t i = 0; i < m_stLen; ++i)
		{
			if (m_pszBuffer[i] == chOld)
			{
				m_pszBuffer[i] = chNew;
				++stReplace;
			}
		}
	}
	return stReplace;
}

template <typename T>
INLINE size_t CTString<T>::Replace(PCTStr pszOld, PCTStr pszNew)
{
	size_t stOldLen = T::Length(pszOld);
	if (stOldLen > 0)
	{
		size_t stNewLen = T::Length(pszNew);
		if ((stNewLen != stOldLen) || (T::Comparen(pszOld, pszNew, stNewLen) != 0))
		{
			size_t stCount = 0;
			PCTStr pFind   = nullptr;
			for (size_t i = 0; i < m_stLen; )
			{
				pFind = T::FindString((m_pszBuffer + i), pszOld);
				if (pFind != nullptr)
				{
					++stCount;
					i = (pFind - m_pszBuffer) + stOldLen;
				}
				else
				{
					break;
				}
			}

			if (stCount > 0)
			{
///////////////////////////////////////////////////////////////////////////////////////////////////////////
				PTStr  pszBuffer      = nullptr;
				size_t stNewBufferLen = DEF_BASE::Align<size_t>((m_stLen + stNewLen * stCount - stOldLen * stCount + 1), (size_t)LMT_KEY);  // add 1 for '\0'
				if (AllocBuffer(stNewBufferLen, pszBuffer))
				{
					size_t stCopy = 0;
					size_t stLen  = 0;
					for (PCTStr pFindOld  = m_pszBuffer; pFindOld != nullptr; )
					{
						pFind = T::FindString(pFindOld, pszOld);
						if (pFind != nullptr)
						{
							stCopy = (pFind - pFindOld);
							MM_SAFE::Memcpy((pszBuffer + stLen), stCopy * sizeof(TChar), pFindOld, stCopy * sizeof(TChar));
							stLen  += stCopy;
							MM_SAFE::Memcpy((pszBuffer + stLen), stNewLen * sizeof(TChar), pszNew, stNewLen * sizeof(TChar));
							stLen  += stNewLen;
							
							pFindOld = pFind + stOldLen;
						}
						else
						{
							stCopy = (m_stLen - (pFindOld - m_pszBuffer));
							MM_SAFE::Memcpy((pszBuffer + stLen), stCopy * sizeof(TChar), pFindOld, stCopy * sizeof(TChar));
							stLen += stCopy;
							pszBuffer[stLen] = 0;
							Empty(true);
							m_pszBuffer   = pszBuffer;
							m_stLen       = stLen;
							m_stBufferLen = stNewBufferLen;
							break;
						}
					}
					return stCount;
				}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
			} // (stCount > 0)
		} // pszOld <> pszNew
	} // (stOldLen > 0)
	return 0;
}

template <typename T>
INLINE size_t CTString<T>::Remove(TChar ch)
{
	if (ch != 0)
	{
		size_t stIndex = 0;
		for (size_t i = 0; i < m_stLen; ++i)
		{
			if (m_pszBuffer[i] != ch)
			{
				m_pszBuffer[stIndex] = m_pszBuffer[i];
				++stIndex;
			}
		}
		if (stIndex > 0)
		{
			m_pszBuffer[stIndex] = 0;
			m_stLen = stIndex;
		}
	}
	return m_stLen;
}

template <typename T>
INLINE void CTString<T>::Shrink(void)
{
	// add 1 for '\0'
	if (DEF_BASE::Align<size_t>(m_stLen + 1, (size_t)LMT_KEY) < (m_stBufferLen >> 1))
	{
		AllocBuffer(DEF_BASE::Align<size_t>(m_stLen + 1, (size_t)LMT_KEY));
	}
}

template <typename T>
INLINE size_t CTString<T>::Format(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = T::GetFormattedLength(pszFormat, vl); 
	if (nRet > 0)
	{
		++nRet; // add 1 for '\0'
		if (DEF_BASE::Align<size_t>((size_t)nRet, (size_t)LMT_KEY) > m_stBufferLen)
		{
			if (AllocBuffer(DEF_BASE::Align<size_t>((size_t)nRet, (size_t)LMT_KEY)) == false)
			{
				va_end(vl);
				return m_stLen;
			}
		}
		m_stLen = T::VFormat(m_pszBuffer, m_stBufferLen, pszFormat, vl);
		if (m_stLen < 0)
		{
			m_stLen = 0;
		}
	}
	va_end(vl);
	return m_stLen;
}

template <typename T>
INLINE size_t CTString<T>::AppendFormat(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = T::GetFormattedLength(pszFormat, vl); 
	if (nRet > 0)
	{
		++nRet; // add 1 for '\0'
		if (DEF_BASE::Align<size_t>(m_stLen + (size_t)nRet, (size_t)LMT_KEY) > m_stBufferLen)
		{
			if (AllocBuffer(DEF_BASE::Align<size_t>(m_stLen + (size_t)nRet, (size_t)LMT_KEY)) == false)
			{
				va_end(vl);
				return m_stLen;
			}
		}
		nRet = T::VFormat((m_pszBuffer + m_stLen), (m_stBufferLen - m_stLen), pszFormat, vl);
		if (nRet > 0)
		{
			m_stLen += (size_t)nRet;
		}
	}
	va_end(vl);
	return m_stLen;
}

template <typename T>
INLINE void CTString<T>::Upper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_pszBuffer[stIndex] = T::ToUpper(m_pszBuffer[stIndex]);
	}
}

template <typename T>
INLINE void CTString<T>::Lower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_pszBuffer[stIndex] = T::ToLower(m_pszBuffer[stIndex]);
	}
}

template <typename T>
INLINE void CTString<T>::Upper(void)
{
	if (m_stLen > 0)
	{
		T::ToUpper(m_pszBuffer, m_stLen + 1);
	}
}

template <typename T>
INLINE void CTString<T>::Lower(void)
{
	if (m_stLen > 0)
	{
		T::ToLower(m_pszBuffer, m_stLen + 1);
	}
}

template <typename T>
INLINE void CTString<T>::Reverse(void)
{
	if (m_stLen > 0)
	{
		T::Reverse(m_pszBuffer);
	}
}

template <typename T>
INLINE void CTString<T>::TrimLeft(TChar ch)
{
	size_t stTrim = 0;
	for (size_t i = 0; i < m_stLen; ++i)
	{
		if (m_pszBuffer[i] == ch)
		{
			++stTrim;
		}
		else
		{
			assert(stTrim <= m_stLen);
			if (stTrim > 0)
			{
				MM_SAFE::Memmov(m_pszBuffer,          (m_stLen - stTrim) * sizeof(TChar),
						        m_pszBuffer + stTrim, (m_stLen - stTrim) * sizeof(TChar));
				m_stLen -= stTrim;
				m_pszBuffer[m_stLen] = 0;
			}
			break;
		}
	}
}

template <typename T>
INLINE void CTString<T>::TrimRight(TChar ch)
{
	size_t stTrim = m_stLen;
	for (size_t i = m_stLen; i > 0; --i)
	{
		if (m_pszBuffer[i - 1] == ch)
		{
			--stTrim;
		}
		else
		{
			if (stTrim < m_stLen)
			{
				m_pszBuffer[stTrim] = 0;
				m_stLen = stTrim;
			}
			break;
		}
	}
}

template <typename T>
INLINE void CTString<T>::Trim(TChar ch)
{
	TrimRight(ch);
	TrimLeft(ch);
}

template <typename T>
INLINE CTString<T> CTString<T>::Left(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CTString<T>(m_pszBuffer, stCount));
}

template <typename T>
INLINE CTString<T> CTString<T>::Right(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CTString<T>(m_pszBuffer + m_stLen - stCount, stCount));
}

template <typename T>
INLINE CTString<T> CTString<T>::Mid(size_t stStart, size_t stCount) const
{
	assert(stStart <= m_stLen);

	stStart = DEF_BASE::Min<size_t>(stStart, m_stLen);
	stCount = DEF_BASE::Min<size_t>(stCount, (m_stLen - stStart));
	return (CTString<T>(m_pszBuffer + stStart, stCount));
}

template <typename T>
INLINE bool CTString<T>::IsDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsDigit(m_pszBuffer[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::IsXDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsXDigit(m_pszBuffer[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::IsSpace(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsSpace(m_pszBuffer[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::IsLower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsLower(m_pszBuffer[stIndex]);
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::IsUpper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return T::IsUpper(m_pszBuffer[stIndex]);
	}
	return false;
}

template <typename T>
INLINE Int CTString<T>::ToInt(void) const
{
	if (m_stLen > 0)
	{
		return T::ToInt(m_pszBuffer);
	}
	return 0;
}

template <typename T>
INLINE Long CTString<T>::ToLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE ULong CTString<T>::ToULong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE LLong CTString<T>::ToLLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToLLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE ULLong CTString<T>::ToULLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return T::ToULLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

template <typename T>
INLINE Double CTString<T>::ToDouble(void) const
{
	if (m_stLen > 0)
	{
		return T::ToDouble(m_pszBuffer, nullptr);
	}
	return 0;
}

template <typename T>
INLINE Long CTString<T>::ToLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd = nullptr;
		Long  lRet = T::ToLong((m_pszBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return lRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE ULong CTString<T>::ToULong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		ULong ulRet = T::ToULong((m_pszBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ulRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE LLong CTString<T>::ToLLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		LLong llRet = T::ToLLong((m_pszBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return llRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE ULLong CTString<T>::ToULLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd   = nullptr;
		ULLong ullRet = T::ToULLong((m_pszBuffer + stIndex), &pEnd, nRadix);
		if ((pEnd != (m_pszBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return ullRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE Double CTString<T>::ToDouble(size_t& stIndex)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd = nullptr;
		Double dRet = T::ToDouble((m_pszBuffer + stIndex), &pEnd);
		if ((pEnd != (m_pszBuffer + stIndex)) && (*pEnd != 0))
		{
			stIndex = (pEnd - m_pszBuffer);
		}
		else
		{
			stIndex = (size_t)-1;
		}
		return dRet;
	}
	stIndex = (size_t)-1;
	return 0;
}

template <typename T>
INLINE bool CTString<T>::ToString(Int nValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(nValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::ToString(Long lValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(lValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::ToString(ULong ulValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(ulValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::ToString(LLong llValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(llValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::ToString(ULLong ullValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(ullValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
INLINE bool CTString<T>::ToString(Double dValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (T::ToString(dValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

template <typename T>
bool CTString<T>::AllocBuffer(size_t stBufferLen)
{
	if (m_pszBuffer == nullptr)
	{
		m_pszBuffer = reinterpret_cast<PTStr>(ALLOC( stBufferLen * sizeof(TChar) ));
		if (m_pszBuffer == nullptr)
		{
			return false;
		}
		m_stBufferLen = stBufferLen;
	}
	else
	{
		PTStr p = reinterpret_cast<PTStr>(REALLOC( m_pszBuffer, stBufferLen * sizeof(TChar) ));
		if (p == nullptr)
		{
			return false;
		}
		m_pszBuffer   = p;
		m_stBufferLen = stBufferLen;
	}
	return true;
}

template <typename T>
bool CTString<T>::AllocBuffer(size_t stBufferLen, PTStr& pszBuffer)
{
	pszBuffer = reinterpret_cast<PTStr>(ALLOC( stBufferLen * sizeof(TChar) ));
	return (pszBuffer != nullptr);
}

#endif // __TSTRING_INL__