///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : container.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : common container                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONTAINER_INL__
#define __CONTAINER_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTContainerAlloc
template <typename T>
INLINE CTContainerAlloc<T>::CTContainerAlloc(void)
: m_pThis(nullptr)
{
}

template <typename T>
INLINE CTContainerAlloc<T>::~CTContainerAlloc(void)
{
}

template <typename T>
INLINE CTContainerAlloc<T>::CTContainerAlloc(const CTContainerAlloc<T>&)
: m_pThis(nullptr)
{
}

template <typename T>
INLINE CTContainerAlloc<T>& CTContainerAlloc<T>::operator=(const CTContainerAlloc<T>&)
{
	return (*this);
}

template <typename T>
INLINE T* CTContainerAlloc<T>::Create(CTContainerAlloc<T>*& pThis, size_t stElements)
{
	assert(stElements > 0);
	if (stElements > 0)
	{
		CTContainerAlloc<T>* pAlloc = reinterpret_cast<CTContainerAlloc<T>*>( ALLOC( sizeof(CTContainerAlloc<T>) + stElements * sizeof(T) ) );
		if (pAlloc != nullptr)
		{
			pAlloc->m_pThis = pThis;
			pThis = pAlloc;

			return (reinterpret_cast<T*>(pAlloc + 1));
		}
	}
	return nullptr;
}

template <typename T>
INLINE T* CTContainerAlloc<T>::GetData(void)
{
	return (reinterpret_cast<T*>(this + 1));
}

template <typename T>
INLINE void CTContainerAlloc<T>::Destroy(void)
{	
	CTContainerAlloc<T>* pAlloc = nullptr;
	for (CTContainerAlloc<T>* pThis = this; pThis != nullptr; pThis = pAlloc)
	{				
		pAlloc = pThis->m_pThis;
		FREE( pThis );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CContainerTraits

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTArray
template <typename T, class Traits, Int nMemType>
INLINE CTArray<T, Traits, nMemType>::CTArray(Int nGrow) 
: m_paT(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
{
	SetGrow(nGrow);
}

template <typename T, class Traits, Int nMemType>
INLINE CTArray<T, Traits, nMemType>::~CTArray(void)
{
	RemoveAll();
}

template <typename T, class Traits, Int nMemType>
INLINE CTArray<T, Traits, nMemType>::CTArray(const CTArray<T, Traits, nMemType>& aSrc)
: m_paT(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
{
	Copy(aSrc);
}

template <typename T, class Traits, Int nMemType>
INLINE CTArray<T, Traits, nMemType>& CTArray<T, Traits, nMemType>::operator=(const CTArray<T, Traits, nMemType>& aSrc)
{
	Copy(aSrc);
	return (*this);
}
#ifdef __CXX11_IMP__
template <typename T, class Traits, Int nMemType>
INLINE CTArray<T, Traits, nMemType>::CTArray(CTArray<T, Traits, nMemType>&& aSrc)
: m_paT(aSrc.m_paT)
, m_nAllocSize(aSrc.m_nAllocSize)
, m_nSize(aSrc.m_nSize)
, m_nGrow(aSrc.m_nGrow)
{
	aSrc.m_paT        = nullptr;
	aSrc.m_nAllocSize = 0;
	aSrc.m_nSize      = 0;
	aSrc.m_nGrow      = 0;
}
#endif
template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::GetAllocSize(void) const
{
	return m_nAllocSize;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::GetSize(void) const
{
	return m_nSize;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::GetGrow(void) const
{
	return m_nGrow;
}

template <typename T, class Traits, Int nMemType>
INLINE void CTArray<T, Traits, nMemType>::SetGrow(Int nGrow)
{
	assert(nGrow > 0);
	nGrow   = DEF_BASE::Maxmin<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_MIN, CContainerTraits::DEFAULT_SIZE_MAX);
	m_nGrow = DEF_BASE::Align<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_GROW);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::GetShare(Int nAllocSize) const
{
	return (Int)(nAllocSize * sizeof(T));
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::SetShare(void* pShare, Int nAllocSize)
{
	if ((nMemType == CContainerTraits::MEM_TYPE_SHARED) && (m_paT == nullptr) && (m_nAllocSize == 0))
	{
		assert(m_nSize == 0);
		m_paT = reinterpret_cast<T*>(pShare);
		m_nAllocSize = nAllocSize;
		return true;
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::Add(INARGTYPE t)
{
	if (SetCount())
	{
		SetAtIndex(m_nSize, t);
		++m_nSize;
		return (m_nSize - 1);
	}
	return -1;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::Add(Int nCount, bool bZeroed)
{
	assert(nCount > 0);
	if (SetCount(nCount))
	{
		if (bZeroed)
		{
			MM_SAFE::Memset((m_paT + m_nSize), 0, nCount * sizeof(T));
		}
		Int nRet = m_nSize;
		m_nSize += nCount;
		return nRet;
	}
	return -1;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTArray<T, Traits, nMemType>::Find(INARGTYPE t) const
{
	for (Int i = 0; i < m_nSize; ++i)
	{
		if (Traits::CompareElements(m_paT[i], t))
		{
			return i;
		}
	}
	return -1;  // not found
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Remove(INARGTYPE t)
{
	Int nIndex = Find(t);
	if (nIndex == -1)
	{
		return false;
	}
	return RemoveAt(nIndex);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::RemoveAt(Int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	if (Check(nIndex) == false)
	{
		return false;
	}
	m_paT[nIndex].~T();
	if (nIndex < (m_nSize - 1))
	{
		Traits::RelocateElements((m_paT + nIndex), (m_paT + nIndex + 1), (m_nSize - (nIndex + 1)));
	}
	--m_nSize;
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE void CTArray<T, Traits, nMemType>::RemoveAll(void)
{
	if (m_paT != nullptr)
	{
		for (Int i = 0; i < m_nSize; ++i)
		{
			m_paT[i].~T();
		}
	}
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		if (m_paT != nullptr)
		{
			FREE( m_paT );
			m_paT = nullptr;
		}
		m_nAllocSize = 0;
		m_nSize = 0;
	}
	else
	{
		m_nSize = 0;
	}
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Check(Int nIndex) const
{
	return ((nIndex >= 0) && (nIndex < m_nSize));
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Insert(Int nIndex, Int nCount, bool bZeroed)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	assert(nCount > 0);
	if (Check(nIndex) && SetCount(nCount))
	{
		Traits::RelocateElements((m_paT + nIndex + nCount), (m_paT + nIndex), (m_nSize - nIndex));
		if (bZeroed)
		{
			MM_SAFE::Memset((m_paT + nIndex), 0, nCount * sizeof(T));
		}
		m_nSize += nCount;
		return true;
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::SetAt(Int nIndex, INARGTYPE t)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	if (Check(nIndex) == false)
	{
		return false;
	}
	SetAtIndex(nIndex, t);
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Shrink(void)
{
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		Int nNewCount = DEF_BASE<Int>((m_nSize + m_nGrow), m_nGrow);
		if (nNewCount < m_nAllocSize)
		{
			T* paT = reinterpret_cast<T*>( ALLOC( nNewCount * sizeof(T) ) );
			if (paT == nullptr)
			{
				return false;
			}
			Traits::RelocateElements(paT, m_paT, m_nSize);
			FREE( m_paT );
			m_paT = paT;
			m_nAllocSize = nNewCount;
			return true;
		}
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTArray<T, Traits, nMemType>::operator[] (Int nIndex)
{
	assert((nIndex >= 0) && (nIndex < m_nSize));
	if (Check(nIndex) == false)
	{
		throw (EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
	}
	return m_paT[nIndex];
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTArray<T, Traits, nMemType>::operator[] (Int nIndex) const
{
	assert((nIndex >= 0) && (nIndex < m_nSize));
	if (Check(nIndex) == false)
	{
		throw (EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
	}
	return m_paT[nIndex];
}

template <typename T, class Traits, Int nMemType>
INLINE const T* CTArray<T, Traits, nMemType>::GetData(void) const
{
	return m_paT;
}

template <typename T, class Traits, Int nMemType>
INLINE T* CTArray<T, Traits, nMemType>::GetItem(Int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	if (Check(nIndex))
	{
		return (m_paT + nIndex);
	}
	return nullptr;
}

template <typename T, class Traits, Int nMemType>
INLINE const T* CTArray<T, Traits, nMemType>::GetItem(Int nIndex) const
{
	assert(nIndex >= 0 && nIndex < m_nSize);
	if (CheckIndex(nIndex))
	{
		return (m_paT + nIndex);
	}
	return nullptr;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Append(const CTArray<T, Traits, nMemType>& aSrc)
{
	assert(this != &aSrc);
	if (this != &aSrc)
	{
		Int j = aSrc.GetSize();	
		if (SetCount(j))
		{
			Traits::CopyElements((m_paT + m_nSize), aSrc.m_paT, j);
			m_nSize += j;
			return true;
		}
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::Copy(const CTArray<T, Traits, nMemType>& aSrc)
{
	assert(this != &aSrc);
	if (this != &aSrc)
	{
		RemoveAll(); // only empty array can copy
		Int j = aSrc.GetSize();
		if (SetCount(j))
		{
			Traits::CopyElements((m_paT + m_nSize), aSrc.m_paT, j);
			m_nSize += j;
			return true;
		}
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTArray<T, Traits, nMemType>::SetCount(Int nNewCount)
{
	if (nNewCount <= 0)
	{
		return false;
	}
	nNewCount += m_nSize; 
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		if (m_paT == nullptr)
		{
			assert(m_nSize == 0);
			nNewCount = DEF_BASE::Align<Int>(nNewCount, m_nGrow);
			m_paT = reinterpret_cast<T*>( ALLOC( nNewCount * sizeof(T) ) );
			if (m_paT == nullptr)
			{
				return false;
			}
			m_nAllocSize = nNewCount;
		}
		else if (nNewCount > m_nAllocSize)
		{
			nNewCount = DEF_BASE::Align<Int>(nNewCount, m_nGrow);
			T* paT = reinterpret_cast<T*>( ALLOC( nNewCount * sizeof(T) ) );
			if (paT == nullptr)
			{
				return false;
			}
			Traits::RelocateElements(paT, m_paT, m_nSize);
			FREE( m_paT );
			m_paT = paT;		
			m_nAllocSize = nNewCount;
		}
	}
	else
	{
		if ((m_paT == nullptr) || (nNewCount > m_nAllocSize))
		{
			return false;
		}
	}
	assert(m_paT != nullptr);
	return true;
}

#pragma push_macro("new")
#undef new
template <typename T, class Traits, Int nMemType>
INLINE void CTArray<T, Traits, nMemType>::SetAtIndex(Int nIndex, INARGTYPE t)
{
	GNEW(m_paT + nIndex) T(t);
}
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTList
template <typename T, class Traits, Int nMemType>
INLINE CTList<T, Traits, nMemType>::CTList(Int nGrow) 
: m_pHead(nullptr)
, m_pTail(nullptr)
, m_pFree(nullptr)
, m_pShare(nullptr)
, m_pAlloc(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
{
	SetGrow(nGrow);
}

template <typename T, class Traits, Int nMemType>
INLINE CTList<T, Traits, nMemType>::~CTList(void)
{
	RemoveAll();
}

template <typename T, class Traits, Int nMemType>
INLINE CTList<T, Traits, nMemType>::CTList(const CTList<T, Traits, nMemType>& aSrc)
: m_pHead(nullptr)
, m_pTail(nullptr)
, m_pFree(nullptr)
, m_pShare(nullptr)
, m_pAlloc(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
{
	AddHeadList(aSrc);
}

template <typename T, class Traits, Int nMemType>
INLINE CTList<T, Traits, nMemType>& CTList<T, Traits, nMemType>::operator=(const CTList<T, Traits, nMemType>& aSrc)
{
	AddHeadList(aSrc);
	return (*this);
}
#ifdef __CXX11_IMP__
template <typename T, class Traits, Int nMemType>
INLINE CTList<T, Traits, nMemType>::CTList(CTList<T, Traits, nMemType>&& aSrc)
: m_pHead(aSrc.m_pHead)
, m_pTail(aSrc.m_pTail)
, m_pFree(aSrc.m_pFree)
, m_pShare(aSrc.m_pShare)
, m_pAlloc(aSrc.m_pAlloc)
, m_nAllocSize(aSrc.m_nAllocSize)
, m_nSize(aSrc.m_nSize)
, m_nGrow(aSrc.m_nGrow)
{
	aSrc.m_pHead      = nullptr;
	aSrc.m_pTail      = nullptr;
	aSrc.m_pFree      = nullptr;
	aSrc.m_pShare     = nullptr;
	aSrc.m_pAlloc     = nullptr;
	aSrc.m_nAllocSize = 0;
	aSrc.m_nSize      = 0;
	aSrc.m_nGrow      = 0;
}
#endif
template <typename T, class Traits, Int nMemType>
INLINE Int CTList<T, Traits, nMemType>::GetAllocSize(void) const
{
	return m_nAllocSize;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTList<T, Traits, nMemType>::GetSize(void) const
{
	return m_nSize;
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTList<T, Traits, nMemType>::GetGrow(void) const
{
	return m_nGrow;
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::SetGrow(Int nGrow)
{
	assert(nGrow > 0);
	nGrow   = DEF_BASE::Maxmin<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_MIN, CContainerTraits::DEFAULT_SIZE_MAX);
	m_nGrow = DEF_BASE::Align<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_GROW);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTList<T, Traits, nMemType>::GetShare(Int nAllocSize) const
{
	return (Int)(nAllocSize * sizeof(NODE));
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::SetShare(void* pShare, Int nAllocSize)
{
	if ((nMemType == CContainerTraits::MEM_TYPE_SHARED) && (m_pShare == nullptr) && (m_nAllocSize == 0))
	{
		assert(m_pHead  == nullptr);
		assert(m_pTail  == nullptr);
		assert(m_pFree  == nullptr);
		assert(m_pAlloc == nullptr);
		assert(m_nSize  == 0);
		m_pShare = reinterpret_cast<NODE*>(pShare);
		m_nAllocSize = nAllocSize;
		return true;
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::AddHead(void)
{
	NODE* pNode = NewNode(nullptr, m_pHead);
	if (pNode == nullptr)
	{
		return nullptr;
	}

	if (m_pHead != nullptr)
	{
		m_pHead->m_pPrev = pNode;
	}
	else
	{
		m_pTail = pNode;
	}
	m_pHead = pNode;

	return ((PINDEX)pNode);
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::AddHead(INARGTYPE t)
{
	NODE* pNode = NewNode(t, nullptr, m_pHead);
	if (pNode == nullptr)
	{
		return nullptr;
	}

	if (m_pHead != nullptr)
	{
		m_pHead->m_pPrev = pNode;
	}
	else
	{
		m_pTail = pNode;
	}
	m_pHead = pNode;

	return ((PINDEX)pNode);
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::AddHeadList(const CTList<T, Traits, nMemType>& aSrc)
{
	if (this == &aSrc)
	{
		return;
	}
	PINDEX p = aSrc.GetTailIndex();
	while (p != nullptr)
	{
		INARGTYPE t = aSrc.GetPrev(p);
		AddHead(t);
	}
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::AddTail(void)
{
	NODE* pNode = NewNode(m_pTail, nullptr);
	if (pNode == nullptr)
	{
		return nullptr;
	}

	if (m_pTail != nullptr)
	{
		m_pTail->m_pNext = pNode;
	}
	else
	{
		m_pHead = pNode;
	}
	m_pTail = pNode;

	return ((PINDEX)pNode);
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::AddTail(INARGTYPE t)
{
	NODE* pNode = NewNode(t, m_pTail, nullptr);
	if (pNode == nullptr)
	{
		return nullptr;
	}

	if (m_pTail != nullptr)
	{
		m_pTail->m_pNext = pNode;
	}
	else
	{
		m_pHead = pNode;
	}
	m_pTail = pNode;

	return ((PINDEX)pNode);
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::AddTailList(const CTList<T, Traits, nMemType>& aSrc)
{
	if (this == &aSrc)
	{
		return;
	}
	PINDEX p = aSrc.GetHeadIndex();
	while (p != nullptr)
	{
		INARGTYPE t = aSrc.GetNext(p);
		AddTail(t);
	}
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::Find(INARGTYPE t, PINDEX pStartAfter) const
{
	NODE* pNode = static_cast<NODE*>(pStartAfter);
	if (pNode == nullptr)
	{
		pNode = m_pHead;  // start at head
	}
	else
	{
		pNode = pNode->m_pNext;  // start after the one specified
	}

	for (; pNode != nullptr; pNode = pNode->m_pNext)
	{
		if (Traits::CompareElements(pNode->m_T, t))
		{
			return ((PINDEX)pNode);
		}
	}
	return nullptr;
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::FindIndex(Int nIndex) const
{
	if (nIndex >= m_nSize)
	{
		return nullptr;  
	}
	NODE* pNode = m_pHead;
	for (Int i = 0; i < nIndex; ++i)
	{
		pNode = pNode->m_pNext;
	}
	return ((PINDEX)pNode);
}

template <typename T, class Traits, Int nMemType>
INLINE T CTList<T, Traits, nMemType>::RemoveHead(void)
{
	assert(m_pHead != nullptr);
	if (m_pHead == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* pNode = m_pHead;
	T t(pNode->m_T);

	m_pHead = pNode->m_pNext;
	if (m_pHead != nullptr)
	{
		m_pHead->m_pPrev = nullptr;
	}
	else
	{
		m_pTail = nullptr;
	}
	FreeNode(pNode);
	return t;
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::RemoveHeadDirect(void)
{
	assert(m_pHead != nullptr);
	NODE* pNode = m_pHead;
	if (pNode != nullptr)
	{
		m_pHead = pNode->m_pNext;
		if (m_pHead != nullptr)
		{
			m_pHead->m_pPrev = nullptr;
		}
		else
		{
			m_pTail = nullptr;
		}
		FreeNode(pNode);
	}
}

template <typename T, class Traits, Int nMemType>
INLINE T CTList<T, Traits, nMemType>::RemoveTail(void)
{
	assert(m_pTail != nullptr);
	if (m_pTail == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* pNode = m_pTail;
	T t(pNode->m_T);

	m_pTail = pNode->m_pPrev;
	if (m_pTail != nullptr)
	{
		m_pTail->m_pNext = nullptr;
	}
	else
	{
		m_pHead = nullptr;
	}
	FreeNode(pNode);
	return t;
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::RemoveTailDirect(void)
{
	assert(m_pTail != nullptr);
	NODE* pNode = m_pTail;
	if (pNode != nullptr)
	{
		m_pTail = pNode->m_pPrev;
		if (m_pTail != nullptr)
		{
			m_pTail->m_pNext = nullptr;
		}
		else
		{
			m_pHead = nullptr;
		}
		FreeNode(pNode);
	}
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::Remove(INARGTYPE t)
{
	PINDEX index = Find(t);
	if (index != nullptr)
	{
		return RemoveAt(index);
	}
	return false;
} 

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::RemoveAt(PINDEX index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode == nullptr)
	{
		return false;
	}
	// remove pNode from list
	if (pNode == m_pHead)
	{
		m_pHead = pNode->m_pNext;
	}
	else
	{
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}

	if (pNode == m_pTail)
	{
		m_pTail = pNode->m_pPrev;
	}
	else
	{
		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}
	FreeNode(pNode);
	return true;
}  

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::RemoveAll(void)
{
	for (NODE* p = m_pHead; p != nullptr; p = m_pHead)
	{
		m_pHead = p->m_pNext;
		p->~NODE();
	}
	if (m_pAlloc != nullptr)
	{	
		m_pAlloc->Destroy();
		m_pAlloc = nullptr;
	}
	m_pHead = nullptr;
	m_pTail = nullptr;
	m_pFree = nullptr;
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		m_nAllocSize = 0;
	}
	m_nSize = 0;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::Check(PINDEX index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if ((pNode == nullptr) || (pNode->m_pPrev == reinterpret_cast<NODE*>(-1))) // free flag -1
	{
		return false;
	}
	return true;
}  

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::InsertBefore(PINDEX index, INARGTYPE t)
{
	if (index == nullptr)
	{
		return AddHead(t); 
	}
	// Insert it before position
	NODE* pOldNode = static_cast<NODE*>(index);
	NODE* pNewNode = NewNode(t, pOldNode->m_pPrev, pOldNode);

	if (pOldNode->m_pPrev != nullptr)
	{
		pOldNode->m_pPrev->m_pNext = pNewNode;
	}
	else
	{
		assert(pOldNode == m_pHead);
		m_pHead = pNewNode;
	}
	pOldNode->m_pPrev = pNewNode;

	return ((PINDEX)pNewNode);
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::InsertAfter(PINDEX index, INARGTYPE t)
{
	if (index == nullptr)
	{
		return AddTail(t);
	}
	// Insert it after position
	NODE* pOldNode = static_cast<NODE*>(index);
	NODE* pNewNode = NewNode(t, pOldNode, pOldNode->m_pNext);

	if (pOldNode->m_pNext != nullptr)
	{
		pOldNode->m_pNext->m_pPrev = pNewNode;
	}
	else
	{
		assert(pOldNode == m_pTail);
		m_pTail = pNewNode;
	}
	pOldNode->m_pNext = pNewNode;

	return ((PINDEX)pNewNode);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::MoveToHead(PINDEX index)
{
	assert(index != nullptr);
	NODE* pNode = static_cast<NODE*>(index);
	if ((pNode == m_pHead) || (pNode == nullptr))
	{
		return false;
	}
	if (pNode->m_pNext == nullptr)
	{
		assert(pNode == m_pTail);
		m_pTail = pNode->m_pPrev;
	}
	else
	{
		pNode->m_pNext->m_pPrev = pNode->m_pPrev;
	}
	assert(pNode->m_pPrev != nullptr); 
	pNode->m_pPrev->m_pNext = pNode->m_pNext;

	m_pHead->m_pPrev = pNode;
	pNode->m_pNext = m_pHead;
	pNode->m_pPrev = nullptr;
	m_pHead = pNode;
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::MoveToTail(PINDEX index)
{
	assert(index != nullptr);
	NODE* pNode = static_cast<NODE*>(index);
	if ((pNode == m_pTail) || (pNode == nullptr))
	{
		return false;
	}
	if (pNode->m_pPrev == nullptr)
	{
		assert( pNode == m_pHead );
		m_pHead = pNode->m_pNext;
	}
	else
	{
		pNode->m_pPrev->m_pNext = pNode->m_pNext;
	}
	assert( pNode->m_pNext != nullptr );  
	pNode->m_pNext->m_pPrev = pNode->m_pPrev;

	m_pTail->m_pNext = pNode;
	pNode->m_pPrev = m_pTail;
	pNode->m_pNext = nullptr;
	m_pTail = pNode;
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::Swap(PINDEX index1, PINDEX index2) 
{
	assert(index1 != nullptr);
	assert(index2 != nullptr);
	if ((index1 == nullptr) || (index2 == nullptr) || (index1 == index2))
	{
		return false;
	}
	NODE* pNode1 = static_cast<NODE*>(index1);
	NODE* pNode2 = static_cast<NODE*>(index2);
	if (pNode2->m_pNext == pNode1)
	{
		// Swap pNode2 and pNode1 so that the next case works
		NODE* pTemp = pNode1;
		pNode1 = pNode2;
		pNode2 = pTemp;
	}
	if (pNode1->m_pNext == pNode2)
	{
		// Node1 and Node2 are adjacent
		pNode2->m_pPrev = pNode1->m_pPrev;
		if (pNode1->m_pPrev != nullptr)
		{
			pNode1->m_pPrev->m_pNext = pNode2;
		}
		else
		{
			assert(m_pHead == pNode1);
			m_pHead = pNode2;
		}
		pNode1->m_pNext = pNode2->m_pNext;
		if (pNode2->m_pNext != nullptr)
		{
			pNode2->m_pNext->m_pPrev = pNode1;
		}
		else
		{
			assert(m_pTail == pNode2);
			m_pTail = pNode1;
		}
		pNode2->m_pNext = pNode1;
		pNode1->m_pPrev = pNode2;
	}
	else
	{
		// The two nodes are not adjacent
		NODE* pTemp = pNode1->m_pPrev;
		pNode1->m_pPrev = pNode2->m_pPrev;
		pNode2->m_pPrev = pTemp;

		pTemp = pNode1->m_pNext;
		pNode1->m_pNext = pNode2->m_pNext;
		pNode2->m_pNext = pTemp;

		if (pNode1->m_pNext != nullptr)
		{
			pNode1->m_pNext->m_pPrev = pNode1;
		}
		else
		{
			assert(m_pTail == pNode2);
			m_pTail = pNode1;
		}
		if (pNode1->m_pPrev != nullptr)
		{
			pNode1->m_pPrev->m_pNext = pNode1;
		}
		else
		{
			assert(m_pHead == pNode2);
			m_pHead = pNode1;
		}
		if (pNode2->m_pNext != nullptr)
		{
			pNode2->m_pNext->m_pPrev = pNode2;
		}
		else
		{
			assert(m_pTail == pNode1);
			m_pTail = pNode2;
		}
		if (pNode2->m_pPrev != nullptr)
		{
			pNode2->m_pPrev->m_pNext = pNode2;
		}
		else
		{
			assert( m_pHead == pNode1 );
			m_pHead = pNode2;
		}
	}
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::SetAt(PINDEX index, INARGTYPE t)
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		return false;
	}
	NODE* pNode = static_cast<NODE*>(index);
	pNode->m_T = t;
	return true;
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTList<T, Traits, nMemType>::GetAt(PINDEX index)
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* pNode = static_cast<NODE*>(index);
	return (pNode->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTList<T, Traits, nMemType>::GetAt(PINDEX index) const
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* pNode = static_cast<NODE*>(index);
	return (pNode->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTList<T, Traits, nMemType>::GetHead(void)
{
	assert(m_pHead != nullptr);
	if (m_pHead == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	return (m_pHead->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTList<T, Traits, nMemType>::GetHead(void) const
{
	assert(m_pHead != nullptr);
	if (m_pHead == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	return (m_pHead->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::GetHeadIndex(void) const
{
	return ((PINDEX)m_pHead);
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTList<T, Traits, nMemType>::GetTail(void)
{
	assert(m_pTail != nullptr);
	if (m_pTail == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	return (m_pTail->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTList<T, Traits, nMemType>::GetTail(void) const
{
	assert(m_pTail != nullptr);
	if (m_pTail == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	return (m_pTail->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE PINDEX CTList<T, Traits, nMemType>::GetTailIndex(void) const
{
	return ((PINDEX)m_pTail);
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTList<T, Traits, nMemType>::GetNext(PINDEX& index)
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* p = static_cast<NODE*>(index);
	index = PINDEX(p->m_pNext);
	return (p->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTList<T, Traits, nMemType>::GetNext(PINDEX& index) const
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* p = static_cast<NODE*>(index);
	index = PINDEX(p->m_pNext);
	return (p->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTList<T, Traits, nMemType>::GetPrev(PINDEX& index)
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* p = static_cast<NODE*>(index);
	index = PINDEX(p->m_pPrev);
	return (p->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTList<T, Traits, nMemType>::GetPrev(PINDEX& index) const
{
	assert(index != nullptr);
	if (index == nullptr)
	{
		throw (EXCEPTION_ACCESS_VIOLATION);
	}
	NODE* p = static_cast<NODE*>(index);
	index = PINDEX(p->m_pPrev);
	return (p->m_T);
}

template <typename T, class Traits, Int nMemType>
INLINE void CTList<T, Traits, nMemType>::FreeNode(NODE* pNode)
{   
	pNode->~NODE();
	pNode->m_pNext = m_pFree;
	pNode->m_pPrev = reinterpret_cast<NODE*>(-1);// free node flag
	m_pFree = pNode;
	assert(m_nSize > 0);
	--m_nSize;
	if (m_nSize == 0)
	{
		RemoveAll();
	}
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTList<T, Traits, nMemType>::GetFreeNode(void)
{
	if (m_pFree == nullptr)
	{
		if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
		{
			NODE* pNode = CTContainerAlloc<NODE>::Create(m_pAlloc, (size_t)m_nGrow);
			if (pNode == nullptr)
			{
				throw (EXCEPTION_OUTOFMEMORY);
			}
			pNode += (m_nGrow - 1);
			for (Int i = m_nGrow - 1; i >= 0; --i)
			{
				pNode->m_pNext = m_pFree;
				pNode->m_pPrev = reinterpret_cast<NODE*>(-1);// free node flag
				m_pFree = pNode;
				--pNode;
			}
			m_nAllocSize += m_nGrow;
		}
		else
		{
			if ((m_pShare == nullptr) || (m_nSize >= m_nAllocSize))
			{
				throw (EXCEPTION_OUTOFMEMORY);
			}
			NODE* pNode = m_pShare;
			pNode += (m_nAllocSize - 1);
			for (Int i = m_nAllocSize - 1; i >= 0; --i)
			{
				pNode->m_pNext = m_pFree;
				pNode->m_pPrev = reinterpret_cast<NODE*>(-1);// free node flag
				m_pFree = pNode;
				--pNode;
			}
		}
	}
	return (m_pFree != nullptr);
}  

#pragma push_macro("new")
#undef new
template <typename T, class Traits, Int nMemType>
INLINE typename CTList<T, Traits, nMemType>::NODE* CTList<T, Traits, nMemType>::NewNode(NODE* pPrev, NODE* pNext)
{
	if (GetFreeNode())
	{
		NODE* pNewNode = m_pFree;
		m_pFree = m_pFree->m_pNext;
		assert(pNewNode->m_pPrev == reinterpret_cast<NODE*>(-1)); // free flag -1

		GNEW(pNewNode) NODE(pPrev, pNext);

		++m_nSize;
		assert(m_nSize > 0);

		return pNewNode;
	}
	return nullptr;
}

template <typename T, class Traits, Int nMemType>
INLINE typename CTList<T, Traits, nMemType>::NODE* CTList<T, Traits, nMemType>::NewNode(INARGTYPE t, NODE* pPrev, NODE* pNext)
{
	if (GetFreeNode())
	{
		NODE* pNewNode = m_pFree;
		m_pFree = m_pFree->m_pNext;
		assert(pNewNode->m_pPrev == reinterpret_cast<NODE*>(-1)); // free flag -1

		GNEW(pNewNode) NODE(t, pPrev, pNext);

		++m_nSize;
		assert(m_nSize > 0);

		return pNewNode;
	}
	return nullptr;
}
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTQueue
template <typename T, class Traits, Int nMemType>
INLINE CTQueue<T, Traits, nMemType>::CTQueue(Int nGrow)
: m_List(nGrow)
{ 
}

template <typename T, class Traits, Int nMemType>
INLINE CTQueue<T, Traits, nMemType>::~CTQueue(void)
{
}

template <typename T, class Traits, Int nMemType>
INLINE CTQueue<T, Traits, nMemType>::CTQueue(const CTQueue<T, Traits, nMemType>&)
: m_List()
{
}

template <typename T, class Traits, Int nMemType>
INLINE CTQueue<T, Traits, nMemType>& CTQueue<T, Traits, nMemType>::operator=(const CTQueue<T, Traits, nMemType>&)
{
	return (*this);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTQueue<T, Traits, nMemType>::GetSize(void) const
{
	return m_List.GetSize();
}

template <typename T, class Traits, Int nMemType>
INLINE void CTQueue<T, Traits, nMemType>::SetGrow(Int nGrow)
{
	m_List.SetGrow(nGrow);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTQueue<T, Traits, nMemType>::GetShare(Int nAllocSize) const
{
	return m_List.GetShare(nAllocSize);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTQueue<T, Traits, nMemType>::SetShare(void* pShare, Int nAllocSize)
{
	return m_List.SetShare(pShare, nAllocSize);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTQueue<T, Traits, nMemType>::IsEmpty(void) const
{
	return (m_List.GetSize() == 0);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTQueue<T, Traits, nMemType>::IsFull(void) const
{
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTQueue<T, Traits, nMemType>::In(INARGTYPE t)
{
	if (IsFull() == false)
	{
		return (m_List.AddTail(t) != nullptr);
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE T CTQueue<T, Traits, nMemType>::Out(void)
{
	if (IsEmpty() == false)
	{
		return m_List.RemoveHead();
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename T, class Traits, Int nMemType>
INLINE void CTQueue<T, Traits, nMemType>::RemoveAll(void)
{
	m_List.RemoveAll();
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTQueue<T, Traits, nMemType>::GetItem(void)
{
	return m_List.GetHead();
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTQueue<T, Traits, nMemType>::GetItem(void) const
{
	return m_List.GetHead();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStack
template <typename T, class Traits, Int nMemType>
INLINE CTStack<T, Traits, nMemType>::CTStack(Int nGrow)
: m_List(nGrow)
{ 
}

template <typename T, class Traits, Int nMemType>
INLINE CTStack<T, Traits, nMemType>::~CTStack(void)
{
}

template <typename T, class Traits, Int nMemType>
INLINE CTStack<T, Traits, nMemType>::CTStack(const CTStack<T, Traits, nMemType>&)
: m_List()
{
}

template <typename T, class Traits, Int nMemType>
INLINE CTStack<T, Traits, nMemType>& CTStack<T, Traits, nMemType>::operator=(const CTStack<T, Traits, nMemType>&)
{
	return (*this);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTStack<T, Traits, nMemType>::GetSize(void) const
{
	return m_List.GetSize();
}

template <typename T, class Traits, Int nMemType>
INLINE void CTStack<T, Traits, nMemType>::SetGrow(Int nGrow)
{
	m_List.SetGrow(nGrow);
}

template <typename T, class Traits, Int nMemType>
INLINE Int CTStack<T, Traits, nMemType>::GetShare(Int nAllocSize) const
{
	return m_List.GetShare(nAllocSize);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTStack<T, Traits, nMemType>::SetShare(void* pShare, Int nAllocSize)
{
	return m_List.SetShare(pShare, nAllocSize);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTStack<T, Traits, nMemType>::IsEmpty(void) const
{
	return (m_List.GetSize() == 0);
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTStack<T, Traits, nMemType>::IsFull(void) const
{
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE bool CTStack<T, Traits, nMemType>::Push(INARGTYPE t)
{
	if (IsFull() == false)
	{
		return (m_List.AddHead(t) != nullptr);
	}
	return false;
}

template <typename T, class Traits, Int nMemType>
INLINE T CTStack<T, Traits, nMemType>::Pop(void)
{
	if (IsEmpty() == false)
	{
		return m_List.RemoveHead();
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename T, class Traits, Int nMemType>
INLINE void CTStack<T, Traits, nMemType>::RemoveAll(void)
{
	m_List.RemoveAll();
}

template <typename T, class Traits, Int nMemType>
INLINE T& CTStack<T, Traits, nMemType>::GetItem(void)
{
	return m_List.GetHead();
}

template <typename T, class Traits, Int nMemType>
INLINE const T& CTStack<T, Traits, nMemType>::GetItem(void) const
{
	return m_List.GetHead();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTMap
template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::CTMap(Int nGrow, Int nHash) 
: m_pFree(nullptr)
, m_pShare(nullptr)
, m_ppHash(nullptr)
, m_pAlloc(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
, m_nHash(0)
, m_nFirst(0)
, m_nLast(0)
{
	SetHash(nHash);
	SetGrow(nGrow);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::~CTMap(void)
{
	RemoveAll();
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::CTMap(const CTMap<K, V, KTraits, VTraits, nMemType, nOrder>& aSrc)
: m_pFree(nullptr)
, m_pShare(nullptr)
, m_ppHash(nullptr)
, m_pAlloc(nullptr)
, m_nAllocSize(0)
, m_nSize(0)
, m_nGrow(0)
, m_nHash(0)
, m_nFirst(0)
, m_nLast(0)
{
	Copy(aSrc);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE CTMap<K, V, KTraits, VTraits, nMemType, nOrder>& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::operator=(const CTMap<K, V, KTraits, VTraits, nMemType, nOrder>& aSrc)
{
	Copy(aSrc);
	return (*this);
}
#ifdef __CXX11_IMP__
template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::CTMap(CTMap<K, V, KTraits, VTraits, nMemType, nOrder>&& aSrc)
: m_pFree(aSrc.m_pFree)
, m_pShare(aSrc.m_pShare)
, m_ppHash(aSrc.m_ppHash)
, m_pAlloc(aSrc.m_pAlloc)
, m_nAllocSize(aSrc.m_nAllocSize)
, m_nSize(aSrc.m_nSize)
, m_nGrow(aSrc.m_nGrow)
, m_nHash(aSrc.m_nHash)
, m_nFirst(aSrc.m_nFirst)
, m_nLast(aSrc.m_nLast)
{
	aSrc.m_pFree      = nullptr;
	aSrc.m_pShare     = nullptr;
	aSrc.m_ppHash     = nullptr;
	aSrc.m_pAlloc     = nullptr;
	aSrc.m_nAllocSize = 0;
	aSrc.m_nSize      = 0;
	aSrc.m_nGrow      = 0;
	aSrc.m_nHash      = 0;
	aSrc.m_nFirst     = 0;
	aSrc.m_nLast      = 0;
}
#endif
template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE Int CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetAllocSize(void) const
{
	return m_nAllocSize;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE Int CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetSize(void) const
{
	return m_nSize;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE Int CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetGrow(void) const
{
	return m_nGrow;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::SetGrow(Int nGrow)
{
	assert(nGrow > 0);
	nGrow   = DEF_BASE::Maxmin<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_MIN, CContainerTraits::DEFAULT_SIZE_MAX);
	m_nGrow = DEF_BASE::Align<Int>(nGrow, CContainerTraits::DEFAULT_SIZE_GROW);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE Int CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetHash(void) const
{
	return m_nHash;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::SetHash(Int nHash)
{
	if (m_ppHash == nullptr)
	{
		assert(nHash > 0);
		nHash    = DEF_BASE::Maxmin<Int>(nHash, CContainerTraits::DEFAULT_HASH_MIN, CContainerTraits::DEFAULT_HASH_MAX);
		m_nHash  = DEF_BASE::Align<Int>(nHash, CContainerTraits::DEFAULT_HASH_MIN);
		m_nFirst = m_nHash - 1;
	}
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE Int CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetShare(Int nAllocSize) const
{
	return (Int)(nAllocSize * sizeof(NODE) + m_nHash * sizeof(NODE*));
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::SetShare(void* pShare, Int nAllocSize)
{
	if ((nMemType == CContainerTraits::MEM_TYPE_SHARED) && (m_pShare == nullptr) && (m_nAllocSize == 0))
	{
		assert(m_pFree  == nullptr);
		assert(m_ppHash == nullptr);
		assert(m_pAlloc == nullptr);
		assert(m_nSize  == 0);
		m_pShare = reinterpret_cast<NODE*>(pShare);
		m_ppHash = reinterpret_cast<NODE**>(m_pShare + nAllocSize);
		m_nAllocSize = nAllocSize;
		BuildHash();
		return true;
	}
	return false;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Add(KINARGTYPE Key)
{
	if (m_ppHash == nullptr)
	{
		if (BuildHash() == false)
		{
			return nullptr;
		}
	}

	NODE* pNode = NewNode(Key);
	return ((PINDEX)pNode);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Add(KINARGTYPE Key, VINARGTYPE Val)
{
	if (m_ppHash == nullptr)
	{
		if (BuildHash() == false)
		{
			return nullptr;
		}
	}

	NODE* pNode = NewNode(Key, Val);
	return ((PINDEX)pNode);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Find(KINARGTYPE Key, PINDEX pStartAfter)
{
	if (m_ppHash != nullptr)
	{
		NODE* pNode = nullptr;
		if (pStartAfter != nullptr)
		{
			pNode = static_cast<NODE*>(pStartAfter);
			if (pNode == nullptr)
			{
				return nullptr;
			}
			if (pNode->m_nHash == (Int)(KTraits::HashElements(Key) & (m_nHash - 1)))
			{
				pNode = pNode->m_pNext;
				if (pNode == nullptr)
				{
					return nullptr;
				}
			}
			else
			{
				pNode = nullptr;
			}
		}
		if (pNode == nullptr)
		{
			pNode = m_ppHash[KTraits::HashElements(Key) & (m_nHash - 1)];
		}
		for (; pNode != nullptr; pNode = pNode->m_pNext)
		{
			if (KTraits::CompareElements(pNode->m_K, Key))
			{
				return ((PAIR*)pNode);
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Find(KINARGTYPE Key, PINDEX pStartAfter) const
{
	if (m_ppHash != nullptr)
	{
		NODE* pNode = nullptr;
		if (pStartAfter != nullptr)
		{
			pNode = static_cast<NODE*>(pStartAfter);
			if (pNode == nullptr)
			{
				return nullptr;
			}
			if (pNode->m_nHash == (Int)(KTraits::HashElements(Key) & (m_nHash - 1)))
			{
				pNode = pNode->m_pNext;
				if (pNode == nullptr)
				{
					return nullptr;
				}
			}
			else
			{
				pNode = nullptr;
			}
		}
		if (pNode == nullptr)
		{
			pNode = m_ppHash[KTraits::HashElements(Key) & (m_nHash - 1)];
		}
		for (; pNode != nullptr; pNode = pNode->m_pNext)
		{
			if (KTraits::CompareElements(pNode->m_K, Key))
			{
				return ((const PAIR*)pNode);
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::FindIndex(KINARGTYPE Key, PINDEX pStartAfter) const
{
	if (m_ppHash != nullptr)
	{
		NODE* pNode = nullptr;
		if (pStartAfter != nullptr)
		{
			pNode = static_cast<NODE*>(pStartAfter);
			if (pNode == nullptr)
			{
				return nullptr;
			}
			if (pNode->m_nHash == (Int)(KTraits::HashElements(Key) & (m_nHash - 1)))
			{
				pNode = pNode->m_pNext;
				if (pNode == nullptr)
				{
					return nullptr;
				}
			}
			else
			{
				pNode = nullptr;
			}
		}
		if (pNode == nullptr)
		{
			pNode = m_ppHash[KTraits::HashElements(Key) & (m_nHash - 1)];
		}
		for (; pNode != nullptr; pNode = pNode->m_pNext)
		{
			if (KTraits::CompareElements(pNode->m_K, Key))
			{
				return ((PINDEX)pNode);
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Remove(KINARGTYPE Key)
{
	NODE* pNode = static_cast<NODE*>(Find(Key));
	if (pNode != nullptr)
	{
		DeleteNode(pNode);
	}
	return (pNode != nullptr);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::RemoveAt(PINDEX index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		DeleteNode(pNode);
	}
	return (pNode != nullptr);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::RemoveAll(void)
{
	if (m_ppHash != nullptr)
	{
		for (Int i = m_nFirst; i <= m_nLast; ++i)
		{
			NODE* pHead = m_ppHash[i];
			for (NODE* p = pHead; p != nullptr; p = pHead)
			{
				assert(p->m_nHash >= 0); // free flag -1
				pHead = p->m_pNext;
				p->~NODE();
			}
		}
	}
	if (m_pAlloc != nullptr)
	{	
		m_pAlloc->Destroy();
		m_pAlloc = nullptr;
	}
	m_pFree = nullptr;
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		if (m_ppHash != nullptr)
		{
			FREE( m_ppHash );
			m_ppHash = nullptr;
		}
		m_nAllocSize = 0;
	}
	else
	{
		BuildHash();
	}
	m_nSize  = 0;
	m_nFirst = m_nHash - 1;
	m_nLast  = 0;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Check(PINDEX index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if ((pNode == nullptr) || (pNode->m_nHash == -1)) // free flag -1
	{
		return false;
	}
	return true;
}

// if can not find the key, add a new node
template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::SetAt(KINARGTYPE Key, VINARGTYPE Val)
{
	NODE* pNode = static_cast<NODE*>(Find(Key));
	if (pNode != nullptr)
	{
		pNode->m_V = Val;
		return ((PINDEX)pNode);
	}
	else
	{
		return Add(Key, Val);
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::SetAtIndex(PINDEX index, VINARGTYPE Val)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		pNode->m_V = Val;
	}
	return (pNode != nullptr);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetAt(PINDEX index)
{
	assert(index != nullptr);
	return static_cast<PAIR*>(index);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetAt(PINDEX index) const
{
	assert(index != nullptr);
	return static_cast<const PAIR*>(index);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const K& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetKeyAt(PINDEX index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		return (pNode->m_K);
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE V& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetValueAt(PINDEX index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		return (pNode->m_V);
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const V& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetValueAt(PINDEX index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		return (pNode->m_V);
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE V& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::operator[] (PINDEX index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		return (pNode->m_V);
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const V& CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::operator[] (PINDEX index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		return (pNode->m_V);
	}
	throw (EXCEPTION_ACCESS_VIOLATION);
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetFirst(void)
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nFirst] != nullptr)
			{
				return ((PAIR*)m_ppHash[m_nFirst]);
			}
			else
			{
				for (Int i = m_nFirst; i <= m_nLast; ++i)
				{
					if (m_ppHash[i] != nullptr)
					{
						m_nFirst = i;
						return ((PAIR*)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetFirst(void) const
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nFirst] != nullptr)
			{
				return ((const PAIR*)m_ppHash[m_nFirst]);
			}
			else
			{
				for (Int i = m_nFirst; i <= m_nLast; ++i)
				{
					if (m_ppHash[i] != nullptr)
					{
						const_cast<CTMap*>(this)->m_nFirst = i;
						return ((const PAIR*)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetFirstIndex(void) const
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nFirst] != nullptr)
			{
				return ((PINDEX)m_ppHash[m_nFirst]);
			}
			else
			{
				for (Int i = m_nFirst; i <= m_nLast; ++i)
				{
					if (m_ppHash[i] != nullptr)
					{
						const_cast<CTMap*>(this)->m_nFirst = i;
						return ((PINDEX)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetLast(void)
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nLast] != nullptr)
			{
				return ((PAIR*)m_ppHash[m_nLast]);
			}
			else
			{
				for (Int i = m_nLast; i >= m_nFirst; --i)
				{
					if (m_ppHash[i] != nullptr)
					{
						m_nLast = i;
						return ((PAIR*)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetLast(void) const
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nLast] != nullptr)
			{
				return ((const PAIR*)m_ppHash[m_nLast]);
			}
			else
			{
				for (Int i = m_nLast; i >= m_nFirst; --i)
				{
					if (m_ppHash[i] != nullptr)
					{
						const_cast<CTMap*>(this)->m_nLast = i;
						return ((const PAIR*)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE PINDEX CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetLastIndex(void) const
{
	if (m_ppHash != nullptr)
	{
		if (m_nFirst <= m_nLast)
		{
			if (m_ppHash[m_nLast] != nullptr)
			{
				return ((PINDEX)m_ppHash[m_nLast]);
			}
			else
			{
				for (Int i = m_nLast; i >= m_nFirst; --i)
				{
					if (m_ppHash[i] != nullptr)
					{
						const_cast<CTMap*>(this)->m_nLast = i;
						return ((PINDEX)m_ppHash[i]);
					}
				}
			}
		}
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetNext(PINDEX& index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		index = ((PINDEX)pNode->m_pNext);
		if (index == nullptr)
		{
			for (Int i = pNode->m_nHash + 1; i <= m_nLast; ++i)
			{
				if (m_ppHash[i] != nullptr)
				{
					index = (PINDEX)m_ppHash[i];
					break;
				}
			}
		}
		return ((PAIR*)pNode);
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetNext(PINDEX& index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		index = ((PINDEX)pNode->m_pNext);
		if (index == nullptr)
		{
			for (Int i = pNode->m_nHash + 1; i <= m_nLast; ++i)
			{
				if (m_ppHash[i] != nullptr)
				{
					index = (PINDEX)m_ppHash[i];
					break;
				}
			}
		}
		return ((const PAIR*)pNode);
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetPrev(PINDEX& index)
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		index = ((PINDEX)pNode->m_pNext);
		if (index == nullptr)
		{
			for (Int i = pNode->m_nHash - 1; i >= m_nFirst; --i)
			{
				if (m_ppHash[i] != nullptr)
				{
					index = (PINDEX)m_ppHash[i];
					break;
				}
			}
		}
		return ((PAIR*)pNode);
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE const typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::PAIR* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetPrev(PINDEX& index) const
{
	NODE* pNode = static_cast<NODE*>(index);
	if (pNode != nullptr)
	{
		index = ((PINDEX)pNode->m_pNext);
		if (index == nullptr)
		{
			for (Int i = pNode->m_nHash - 1; i >= m_nFirst; --i)
			{
				if (m_ppHash[i] != nullptr)
				{
					index = (PINDEX)m_ppHash[i];
					break;
				}
			}
		}
		return ((const PAIR*)pNode);
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Append(const CTMap<K, V, KTraits, VTraits, nMemType, nOrder>& aSrc)
{
	if (this != &aSrc)
	{
		PINDEX index = aSrc.GetFirstIndex();
		while (index != nullptr)
		{
			const PAIR* p = aSrc.GetNext(index);
			Add(p->m_K, p->m_V);
		}
	}
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Copy(const CTMap<K, V, KTraits, VTraits, nMemType, nOrder>& aSrc)
{
	if (this != &aSrc)
	{
		RemoveAll();
		PINDEX index = aSrc.GetFirstIndex();
		while (index != nullptr)
		{
			const PAIR* p = aSrc.GetNext(index);
			Add(p->m_K, p->m_V);
		}
	}
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::BuildHash(void)
{
	if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
	{
		if (m_ppHash == nullptr)
		{
			m_ppHash = reinterpret_cast<NODE**>( ALLOC( m_nHash * sizeof(NODE*) ) );
			if (m_ppHash != nullptr)
			{
				MM_SAFE::Memset(m_ppHash, 0, m_nHash * sizeof(NODE*));
				return true;
			}
		}
	}
	else if (m_ppHash != nullptr)
	{
		MM_SAFE::Memset(m_ppHash, 0, m_nHash * sizeof(NODE*));
		return true;
	}
	return false;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::Hash(NODE* pNode)
{
	Int nHash = (Int)(KTraits::HashElements(pNode->m_K) & (m_nHash - 1));
	if (nOrder == CContainerTraits::DEFAULT_ORDER_LF)
	{
		NODE* pNext = m_ppHash[nHash];
		pNode->m_nHash = nHash;
		pNode->m_pNext = pNext;
		if (pNext != nullptr)
		{
			pNext->m_pPrev = pNode;
		}
		m_ppHash[nHash] = pNode;
	}
	else
	{
		pNode->m_nHash = nHash;
		pNode->m_pNext = nullptr;

		NODE* pHead = m_ppHash[nHash];
		if (pHead != nullptr)
		{
			if (pHead->m_pPrev != nullptr)
			{
				pHead->m_pPrev->m_pNext = pNode;
				pNode->m_pPrev = pHead->m_pPrev;
				pHead->m_pPrev = pNode;
			}
			else
			{
				assert(pHead->m_pNext == nullptr);
				pHead->m_pNext = pNode;
				pHead->m_pPrev = pNode;
				pNode->m_pPrev = pHead;
			}
		}
		else
		{
			pNode->m_pPrev  = nullptr;
			m_ppHash[nHash] = pNode;
		}
	}
	if (nHash < m_nFirst)
	{
		m_nFirst = nHash;
	}
	if (nHash > m_nLast)
	{
		m_nLast = nHash;
	}
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::ReHash(NODE* pNode)
{
	Int nHash = pNode->m_nHash;
	if (nOrder == CContainerTraits::DEFAULT_ORDER_LF)
	{
		if (m_ppHash[nHash] == pNode)
		{
			m_ppHash[nHash] = pNode->m_pNext;
			if (m_ppHash[nHash] == nullptr)
			{
				if (m_nFirst == nHash)
				{
					m_nFirst = DEF_BASE::Min<Int>((m_nFirst + 1), (m_nHash - 1));
				}
				if (m_nLast == nHash)
				{
					m_nLast = DEF_BASE::Max<Int>((m_nLast - 1), 0);
				}
			}
		}
		if (pNode->m_pPrev != nullptr)
		{
			pNode->m_pPrev->m_pNext = pNode->m_pNext;
		}
		if (pNode->m_pNext != nullptr)
		{
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
		}
	}
	else
	{
		NODE* pHead = m_ppHash[nHash];
		if (pHead == pNode) // head
		{
			if (pHead->m_pPrev == pHead->m_pNext)
			{
				pHead = pHead->m_pNext;
				if (pHead != nullptr)
				{
					pHead->m_pPrev = nullptr;
					assert(pHead->m_pNext == nullptr);
				}
			}
			else
			{
				assert(pHead->m_pNext != nullptr);
				pHead = pHead->m_pNext;
				assert(pHead->m_pNext != nullptr);
				pHead->m_pPrev = pNode->m_pPrev;
			}
			m_ppHash[nHash] = pHead;
		}
		else if (pHead->m_pPrev == pNode) // tail
		{
			assert(pNode->m_pPrev != nullptr);
			assert(pNode->m_pNext == nullptr);
			if (pHead->m_pPrev == pHead->m_pNext)
			{
				pHead->m_pPrev = nullptr;
				pHead->m_pNext = nullptr;
			}
			else
			{
				pHead->m_pPrev = pNode->m_pPrev;
				pNode->m_pPrev->m_pNext = nullptr;
			}
		}
		else
		{
			assert(pNode->m_pPrev != nullptr);
			assert(pNode->m_pNext != nullptr);
			pNode->m_pPrev->m_pNext = pNode->m_pNext;
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
		}
	}
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE bool CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::GetFreeNode(void)
{
	if (m_pFree == nullptr)
	{
		if (nMemType != CContainerTraits::MEM_TYPE_SHARED)
		{
			NODE* pNode = CTContainerAlloc<NODE>::Create(m_pAlloc, (size_t)m_nGrow);
			if (pNode == nullptr)
			{
				throw (EXCEPTION_OUTOFMEMORY);
			}
			pNode += (m_nGrow - 1);
			for (Int i = m_nGrow - 1; i >= 0; --i)
			{
				pNode->m_nHash = -1;// free node flag
				pNode->m_pPrev = nullptr;
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				--pNode;
			}
			m_nAllocSize += m_nGrow;
		}
		else
		{
			if ((m_pShare == nullptr) || (m_nSize >= m_nAllocSize))
			{
				throw (EXCEPTION_OUTOFMEMORY);
			}
			NODE* pNode = m_pShare;
			pNode += (m_nAllocSize - 1);
			for (Int i = m_nAllocSize - 1; i >= 0; --i)
			{
				pNode->m_nHash = -1;// free node flag
				pNode->m_pPrev = nullptr;
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				--pNode;
			}
		}
	}
	return (m_pFree != nullptr);
}

#pragma push_macro("new")
#undef new
template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::NODE* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::NewNode(KINARGTYPE Key)
{
	if (GetFreeNode())
	{
		NODE* pNewNode = m_pFree;
		m_pFree = m_pFree->m_pNext;
		assert(pNewNode->m_nHash == -1); // free flag -1

		GNEW(pNewNode) NODE(Key);

		Hash(pNewNode);

		++m_nSize;
		assert(m_nSize > 0);

		return pNewNode;
	}
	return nullptr;
}

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE typename CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::NODE* CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::NewNode(KINARGTYPE Key, VINARGTYPE Val)
{
	if (GetFreeNode())
	{
		NODE* pNewNode = m_pFree;
		m_pFree = m_pFree->m_pNext;
		assert(pNewNode->m_nHash == -1); // free flag -1

		GNEW(pNewNode) NODE(Key, Val);

		Hash(pNewNode);

		++m_nSize;
		assert(m_nSize > 0);

		return pNewNode;
	}
	return nullptr;
}
#pragma pop_macro("new")

template <typename K, typename V, class KTraits, class VTraits, Int nMemType, Int nOrder>
INLINE void CTMap<K, V, KTraits, VTraits, nMemType, nOrder>::DeleteNode(NODE* pNode)
{
	ReHash(pNode);

	pNode->~NODE();
	pNode->m_nHash = -1;// free node flag
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;

	assert(m_nSize > 0);
	--m_nSize;
	if (m_nSize == 0)
	{
		RemoveAll();
	}
}

#endif // __CONTAINER_INL__