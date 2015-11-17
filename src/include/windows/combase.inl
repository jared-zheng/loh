///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : combase.inl                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows com base                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __COM_BASE_INL__
#define __COM_BASE_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComPtr
template <typename T>
INLINE CComPtr<T>::CComPtr(void)
: m_pPtr(nullptr)
{
}

template <typename T>
INLINE CComPtr<T>::~CComPtr(void)
{
	if (m_pPtr != nullptr)
	{
		m_pPtr->Release();
	}
}

template <typename T>
INLINE CComPtr<T>::CComPtr(T* pT, bool bAddRef)
: m_pPtr(pT)
{
	if (bAddRef && (m_pPtr != nullptr))
	{
		m_pPtr->AddRef();
	}
}

template <typename T>
INLINE CComPtr<T>::CComPtr(const CComPtr<T>& aSrc)
: m_pPtr(aSrc.m_pPtr)
{
	if (m_pPtr != nullptr)
	{
		m_pPtr->AddRef();
	}
}

template <typename T>
INLINE CComPtr<T>& CComPtr<T>::operator=(const CComPtr<T>& aSrc)
{
	if (&aSrc != this)
	{
		if (m_pPtr != nullptr)
		{
			m_pPtr->Release();
		}
		m_pPtr =  aSrc.m_pPtr;
		if (m_pPtr != nullptr)
		{
			m_pPtr->AddRef();
		}
	}
	return (*this);
}

template <typename T>
INLINE CComPtr<T>& CComPtr<T>::operator=(T* pT)
{
	if (pT != m_pPtr)
	{
		if (m_pPtr != nullptr)
		{
			m_pPtr->Release();
		}
		m_pPtr =  pT;
		if (m_pPtr != nullptr)
		{
			m_pPtr->AddRef();
		}
	}
	return (*this);
}

template <typename T>
INLINE CComPtr<T>::operator T*(void) const
{
	return m_pPtr;
}

template <typename T>
INLINE T& CComPtr<T>::operator*(void) const
{
	return (*m_pPtr);
}

template <typename T>
INLINE T* CComPtr<T>::operator->(void) const
{
	return m_pPtr;
}

template <typename T>
INLINE T* CComPtr<T>::Get(void) const
{
	return m_pPtr;
}

template <typename T>
INLINE bool CComPtr<T>::operator==(T* pT) const
{
	return (m_pPtr == pT);
}

template <typename T>
INLINE bool CComPtr<T>::operator!=(T* pT) const
{
	return (m_pPtr != pT);
}

template <typename T>
INLINE bool CComPtr<T>::operator==(const CComPtr<T>& aSrc) const
{
	return (m_pPtr == aSrc.m_pPtr);
}

template <typename T>
INLINE bool CComPtr<T>::operator!=(const CComPtr<T>& aSrc) const
{
	return (m_pPtr != aSrc.m_pPtr);
}
// Attach to an existing interface (does not AddRef)
template <typename T>
INLINE void CComPtr<T>::Attach(T* pT)
{
	if (m_pPtr != nullptr)
	{
		m_pPtr->Release();
	}
	m_pPtr = pT;
}
// Detach the interface (does not Release)
template <typename T>
INLINE T* CComPtr<T>::Detach(void)
{
	T* pT  = m_pPtr;
	m_pPtr = nullptr;
	return pT;
}

template <typename T>
template <typename X>
INLINE Long CComPtr<T>::QueryInterface(X** ppX) const
{
	assert(m_pPtr != nullptr);
	assert(ppX != nullptr);
	return m_pPtr->QueryInterface(__uuidof(X), (void**)ppX);
}

template <typename T>
INLINE void CComPtr<T>::Release(void)
{
	T* pT = m_pPtr;
	if (pT != nullptr)
	{
		m_pPtr = nullptr;
		pT->Release();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComQIPtr
template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>::CComQIPtr(void)
{
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>::~CComQIPtr(void)
{
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>::CComQIPtr(T* pT, bool bAddRef)
: CComPtr<T>(pT, bAddRef)
{
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>::CComQIPtr(IUnknown* pUnk)
{
	if (pUnk != nullptr)
	{
		pUnk->QueryInterface(*puid, (void**)&m_pPtr);
	}
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>::CComQIPtr(const CComQIPtr<T, puid>& aSrc)
: CComPtr<T>(aSrc.m_pPtr)
{
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>& CComQIPtr<T, puid>::operator=(const CComQIPtr<T, puid>& aSrc)
{
	if (&aSrc != this)
	{
		if (m_pPtr != nullptr)
		{
			m_pPtr->Release();
		}
		m_pPtr =  aSrc.m_pPtr;
		if (m_pPtr != nullptr)
		{
			m_pPtr->AddRef();
		}
	}
	return (*this);
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>& CComQIPtr<T, puid>::operator=(T* pT)
{
	if (pT != m_pPtr)
	{
		if (m_pPtr != nullptr)
		{
			m_pPtr->Release();
		}
		m_pPtr =  pT;
		if (m_pPtr != nullptr)
		{
			m_pPtr->AddRef();
		}
	}
	return (*this);
}

template <typename T, const PUID puid>
INLINE CComQIPtr<T, puid>& CComQIPtr<T, puid>::operator=(IUnknown* pUnk)
{
	if (&aSrc != this)
	{
		if (m_pPtr != nullptr)
		{
			m_pPtr->Release();
			m_pPtr = nullptr;
		}
		if (pUnk != nullptr)
		{
			pUnk->QueryInterface(*puid, (void**)&m_pPtr);
		}
	}
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComStr
INLINE CComStr::CComStr(void)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
}

INLINE CComStr::~CComStr(void)
{
	if (m_pszBuffer != nullptr)
	{
		::SysFreeString(m_pszBuffer);
		m_pszBuffer = nullptr;
	}
}

INLINE CComStr::CComStr(PCTStr psz, size_t stLen)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	assert(psz != nullptr);
	FillBuffer(psz, stLen);
}

INLINE CComStr::CComStr(const CComStr& aSrc)
: m_stLen(0)
, m_stBufferLen(0)
, m_pszBuffer(nullptr)
{
	FillBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
}

#ifdef __CXX11_IMP__
template <typename T>
INLINE CComStr::CComStr(CComStr&& aSrc)
: m_stLen(aSrc.m_stLen)
, m_stBufferLen(aSrc.m_stBufferLen)
, m_pszBuffer(aSrc.m_pszBuffer)
{
	aSrc.m_stLen = 0;
	aSrc.m_stBufferLen = 0;
	aSrc.m_pszBuffer = nullptr;
}
#endif

INLINE CComStr& CComStr::operator=(PCTStr psz)
{
	assert(psz != nullptr);
	assert(psz != m_pszBuffer);
	FillBuffer(psz);
	return (*this);
}

INLINE CComStr& CComStr::operator=(const CComStr& aSrc)
{
	assert(this != &aSrc);
	if (this != &aSrc)
	{
		FillBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
	}
	return (*this);
}

INLINE CComStr& CComStr::operator+=(TChar ch)
{
	assert(ch != 0);
	AppendBuffer(ch);
	return (*this);
}

INLINE CComStr& CComStr::operator+=(PCTStr psz)
{
	assert(psz != nullptr);
	AppendBuffer(psz);
	return (*this);
}

INLINE CComStr& CComStr::operator+=(const CComStr& aSrc)
{
	AppendBuffer(aSrc.m_pszBuffer, aSrc.m_stLen);
	return (*this);
}

INLINE CComStr CComStr::operator+(TChar ch)
{
	return (CComStr(*this) += ch);
}

INLINE CComStr CComStr::operator+(PCTStr psz)
{
	return (CComStr(*this) += psz);
}

INLINE CComStr CComStr::operator+(const CComStr& aSrc)
{
	return (CComStr(*this) += aSrc);
}

INLINE bool CComStr::operator<=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) <= 0);
}

INLINE bool CComStr::operator<(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) < 0);
}

INLINE bool CComStr::operator>=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) >= 0);
}

INLINE bool CComStr::operator>(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) > 0);
}

INLINE bool CComStr::operator==(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) == 0);
}

INLINE bool CComStr::operator!=(PCTStr psz) const
{
	assert(psz != nullptr);
	return (CWChar::Compare(m_pszBuffer, psz) != 0);
}

INLINE bool CComStr::operator<=(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) <= 0);
}

INLINE bool CComStr::operator<(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) < 0);
}

INLINE bool CComStr::operator>=(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) >= 0);
}

INLINE bool CComStr::operator>(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) > 0);
}

INLINE bool CComStr::operator==(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) == 0);
}

INLINE bool CComStr::operator!=(const CComStr& aSrc) const
{
	return (CWChar::Compare(m_pszBuffer, aSrc.m_pszBuffer) != 0);
}

INLINE typename CComStr::TChar CComStr::operator[](size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_pszBuffer[stIndex];
	}
	return 0;
}

INLINE typename CComStr::PTStr CComStr::operator*(void)
{
	return m_pszBuffer;
}

INLINE typename CComStr::PCTStr CComStr::operator*(void)const
{
	return m_pszBuffer;
}

INLINE void CComStr::Attach(PCTStr psz)
{
	if (m_pszBuffer != psz)
	{
		::SysFreeString(m_pszBuffer);
		FillBuffer(psz);
	}
}

INLINE CComStr::PTStr CComStr::Detach(void)
{
	PTStr p = m_pszBuffer; 
	m_pszBuffer = nullptr;
	return p;
}

INLINE CComStr::PTStr CComStr::Copy(void) const
{
	if (m_stBufferLen > 0)
	{
		return (PTStr)::SysAllocStringLen(m_pszBuffer, m_stBufferLen);
	}
	return nullptr;
}

INLINE Long CComStr::CopyTo(PTStr& pszRef) const
{
	pszRef = Copy();
	if ((pszRef == nullptr) && (m_pszBuffer != nullptr))
	{
		return E_OUTOFMEMORY;
	}
	return S_OK;
}

INLINE Long CComStr::CopyToCopyTo(VARIANT* pVar) const
{
	assert(pVar != nullptr);
	if (pVar != nullptr)
	{
		pVar->vt      = VT_BSTR;
		pVar->bstrVal = Copy();
		if ((pVar->bstrVal == nullptr) && (m_pszBuffer != nullptr))
		{
			return E_OUTOFMEMORY;
		}
		return S_OK;
	}
	return E_POINTER;
}

INLINE typename CComStr::PCTStr CComStr::GetBuffer(void) const
{
	return m_pszBuffer;
}

INLINE typename CComStr::TChar CComStr::GetAt(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return m_pszBuffer[stIndex];
	}
	return 0;
}

INLINE bool CComStr::SetAt(size_t stIndex, TChar ch)
{
	if ((stIndex < m_stLen) && (ch > 0))
	{
		m_pszBuffer[stIndex] = ch;
		return true;
	}
	return false;
}

bool CComStr::FillBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = (Int)CWChar::Length(psz);
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
	CWChar::Copy(m_pszBuffer, m_stBufferLen, psz, m_stLen);
	m_pszBuffer[m_stLen] = 0;
	return true;
}

bool CComStr::AppendBuffer(PCTStr psz, size_t stLen)
{
	if (psz == nullptr)
	{
		return false;
	}
	if (stLen == 0)
	{
		stLen = (Int)CWChar::Length(psz);
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
	CWChar::Copy(m_pszBuffer + m_stLen, (m_stBufferLen - m_stLen), psz, stLen);
	m_stLen += stLen;
	m_pszBuffer[m_stLen] = 0;
	return true;
}

INLINE bool CComStr::AppendBuffer(TChar ch)
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

INLINE bool CComStr::SetBufferLength(size_t stLen)
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

INLINE bool CComStr::IsEmpty(void) const
{
	return (m_stLen == 0);
}

INLINE void CComStr::Empty(bool bRelease)
{
	if (m_pszBuffer != nullptr)
	{
		if (bRelease)
		{
			::SysFreeString(m_pszBuffer);
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

INLINE size_t CComStr::ResetLength(void)
{
	if (m_pszBuffer != nullptr)
	{
		m_stLen = CWChar::Length(m_pszBuffer, m_stBufferLen);
		if (m_stLen >= m_stBufferLen)
		{
			m_stLen = 0;
		}
	}
	return m_stLen;
}

INLINE size_t CComStr::Length(bool bStream) const
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

INLINE size_t CComStr::BufferLength(void) const
{
	return m_stBufferLen;
}

INLINE Int CComStr::Find(TChar ch, size_t stStart, bool bRev) const
{
	assert(stStart < m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = nullptr;
		if (bRev == false)
		{
			p = CWChar::FindChar(GetBuffer() + stStart, ch);
		}
		else
		{
			p = CWChar::FindCharRev(GetBuffer() + stStart, ch);
		}
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_pszBuffer);
		}
	}
	return -1;
}

INLINE Int CComStr::Find(PCTStr pszSub, size_t stStart) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		PCTStr p = CWChar::FindString(GetBuffer() + stStart, pszSub);
		if (p != nullptr)
		{
			return (Int)(intptr_t)(p - m_pszBuffer);
		}
	}
	return -1;
}

INLINE Int CComStr::FindOneOf(PCTStr pszMatch, size_t stStart, bool bInMatch) const
{
	assert(stStart <= m_stLen);
	if (stStart < m_stLen)
	{
		size_t stRet = 0;
		if (bInMatch)
		{
			stRet = CWChar::FindOneIndexOf(GetBuffer() + stStart, pszMatch);
		}
		else
		{
			stRet = CWChar::FindOneIndexNotOf(GetBuffer() + stStart, pszMatch);
		}
		if (stRet < m_stLen)
		{
			return ((Int)stRet);
		}
	}
	return -1;
}

INLINE Int CComStr::Compare(PCTStr psz) const
{
	assert(psz != nullptr);
	return CWChar::Compare(m_pszBuffer, psz);
}

INLINE Int CComStr::CompareIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return CWChar::CompareIgnore(m_pszBuffer, psz);
}

INLINE Int CComStr::Comparen(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = CWChar::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return CWChar::Comparen(m_pszBuffer, psz, stLen);
}

INLINE Int CComStr::CompareIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = CWChar::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return CWChar::CompareIgnoren(m_pszBuffer, psz, stLen);
}

INLINE Int CComStr::Collate(PCTStr psz) const
{
	assert(psz != nullptr);
	return CWChar::Collate(m_pszBuffer, psz);
}

INLINE Int CComStr::CollateIgnore(PCTStr psz) const
{
	assert(psz != nullptr);
	return CWChar::CollateIgnore(m_pszBuffer, psz);
}

INLINE Int CComStr::Collaten(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = CWChar::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return CWChar::Collaten(m_pszBuffer, psz, stLen);
}

INLINE Int CComStr::CollateIgnoren(PCTStr psz, size_t stLen) const
{
	assert(psz != nullptr);
	if (stLen == 0)
	{
		stLen = CWChar::Length(psz);
	}
	stLen = DEF_BASE::Max<size_t>(stLen, m_stLen);
	return CWChar::CollateIgnoren(m_pszBuffer, psz, stLen);
}

INLINE size_t CComStr::Delete(size_t stIndex, size_t stCount)
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

INLINE size_t CComStr::Insert(size_t stIndex, TChar ch)
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

INLINE size_t CComStr::Insert(size_t stIndex, PCTStr psz)
{
	assert(psz != nullptr);
	size_t stLen = CWChar::Length(psz);
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

INLINE size_t CComStr::Replace(TChar chOld, TChar chNew)
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

INLINE size_t CComStr::Replace(PCTStr pszOld, PCTStr pszNew)
{
	size_t stOldLen = CWChar::Length(pszOld);
	if (stOldLen > 0)
	{
		size_t stNewLen = CWChar::Length(pszNew);
		if ((stNewLen != stOldLen) || (CWChar::Comparen(pszOld, pszNew, stNewLen) != 0))
		{
			size_t stCount = 0;
			PCTStr pFind   = nullptr;
			for (size_t i = 0; i < m_stLen; )
			{
				pFind = CWChar::FindString((m_pszBuffer + i), pszOld);
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
						pFind = CWChar::FindString(pFindOld, pszOld);
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

INLINE size_t CComStr::Remove(TChar ch)
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

INLINE void CComStr::Shrink(void)
{
	// add 1 for '\0'
	if (DEF_BASE::Align<size_t>(m_stLen + 1, (size_t)LMT_KEY) < (m_stBufferLen >> 1))
	{
		AllocBuffer(DEF_BASE::Align<size_t>(m_stLen + 1, (size_t)LMT_KEY));
	}
}

INLINE size_t CComStr::Load(UInt uId, LibRef lrRef)
{
	if (m_stBufferLen > 0)
	{
		m_stLen = (size_t)::LoadString(lrRef, uId, m_pszBuffer, m_stBufferLen);
		return m_stLen;
	}
	return 0;
}

INLINE size_t CComStr::Format(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = CWChar::GetFormattedLength(pszFormat, vl); 
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
		m_stLen = CWChar::VFormat(m_pszBuffer, m_stBufferLen, pszFormat, vl);
		if (m_stLen < 0)
		{
			m_stLen = 0;
		}
	}
	va_end(vl);
	return m_stLen;
}

INLINE size_t CComStr::AppendFormat(PCTStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	Int nRet = CWChar::GetFormattedLength(pszFormat, vl); 
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
		nRet = CWChar::VFormat((m_pszBuffer + m_stLen), (m_stBufferLen - m_stLen), pszFormat, vl);
		if (nRet > 0)
		{
			m_stLen += (size_t)nRet;
		}
	}
	va_end(vl);
	return m_stLen;
}

INLINE void CComStr::Upper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_pszBuffer[stIndex] = CWChar::ToUpper(m_pszBuffer[stIndex]);
	}
}

INLINE void CComStr::Lower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		m_pszBuffer[stIndex] = CWChar::ToLower(m_pszBuffer[stIndex]);
	}
}

INLINE void CComStr::Upper(void)
{
	if (m_stLen > 0)
	{
		CWChar::ToUpper(m_pszBuffer, m_stLen + 1);
	}
}

INLINE void CComStr::Lower(void)
{
	if (m_stLen > 0)
	{
		CWChar::ToLower(m_pszBuffer, m_stLen + 1);
	}
}

INLINE void CComStr::Reverse(void)
{
	if (m_stLen > 0)
	{
		CWChar::Reverse(m_pszBuffer);
	}
}

INLINE void CComStr::TrimLeft(TChar ch)
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

INLINE void CComStr::TrimRight(TChar ch)
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

INLINE void CComStr::Trim(TChar ch)
{
	TrimRight(ch);
	TrimLeft(ch);
}

INLINE CComStr CComStr::Left(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CComStr(m_pszBuffer, stCount));
}

INLINE CComStr CComStr::Right(size_t stCount) const
{
	assert(stCount > 0);
	stCount = DEF_BASE::Min<size_t>(stCount, m_stLen);
	return (CComStr(m_pszBuffer + m_stLen - stCount, stCount));
}

INLINE CComStr CComStr::Mid(size_t stStart, size_t stCount) const
{
	assert(stStart <= m_stLen);

	stStart = DEF_BASE::Min<size_t>(stStart, m_stLen);
	stCount = DEF_BASE::Min<size_t>(stCount, (m_stLen - stStart));
	return (CComStr(m_pszBuffer + stStart, stCount));
}

INLINE bool CComStr::IsDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return CWChar::IsDigit(m_pszBuffer[stIndex]);
	}
	return false;
}

INLINE bool CComStr::IsXDigit(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return CWChar::IsXDigit(m_pszBuffer[stIndex]);
	}
	return false;
}

INLINE bool CComStr::IsSpace(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return CWChar::IsSpace(m_pszBuffer[stIndex]);
	}
	return false;
}

INLINE bool CComStr::IsLower(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return CWChar::IsLower(m_pszBuffer[stIndex]);
	}
	return false;
}

INLINE bool CComStr::IsUpper(size_t stIndex)
{
	if (stIndex < m_stLen)
	{
		return CWChar::IsUpper(m_pszBuffer[stIndex]);
	}
	return false;
}

INLINE Int CComStr::ToInt(void) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToInt(m_pszBuffer);
	}
	return 0;
}

INLINE Long CComStr::ToLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

INLINE ULong CComStr::ToULong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToULong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

INLINE LLong CComStr::ToLLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToLLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

INLINE ULLong CComStr::ToULLong(Int nRadix) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToULLong(m_pszBuffer, nullptr, nRadix);
	}
	return 0;
}

INLINE Double CComStr::ToDouble(void) const
{
	if (m_stLen > 0)
	{
		return CWChar::ToDouble(m_pszBuffer, nullptr);
	}
	return 0;
}

INLINE Long CComStr::ToLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd = nullptr;
		Long  lRet = CWChar::ToLong((m_pszBuffer + stIndex), &pEnd, nRadix);
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

INLINE ULong CComStr::ToULong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		ULong ulRet = CWChar::ToULong((m_pszBuffer + stIndex), &pEnd, nRadix);
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

INLINE LLong CComStr::ToLLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr pEnd  = nullptr;
		LLong llRet = CWChar::ToLLong((m_pszBuffer + stIndex), &pEnd, nRadix);
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

INLINE ULLong CComStr::ToULLong(size_t& stIndex, Int nRadix)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd   = nullptr;
		ULLong ullRet = CWChar::ToULLong((m_pszBuffer + stIndex), &pEnd, nRadix);
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

INLINE Double CComStr::ToDouble(size_t& stIndex)
{
	if (stIndex < m_stLen)
	{
		PTStr  pEnd = nullptr;
		Double dRet = CWChar::ToDouble((m_pszBuffer + stIndex), &pEnd);
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

INLINE bool CComStr::ToString(Int nValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(nValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

INLINE bool CComStr::ToString(Long lValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(lValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

INLINE bool CComStr::ToString(ULong ulValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(ulValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

INLINE bool CComStr::ToString(LLong llValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(llValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

INLINE bool CComStr::ToString(ULLong ullValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(ullValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

INLINE bool CComStr::ToString(Double dValue, Int nRadix)
{
	if (m_stBufferLen < (size_t)LMT_KEY)
	{
		if (AllocBuffer(DEF_BASE::Align<size_t>(m_stBufferLen + (size_t)LMT_KEY, (size_t)LMT_KEY)) == false)
		{
			return false;
		}
	}
	if (CWChar::ToString(dValue, m_pszBuffer, m_stBufferLen, nRadix))
	{
		ResetLength();
		return true;
	}
	return false;
}

bool CComStr::AllocBuffer(size_t stBufferLen)
{
	if (m_pszBuffer == nullptr)
	{
		m_pszBuffer = reinterpret_cast<PTStr>(::SysAllocStringLen(nullptr, stBufferLen));
		if (m_pszBuffer == nullptr)
		{
			return false;
		}
		m_stBufferLen = stBufferLen;
	}
	else
	{
		if (::SysReAllocStringLen( m_pszBuffer, nullptr, stBufferLen) == FALSE)
		{
			return false;
		}
		m_stBufferLen = stBufferLen;
	}
	return true;
}

template <typename T>
bool CComStr::AllocBuffer(size_t stBufferLen, PTStr& pszBuffer)
{
	pszBuffer = reinterpret_cast<PTStr>(::SysAllocStringLen(nullptr, stBufferLen));
	return (pszBuffer != nullptr);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComVar
INLINE CComVar::CComVar(void)
{
	::VariantInit(this);
}

INLINE CComVar::~CComVar(void)
{
	Clear();
}

INLINE CComVar::CComVar(const CComVar& aSrc)
{
	vt = VT_EMPTY;
	CopyScode(&aSrc);
}

INLINE CComVar::CComVar(const VARIANT& aSrc)
{
	vt = VT_EMPTY;
	CopyScode(&aSrc);
}

INLINE CComVar::CComVar(Char c)
{
	vt   = VT_I1;
	cVal = c;
}

INLINE CComVar::CComVar(Short s)
{
	vt   = VT_I2;
	iVal = s;
}

INLINE CComVar::CComVar(Int n, VARTYPE vtType)
{
	assert((vtType == VT_I4) || (vtType == VT_INT));
	vt     = vtType;
	intVal = n;
}

INLINE CComVar::CComVar(Long l, VARTYPE vtType)
{
	assert((vtType == VT_I4) || (vtType == VT_ERROR));
	vt   = vtType;
	lVal = l;
}

INLINE CComVar::CComVar(LLong ll)
{
	vt    = VT_I8;
	llVal = ll;
}

INLINE CComVar::CComVar(UChar uc)
{
	vt   = VT_UI1;
	bVal = uc;
}

INLINE CComVar::CComVar(UShort us)
{
	vt    = VT_UI2;
	uiVal = us;
}

INLINE CComVar::CComVar(UInt u, VARTYPE vtType)
{
	assert((vtType == VT_UI4) || (vtType == VT_UINT));
	vt      = vtType;
	uintVal = u;
}

INLINE CComVar::CComVar(ULong  ul)
{
	vt    = VT_UI4;
	ulVal = ul;
}

INLINE CComVar::CComVar(ULLong ull)
{
	vt     = VT_UI8;
	ullVal = ull;
}

INLINE CComVar::CComVar(Float f)
{
	vt     = VT_R4;
	fltVal = f;
}

INLINE CComVar::CComVar(Double d, VARTYPE vtType)
{
	assert((vtType == VT_R8) || (vtType == VT_DATE));
	vt     = vtType;
	dblVal = d;
}

INLINE CComVar::CComVar(bool b)
{
	vt      = VT_BOOL;
	boolVal = b ? VARIANT_TRUE : VARIANT_FALSE;
}

INLINE CComVar::CComVar(CY cy)
{
	vt       = VT_CY;
	cyVal.Hi = cy.Hi;
	cyVal.Lo = cy.Lo;
}

INLINE CComVar::CComVar(PCCStr pszC)
{
	vt    = VT_EMPTY;
	*this = pszC;
}

INLINE CComVar::CComVar(PCWStr pszW)
{
	vt    = VT_EMPTY;
	*this = pszW;
}

INLINE CComVar::CComVar(const CComStr& str)
{
	vt    = VT_EMPTY;
	*this = str;
}

INLINE CComVar::CComVar(IDispatch* p)
{
	vt       = VT_DISPATCH;
	pdispVal = p;
	// Need to AddRef as VariantClear will Release
	if (pdispVal != nullptr)
	{
		pdispVal->AddRef();
	}
}

INLINE CComVar::CComVar(IUnknown*  p)
{
	vt      = VT_UNKNOWN;
	punkVal = p;
	// Need to AddRef as VariantClear will Release
	if (punkVal != nullptr)
	{
		punkVal->AddRef();
	}
}

INLINE CComVar::CComVar(const SAFEARRAY* p)
{
	LPSAFEARRAY pCopy = nullptr;
	if (p != nullptr)
	{
		Long lRet = ::SafeArrayCopy((LPSAFEARRAY)p, &pCopy);
		if (SUCCEEDED(lRet) && (pCopy != nullptr))
		{
			lRet = ::SafeArrayGetVartype(p, &vt);
			if (SUCCEEDED(lRet))
			{
				// When Windows has a SAFEARRAY of type VT_DISPATCH with FADF_HAVEIID,
				// it returns VT_UNKNOWN instead of VT_DISPATCH. We patch the value to be correct
				if ((vt == VT_UNKNOWN) && ((p->fFeatures & FADF_HAVEIID) != 0) && ((p->fFeatures & FADF_DISPATCH) != 0))
				{
					vt = VT_DISPATCH;
				}
				vt    |= VT_ARRAY;
				parray = pCopy;
			}
			else
			{
				vt    = VT_ERROR;
				scode = lRet;
			}
		}
		else
		{
			vt    = VT_ERROR;
			scode = lRet;
		}
	}
	else
	{
		vt = VT_EMPTY;
	}
}

INLINE CComVar& CComVar::operator=(const CComVar& aSrc)
{
	if (&aSrc != this)
	{
		CopyScode(&aSrc);
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(const VARIANT& aSrc)
{
	if(&aSrc != static_cast<VARIANT*>(this))
	{
		CopyScode(&aSrc);
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(Char c)
{
	if (vt != VT_I1)
	{
		Clear();
		vt = VT_I1;
	}
	cVal = c;
	return (*this);
}

INLINE CComVar& CComVar::operator=(Short s)
{
	if (vt != VT_I2)
	{
		Clear();
		vt = VT_I2;
	}
	iVal = s;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PShort ps)
{
	if (vt != (VT_I2|VT_BYREF))
	{
		Clear();
		vt = VT_I2|VT_BYREF;
	}
	piVal = ps;
	return (*this);
}

INLINE CComVar& CComVar::operator=(Int n)
{
	if (vt != VT_I4)
	{
		Clear();
		vt = VT_I4;
	}
	intVal = n;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PInt pn)
{
	if (vt != (VT_I4|VT_BYREF))
	{
		Clear();
		vt = VT_I4|VT_BYREF;
	}
	pintVal = pn;
	return (*this);
}

INLINE CComVar& CComVar::operator=(Long l)
{
	if (vt != VT_I4)
	{
		Clear();
		vt = VT_I4;
	}
	lVal = l;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PLong pl)
{
	if (vt != (VT_I4|VT_BYREF))
	{
		Clear();
		vt = VT_I4|VT_BYREF;
	}
	plVal = pl;
	return (*this);
}

INLINE CComVar& CComVar::operator=(LLong ll)
{
	if (vt != VT_I8)
	{
		Clear();
		vt = VT_I8;
	}
	llVal = ll;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PLLong pll)
{
	if (vt != (VT_I8|VT_BYREF))
	{
		Clear();
		vt = VT_I8|VT_BYREF;
	}
	pllVal = pll;
	return (*this);
}

INLINE CComVar& CComVar::operator=(UChar uc)
{
	if (vt != VT_UI1)
	{
		Clear();
		vt = VT_UI1;
	}
	bVal = uc;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PUChar puc)
{
	if (vt != (VT_UI1|VT_BYREF))
	{
		Clear();
		vt = VT_UI1|VT_BYREF;
	}
	pbVal = puc;
	return (*this);
}

INLINE CComVar& CComVar::operator=(UShort us)
{
	if (vt != VT_UI2)
	{
		Clear();
		vt = VT_UI2;
	}
	uiVal = us;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PUShort pus)
{
	if (vt != (VT_UI2|VT_BYREF))
	{
		Clear();
		vt = VT_UI2|VT_BYREF;
	}
	puiVal = pus;
	return (*this);
}

INLINE CComVar& CComVar::operator=(UInt u)
{
	if (vt != VT_UI4)
	{
		Clear();
		vt = VT_UI4;
	}
	uintVal= u;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PUInt pu)
{
	if (vt != (VT_UI4|VT_BYREF))
	{
		Clear();
		vt = VT_UI4|VT_BYREF;
	}
	puintVal = pu;
	return (*this);
}

INLINE CComVar& CComVar::operator=(ULong ul)
{
	if (vt != VT_UI4)
	{
		Clear();
		vt = VT_UI4;
	}
	ulVal = ul;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PULong pul)
{
	if (vt != (VT_UI4|VT_BYREF))
	{
		Clear();
		vt = VT_UI4|VT_BYREF;
	}
	pulVal = pul;
	return (*this);
}

INLINE CComVar& CComVar::operator=(ULLong ull)
{
	if (vt != VT_UI8)
	{
		Clear();
		vt = VT_UI8;
	}
	ullVal = ull;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PULLong pull)
{
	if (vt != (VT_UI8|VT_BYREF))
	{
		Clear();
		vt = VT_UI8|VT_BYREF;
	}
	pullVal = pull;
	return (*this);
}

INLINE CComVar& CComVar::operator=(Float f)
{
	if (vt != VT_R4)
	{
		Clear();
		vt = VT_R4;
	}
	fltVal = f;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PFloat pf)
{
	if (vt != (VT_R4|VT_BYREF))
	{
		Clear();
		vt = VT_R4|VT_BYREF;
	}
	pfltVal = pf;
	return (*this);
}

INLINE CComVar& CComVar::operator=(Double d)
{
	if (vt != VT_R8)
	{
		Clear();
		vt = VT_R8;
	}
	dblVal = d;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PDouble pd)
{
	if (vt != (VT_R8|VT_BYREF))
	{
		Clear();
		vt = VT_R8|VT_BYREF;
	}
	pdblVal = pd;
	return (*this);
}

INLINE CComVar& CComVar::operator=(bool b)
{
	if (vt != VT_BOOL)
	{
		Clear();
		vt = VT_BOOL;
	}
	boolVal = b ? VARIANT_TRUE : VARIANT_FALSE;
	return (*this);
}

INLINE CComVar& CComVar::operator=(CY cy)
{
	if (vt != VT_CY)
	{
		Clear();
		vt = VT_CY;
	}
	cyVal.Hi = cy.Hi;
	cyVal.Lo = cy.Lo;
	return (*this);
}

INLINE CComVar& CComVar::operator=(PCCStr pszC)
{
	Int nLen = CComStr::Convert(pszC, -1, nullptr, 0);
	if (nLen > 0)
	{
		CComStr str;
		str.SetBufferLength(nLen + 1);
		CComStr::Convert(pszC, -1, *str, nLen);

		*this = str;
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(PCWStr pszW)
{
	Clear();
	vt      = VT_BSTR;
	bstrVal = ::SysAllocString(pszW);
	if ((bstrVal == nullptr) && (pszW != nullptr))
	{
		vt    = VT_ERROR;
		scode = E_OUTOFMEMORY;
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(const CComStr& str)
{
	Clear();
	vt      = VT_BSTR;
	bstrVal = str.Copy();
	if ((bstrVal == nullptr) && (*str != nullptr))
	{
		vt    = VT_ERROR;
		scode = E_OUTOFMEMORY;
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(IDispatch* p)
{
	Clear();
	vt = VT_DISPATCH;
	pdispVal = p;
	// Need to AddRef as VariantClear will Release
	if (pdispVal != nullptr)
	{
		pdispVal->AddRef();
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(IUnknown*  p)
{
	Clear();
	vt = VT_UNKNOWN;
	punkVal = p;

	// Need to AddRef as VariantClear will Release
	if (punkVal != nullptr)
	{
		punkVal->AddRef();
	}
	return (*this);
}

INLINE CComVar& CComVar::operator=(const SAFEARRAY* p)
{
	Clear();
	LPSAFEARRAY pCopy = nullptr;
	if (p != nullptr)
	{
		Long lRet = ::SafeArrayCopy((LPSAFEARRAY)p, &pCopy);
		if (SUCCEEDED(lRet) && (pCopy != nullptr))
		{
			lRet = ::SafeArrayGetVartype(p, &vt);
			if (SUCCEEDED(lRet))
			{
				// When Windows has a SAFEARRAY of type VT_DISPATCH with FADF_HAVEIID,
				// it returns VT_UNKNOWN instead of VT_DISPATCH. We patch the value to be correct
				if ((vt == VT_UNKNOWN) && ((p->fFeatures & FADF_HAVEIID) != 0) && ((p->fFeatures & FADF_DISPATCH) != 0))
				{
					vt = VT_DISPATCH;
				}
				vt    |= VT_ARRAY;
				parray = pCopy;
			}
			else
			{
				vt    = VT_ERROR;
				scode = lRet;
			}
		}
		else
		{
			vt    = VT_ERROR;
			scode = lRet;
		}
	}
	return (*this);
}

INLINE bool CComVar::operator==(const VARIANT& aSrc) const
{
	// For backwards compatibility
	if (vt == VT_NULL && aSrc.vt == VT_NULL)
	{
		return true;
	}
	// Variants not equal if types don't match
	if (vt != aSrc.vt)
	{
		return false;
	}
	return (VarCmp((VARIANT*)this, (VARIANT*)&aSrc, LOCALE_USER_DEFAULT, 0) ==	static_cast<HRESULT>(VARCMP_EQ));
}

INLINE bool CComVar::operator!=(const VARIANT& aSrc) const
{
	return ((operator==(aSrc)) == false);
}

INLINE bool CComVar::operator<(const VARIANT& aSrc) const;
{
	if (vt == VT_NULL && aSrc.vt == VT_NULL)
	{
		return false;
	}
	return (VarCmp((VARIANT*)this, (VARIANT*)&aSrc, LOCALE_USER_DEFAULT, 0) ==	static_cast<HRESULT>(VARCMP_LT));
}

INLINE bool CComVar::operator>(const VARIANT& aSrc) const
{
	if (vt == VT_NULL && aSrc.vt == VT_NULL)
	{
		return false;
	}
	return (VarCmp((VARIANT*)this, (VARIANT*)&aSrc, LOCALE_USER_DEFAULT, 0) ==	static_cast<HRESULT>(VARCMP_GT));
}

INLINE Long CComVar::Attach(VARIANT* pVar)
{
	if(pVar == nullptr)
	{
		return E_INVALIDARG;
	}		
	// Clear out the variant
	Long lRet = Clear();
	if (SUCCEEDED(lRet))
	{
		// Copy the contents and give control to CComVariant
		MM_SAFE::Memcpy(this, sizeof(CComVar), pVar, sizeof(VARIANT));
		p->vt = VT_EMPTY;
		lRet  = S_OK;
	}
	return lRet;
}

INLINE Long CComVar::Detach(VARIANT* pVar)
{
	if(pVar == nullptr)
	{
		return E_INVALIDARG;
	}		
	// Clear out the variant
	Long lRet = ::VariantClear(pVar);
	if (SUCCEEDED(lRet))
	{
		// Copy the contents and remove control from CComVariant
		MM_SAFE::Memcpy(pVar, sizeof(VARIANT), this, sizeof(VARIANT));
		vt   = VT_EMPTY;
		lRet = S_OK;
	}
	return lRet;
}

INLINE Long CComVar::Clear(bool bZero)
{
	Long lRet = ::VariantClear(this);
	if (bZero && SUCCEEDED(lRet))
	{
		MM_SAFE::Memset(this, 0, sizeof(VARIANT));
		vt = VT_EMPTY;
	}
	return lRet;
}

INLINE Long CComVar::ClearScode(void)
{
	Long lRet = Clear();
	if (FAILED(lRet))
	{
		vt    = VT_ERROR;
		scode = lRet;
	}
	return lRet;
}

INLINE Long CComVar::Copy(const VARIANT* pVar)
{
	return ::VariantCopy(this, const_cast<VARIANT*>(pVar));
}

INLINE void CComVar::CopyScode(const VARIANT* pVar)
{
	Long lRet = Copy(pVar);
	if (FAILED(lRet))
	{
		vt    = VT_ERROR;
		scode = lRet;
	}
	return lRet;
}

INLINE Long CComVar::ChangeType(VARTYPE vtNew, const VARIANT* pVar)
{
	VARIANT* p = const_cast<VARIANT*>(pVar);
	// Convert in place if pSrc is nullptr
	if (p == nullptr)
	{
		p = this;
	}
	// Do nothing if doing in place convert and vts not different
	return ::VariantChangeType(this, p, 0, vtNew);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __COM_BASE_INL__