///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : refcount.h                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : smartpointer                                                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REF_COUNT_H__
#define __REF_COUNT_H__

#pragma once

#include "sync.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRefCount : root class of all Reference Counting
template <typename T> 
class NOVTABLE CTRefCount ABSTRACT : public MObject
{
public:
	virtual Long AddRef(void);
	virtual Long Release(void);
protected:
	CTRefCount(void);
	virtual ~CTRefCount(void);
private:
	CTRefCount(const CTRefCount&);
	CTRefCount& operator=(const CTRefCount&);
protected:
	CSyncCounter   m_Counter;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRefCountPtr : smart pointer, Class T must be derived from CTRefCount
template <typename T>
class CTRefCountPtr : public MObject
{
public:
	CTRefCountPtr(T* pRefCount = nullptr, bool bAddRef = true);
	~CTRefCountPtr(void);
	CTRefCountPtr(const CTRefCountPtr& aSrc);
	template <typename X> CTRefCountPtr(const CTRefCountPtr<X>& aSrc);

	CTRefCountPtr& operator=(const CTRefCountPtr& aSrc);
	template <typename X> CTRefCountPtr& operator=(const CTRefCountPtr<X>& aSrc);
	CTRefCountPtr& operator=(T* pRefCount);
	template <typename X> CTRefCountPtr& operator=(X* pRefCount);

	operator T*(void)   const;
	T& operator*(void)  const;
	T* operator->(void) const;
	T* Get(void)        const;

	bool operator==(T* pRefCount) const;
	bool operator!=(T* pRefCount) const;
	bool operator==(const CTRefCountPtr& aSrc) const;
	template <typename X> bool operator==(const CTRefCountPtr<X>& aSrc) const;
	bool operator!=(const CTRefCountPtr& aSrc) const;
	template <typename X> bool operator!=(const CTRefCountPtr<X>& aSrc) const;

	template <typename X> CTRefCountPtr<X>& Cast(void);
private:
	T*   m_pRefCount;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSharePtr : smartpointer
template <typename T> 
class CTSharePtr : public MObject
{
private:
    template <typename X> friend class CTSharePtr;
    template <typename X> friend class CTWeakPtr;
public:
	CTSharePtr(T* pObject = nullptr);
	~CTSharePtr(void);
	CTSharePtr(const CTSharePtr& aSrc);
	template <typename X> CTSharePtr(const CTSharePtr<X>& aSrc);
	template <typename X> CTSharePtr(const CTWeakPtr<X>& aSrc);

	CTSharePtr& operator=(const CTSharePtr& aSrc);
	template <typename X> CTSharePtr& operator=(const CTSharePtr<X>& aSrc);
	template <typename X> CTSharePtr& operator=(const CTWeakPtr<X>& aSrc);
	CTSharePtr& operator=(T* pObject);

	operator T*(void)   const;
	T& operator*(void)  const;
	T* operator->(void) const;
	T* Get(void)        const;

	bool Check(void) const;
	bool operator==(T* pObject) const;
	bool operator!=(T* pObject) const;
	bool operator==(const CTSharePtr& aSrc) const;
	template <typename X> bool operator==(const CTSharePtr<X>& aSrc) const;
	bool operator!=(const CTSharePtr& aSrc) const;
	template <typename X> bool operator!=(const CTSharePtr<X>& aSrc) const;
private:
	T*              m_pObject;
	CSyncCounter*   m_pCounter;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTWeakPtr : smartpointer
template <typename T> 
class CTWeakPtr : public MObject
{
private:
    template <typename X> friend class CTWeakPtr;
    template <typename X> friend class CTSharePtr;
public:
	CTWeakPtr(void);
	~CTWeakPtr(void);
	CTWeakPtr(const CTWeakPtr& aSrc);
	template <typename X> CTWeakPtr(const CTWeakPtr<X>& aSrc);
	template <typename X> CTWeakPtr(const CTSharePtr<X>& aSrc);

	CTWeakPtr& operator=(const CTWeakPtr& aSrc);
	template <typename X> CTWeakPtr& operator=(const CTWeakPtr<X>& aSrc);
	template <typename X> CTWeakPtr& operator=(const CTSharePtr<X>& aSrc);

	operator T*(void)   const;
	T& operator*(void)  const;
	T* operator->(void) const;
	T* Get(void)        const;

	bool Check(void) const;
	bool operator==(T* pObject) const;
	bool operator!=(T* pObject) const;
	bool operator==(const CTWeakPtr& aSrc) const;
	template <typename X> bool operator==(const CTWeakPtr<X>& aSrc) const;
	bool operator!=(const CTWeakPtr& aSrc) const;
	template <typename X> bool operator!=(const CTWeakPtr<X>& aSrc) const;
private:
	T*              m_pObject;
	CSyncCounter*   m_pCounter;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTScopePtr : smartpointer
template <typename T> 
class CTScopePtr : public MObject
{
public:
	CTScopePtr(T* pObject = nullptr, bool bManaged = true);
	~CTScopePtr(void);
	CTScopePtr& operator=(T* pObject);

	operator T*(void)   const;
	T& operator*(void)  const;
	T* operator->(void) const;
	T* Get(void)        const;

	bool operator==(T* pObject) const;
	bool operator!=(T* pObject) const;
private:
	CTScopePtr(const CTScopePtr& aSrc);
	CTScopePtr& operator=(const CTScopePtr& aSrc);
private:
	T*     m_pObject;
	bool   m_bManaged;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "refcount.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __REF_COUNT_H__