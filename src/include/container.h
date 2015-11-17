///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : container.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : common container                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#pragma once

#include "traits.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTContainerAlloc
template <typename T>
class CTContainerAlloc : public MObject
{
public:
	static T* Create(CTContainerAlloc*& pThis, size_t stElements);

	T*     GetData(void);
	void   Destroy(void);
private:
	CTContainerAlloc(void);
	~CTContainerAlloc(void);
	CTContainerAlloc(const CTContainerAlloc&);
	CTContainerAlloc& operator=(const CTContainerAlloc&);
private:
	CTContainerAlloc*   m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CContainerTraits
class CContainerTraits
{
public:
	enum MEM_TYPE
	{
		MEM_TYPE_NORMAL = 0,
		MEM_TYPE_SHARED,
	};

	enum DEFAULT_SIZE
	{
		DEFAULT_SIZE_MIN   = 4,
		DEFAULT_SIZE_GROW  = 32,
		DEFAULT_SIZE_MAX   = INT_MAX,
	};

	enum DEFAULT_HASH
	{
		DEFAULT_HASH_MIN   = 32,
		DEFAULT_HASH_SIZE  = 1024,
		DEFAULT_HASH_MAX   = INT_MAX,
	};

	enum DEFAULT_ORDER
	{
		DEFAULT_ORDER_LF = 0,
		DEFAULT_ORDER_FF,
	};
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTArray
template <typename T, class Traits = CTElementTraits<T>, Int nMemType = CContainerTraits::MEM_TYPE_NORMAL>
class CTArray : public MObject
{
public:
	typedef typename Traits::INARGTYPE INARGTYPE;
	typedef typename Traits::OUTARGTYPE OUTARGTYPE;
public:
	CTArray(Int nGrow = CContainerTraits::DEFAULT_SIZE_GROW); 
	~CTArray(void);

	CTArray(const CTArray& aSrc);
	CTArray& operator=(const CTArray& aSrc);
#ifdef __CXX11_IMP__
	CTArray(CTArray&& aSrc);
#endif
	Int      GetAllocSize(void) const;
	Int      GetSize(void) const;
	Int      GetGrow(void) const;
	void     SetGrow(Int nGrow);
	// ---memory share used only---
	Int      GetShare(Int nAllocSize) const;
	bool     SetShare(void* pShare, Int nAllocSize);
	// ------

	Int      Add(INARGTYPE t);
	Int      Add(Int nCount, bool bZeroed);

	Int      Find(INARGTYPE t) const;

	bool     Remove(INARGTYPE t);
	bool     RemoveAt(Int nIndex);
	void     RemoveAll(void);

	bool     Check(Int nIndex) const;

	bool     Insert(Int nIndex, Int nCount = 1, bool bZeroed = false);
	bool     SetAt(Int nIndex, INARGTYPE t);
	bool     Shrink(void);

	T&       operator[] (Int nIndex);
	const T& operator[] (Int nIndex) const;

	const T* GetData(void) const;

	T*       GetItem(Int nIndex);
	const T* GetItem(Int nIndex) const;

	bool     Append(const CTArray& aSrc);
	bool     Copy(const CTArray& aSrc);
private:
	bool     SetCount(Int nNewCount = 1);
	void     SetAtIndex(Int nIndex, INARGTYPE t);
private:
	T*       m_paT;
	Int      m_nAllocSize;
	Int      m_nSize;
	Int      m_nGrow;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTList
template <typename T, class Traits = CTElementTraits<T>, Int nMemType = CContainerTraits::MEM_TYPE_NORMAL>
class CTList : public MObject
{
public:
	typedef typename Traits::INARGTYPE INARGTYPE;
private:
	class NODE : public INDEX
	{
	public:
		NODE(NODE* pPrev, NODE* pNext) 
		: m_pNext(pNext)
		, m_pPrev(pPrev)
		{ 
		}

		NODE(INARGTYPE t, NODE* pPrev, NODE* pNext) 
		: m_pNext(pNext)
		, m_pPrev(pPrev)
		, m_T(t)
		{
		}

		NODE(const NODE& aSrc)
		: m_pNext(aSrc.m_pNext)
		, m_pPrev(aSrc.m_pPrev)
		, m_T(aSrc.m_T)
		{
		}

		NODE& operator=(const NODE& aSrc)
		{
			m_pNext = aSrc.m_pNext;
			m_pPrev = aSrc.m_pPrev;
			m_T     = aSrc.m_T;
			return (*this);
		}

		~NODE(void)
		{ 
			m_pNext = nullptr;
			m_pPrev = nullptr;
		}
	public:
		NODE*   m_pNext;
		NODE*   m_pPrev;
		T       m_T;
	};
public:
	CTList(Int nGrow = CContainerTraits::DEFAULT_SIZE_GROW); 
	~CTList(void);

	CTList(const CTList& aSrc);
	CTList& operator=(const CTList& aSrc);
#ifdef __CXX11_IMP__
	CTList(CTList&& aSrc);
#endif

	Int      GetAllocSize(void) const;
	Int      GetSize(void) const;
	Int      GetGrow(void) const;
	void     SetGrow(Int nGrow);
	// ---memory share used only---
	Int      GetShare(Int nAllocSize) const;
	bool     SetShare(void* pShare, Int nAllocSize);
	// ------

	PINDEX   AddHead(void);
	PINDEX   AddHead(INARGTYPE t);
	void     AddHeadList(const CTList& aSrc);
	PINDEX   AddTail(void);
	PINDEX   AddTail(INARGTYPE t);
	void     AddTailList(const CTList& aSrc);

	PINDEX   Find(INARGTYPE t, PINDEX pStartAfter = nullptr) const;
	PINDEX   FindIndex(Int nIndex) const;

	T        RemoveHead(void);
	void     RemoveHeadDirect(void);
	T        RemoveTail(void);
	void     RemoveTailDirect(void);

	bool     Remove(INARGTYPE t);
	bool     RemoveAt(PINDEX index);
	void     RemoveAll(void);

	bool     Check(PINDEX index) const;

	PINDEX   InsertBefore(PINDEX index, INARGTYPE t);
	PINDEX   InsertAfter(PINDEX index, INARGTYPE t);

	bool     MoveToHead(PINDEX index);
	bool     MoveToTail(PINDEX index);
	bool     Swap(PINDEX index1, PINDEX index2);

	bool     SetAt(PINDEX index, INARGTYPE t);

	T&       GetAt(PINDEX index);
	const T& GetAt(PINDEX index) const;

	T&       GetHead(void);
	const T& GetHead(void) const;
	PINDEX   GetHeadIndex(void) const;

	T&       GetTail(void);
	const T& GetTail(void) const;
	PINDEX   GetTailIndex(void) const;

	T&       GetNext(PINDEX& index);
	const T& GetNext(PINDEX& index) const;
	T&       GetPrev(PINDEX& index);
	const T& GetPrev(PINDEX& index) const;
private:
	void     FreeNode(NODE* pNode);
	bool     GetFreeNode(void);
	NODE*    NewNode(NODE* pPrev, NODE* pNext);
	NODE*    NewNode(INARGTYPE t, NODE* pPrev, NODE* pNext);
private :
	NODE*                     m_pHead;
	NODE*                     m_pTail;
	NODE*                     m_pFree;
	NODE*                     m_pShare;
	CTContainerAlloc<NODE>*   m_pAlloc;
	Int                       m_nAllocSize;
	Int                       m_nSize;
	Int                       m_nGrow;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTQueue
template <typename T, class Traits = CTElementTraits<T>, Int nMemType = CContainerTraits::MEM_TYPE_NORMAL>
class CTQueue : public MObject
{
public:
	typedef typename Traits::INARGTYPE INARGTYPE;
public:
	CTQueue(Int nGrow = CContainerTraits::DEFAULT_SIZE_GROW); 
	~CTQueue(void);

	Int      GetSize(void) const;
	void     SetGrow(Int nGrow);
	// ---memory share used only---
	Int      GetShare(Int nAllocSize) const;
	bool     SetShare(void* pShare, Int nAllocSize);
	// ------

	bool     IsEmpty(void) const;
	bool     IsFull(void) const;

	bool     In(INARGTYPE t);
	T	     Out(void);

	void     RemoveAll(void);

	T&       GetItem(void);
	const T& GetItem(void) const;
private:
	CTQueue(const CTQueue& aSrc);
	CTQueue& operator=(const CTQueue& aSrc);
private:
	CTList<T, Traits, nMemType>   m_List;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTStack
template <typename T, class Traits = CTElementTraits<T>, Int nMemType = CContainerTraits::MEM_TYPE_NORMAL>
class CTStack : public MObject
{
public:
	typedef typename Traits::INARGTYPE INARGTYPE;
public:
	CTStack(Int nGrow = CContainerTraits::DEFAULT_SIZE_GROW); 
	~CTStack(void);

	Int      GetSize(void) const;
	void     SetGrow(Int nGrow);
	// ---memory share used only---
	Int      GetShare(Int nAllocSize) const;
	bool     SetShare(void* pShare, Int nAllocSize);
	// ------

	bool     IsEmpty(void) const;
	bool     IsFull(void) const;

	bool     Push(INARGTYPE t);
	T	     Pop(void);

	void     RemoveAll(void);

	T&       GetItem(void);
	const T& GetItem(void) const;
private:
	CTStack(const CTStack& aSrc);
	CTStack& operator=(const CTStack& aSrc);
private:
	CTList<T, Traits, nMemType>   m_List;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTMap
template <typename K, typename V, class KTraits = CTElementTraits<K>, class VTraits = CTElementTraits<V>, Int nMemType = CContainerTraits::MEM_TYPE_NORMAL, Int nOrder = CContainerTraits::DEFAULT_ORDER_LF>
class CTMap : public MObject
{
public:
	typedef typename KTraits::INARGTYPE KINARGTYPE;
	typedef typename KTraits::OUTARGTYPE KOUTARGTYPE;
	typedef typename VTraits::INARGTYPE VINARGTYPE;
	typedef typename VTraits::OUTARGTYPE VOUTARGTYPE;
public:
	class PAIR : public INDEX
	{
	protected:
		PAIR(KINARGTYPE tK) 
		: m_K(tK)
		{ 
		}

		PAIR(KINARGTYPE tK, VINARGTYPE tV) 
		: m_K(tK)
		, m_V(tV)
		{ 
		}

		PAIR(const PAIR& aSrc)
		: m_K(aSrc.m_K)
		, m_V(aSrc.m_V)
		{
		}

		PAIR& operator=(const PAIR& aSrc)
		{
			m_K = aSrc.m_K;
			m_V = aSrc.m_V;
			return (*this);
		}

		~PAIR(void)
		{ 
		}
	public:
		const K   m_K;
		V         m_V;
	};
private :
	class NODE : public PAIR
	{
	public:
		NODE(KINARGTYPE tK) 
		: PAIR(tK)
		, m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_nHash(0)
		{ 
		}

		NODE(KINARGTYPE tK, VINARGTYPE tV) 
		: PAIR(tK, tV)
		, m_pNext(nullptr)
		, m_pPrev(nullptr)
		, m_nHash(0)
		{ 
		}

		NODE(const NODE& aSrc)
		: PAIR(aSrc.m_K, aSrc.m_V)
		, m_pNext(aSrc.m_pNext)
		, m_pPrev(aSrc.m_pPrev)
		, m_nHash(aSrc.m_nHash)
		{
		}

		NODE& operator=(const NODE& aSrc)
		{
			m_K     = aSrc.m_K;
			m_V     = aSrc.m_V;
			m_pNext = aSrc.m_pNext;
			m_pPrev = aSrc.m_pPrev;
			m_nHash = aSrc.m_nHash;
			return (*this);
		}

		~NODE(void)
		{
			m_pNext = nullptr;
			m_pPrev = nullptr;
			m_nHash = 0;
		}
	public:
		NODE*   m_pNext;
		NODE*   m_pPrev;
		Int     m_nHash;
	};
public :
	CTMap(Int nGrow = CContainerTraits::DEFAULT_SIZE_GROW, Int nHash = CContainerTraits::DEFAULT_HASH_SIZE);
	~CTMap(void);

	CTMap(const CTMap& aSrc);
	CTMap& operator=(const CTMap& aSrc);
#ifdef __CXX11_IMP__
	CTMap(CTMap&& aSrc);
#endif
	Int         GetAllocSize(void) const;
	Int         GetSize(void) const;
	Int         GetGrow(void) const;
	void        SetGrow(Int nGrow);
	Int         GetHash(void) const;
	void        SetHash(Int nHash);
	// ---memory share used only---
	Int         GetShare(Int nAllocSize) const;
	bool        SetShare(void* pShare, Int nAllocSize);
	// ------

	PINDEX      Add(KINARGTYPE Key);
	PINDEX      Add(KINARGTYPE Key, VINARGTYPE Val);

	PAIR*       Find(KINARGTYPE Key, PINDEX pStartAfter = nullptr);
	const PAIR* Find(KINARGTYPE Key, PINDEX pStartAfter = nullptr) const;
	PINDEX      FindIndex(KINARGTYPE Key, PINDEX pStartAfter = nullptr) const;

	bool        Remove(KINARGTYPE Key);
	bool        RemoveAt(PINDEX index);
	void        RemoveAll(void);

	bool        Check(PINDEX index) const;

	// if can not find the key, add a new node
	PINDEX      SetAt(KINARGTYPE Key, VINARGTYPE Val);
	bool        SetAtIndex(PINDEX index, VINARGTYPE Val);

	PAIR*       GetAt(PINDEX index);
	const PAIR* GetAt(PINDEX index) const;
	const K&    GetKeyAt(PINDEX index) const;
	V&          GetValueAt(PINDEX index);
	const V&    GetValueAt(PINDEX index) const;

	V&          operator[] (PINDEX index);
	const V&    operator[] (PINDEX index) const;

	PAIR*       GetFirst(void);
	const PAIR* GetFirst(void) const;
	PINDEX      GetFirstIndex(void) const;

	PAIR*       GetLast(void);
	const PAIR* GetLast(void) const;
	PINDEX      GetLastIndex(void) const;

	PAIR*       GetNext(PINDEX& index);
	const PAIR* GetNext(PINDEX& index) const;
	PAIR*       GetPrev(PINDEX& index);
	const PAIR* GetPrev(PINDEX& index) const;

	void        Append(const CTMap& aSrc);
	void        Copy(const CTMap& aSrc);
private :	
	bool        BuildHash(void);
	void        Hash(NODE* pNode);
	void        ReHash(NODE* pNode);

	bool        GetFreeNode(void);
	NODE*       NewNode(KINARGTYPE Key);
	NODE*       NewNode(KINARGTYPE Key, VINARGTYPE Val);
	void        DeleteNode(NODE* pNode);
private :
	NODE*                     m_pFree;
	NODE*                     m_pShare;
	NODE**                    m_ppHash;
	CTContainerAlloc<NODE>*   m_pAlloc;
	Int                       m_nAllocSize;
	Int                       m_nSize;
	Int                       m_nGrow;
	Int                       m_nHash;
	Int                       m_nFirst;
	Int                       m_nLast;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "container.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __CONTAINER_H__