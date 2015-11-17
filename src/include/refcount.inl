///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : refcount.inl                                                                          //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : smartpointer                                                                               //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REF_COUNT_INL__
#define __REF_COUNT_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRefCount
template <typename T>
INLINE CTRefCount<T>::CTRefCount(void)
{
}

template <typename T>
INLINE CTRefCount<T>::~CTRefCount(void)
{
}

template <typename T>
INLINE CTRefCount<T>::CTRefCount(const CTRefCount<T>&)
{
}

template <typename T>
INLINE CTRefCount<T>& CTRefCount<T>::operator=(const CTRefCount<T>&)
{ 
	return (*this);
}

template <typename T>
INLINE Long CTRefCount<T>::AddRef(void)
{
	return m_Counter.Signal();
}

template <typename T>
INLINE Long CTRefCount<T>::Release(void)
{
	Long lRef = m_Counter.Reset();
	if (lRef == 0)
	{
		MDELETE this;
	}
	return lRef;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTRefCountPtr
template <typename T>
INLINE CTRefCountPtr<T>::CTRefCountPtr(T* pRefCount, bool bAddRef)
: m_pRefCount(pRefCount)
{
	if ((m_pRefCount != nullptr) && (bAddRef == true))
	{ 
		m_pRefCount->AddRef();
	}
}

template <typename T>
INLINE CTRefCountPtr<T>::~CTRefCountPtr(void)
{
	if (m_pRefCount != nullptr) 
	{ 
		m_pRefCount->Release();
	}
	m_pRefCount = nullptr;
}

template <typename T>
INLINE CTRefCountPtr<T>::CTRefCountPtr(const CTRefCountPtr<T>& aSrc)
: m_pRefCount(aSrc.m_pRefCount)
{
	if (m_pRefCount != nullptr) 
	{ 
		m_pRefCount->AddRef();
	}
}

template <typename T>
template <typename X>
INLINE CTRefCountPtr<T>::CTRefCountPtr(const CTRefCountPtr<X>& aSrc)
: m_pRefCount(static_cast<T*>(aSrc.Get()))
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pRefCount != nullptr) 
	{ 
		m_pRefCount->AddRef();
	}
}

template <typename T>
INLINE CTRefCountPtr<T>& CTRefCountPtr<T>::operator=(const CTRefCountPtr<T>& aSrc)
{
	if (m_pRefCount != aSrc.m_pRefCount)
	{
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->Release();
		}
		m_pRefCount = aSrc.m_pRefCount;
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->AddRef();
		}
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTRefCountPtr<T>& CTRefCountPtr<T>::operator=(const CTRefCountPtr<X>& aSrc)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pRefCount != static_cast<T*>(aSrc.Get()))
	{
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->Release();
		}
		m_pRefCount = static_cast<T*>(aSrc.Get());
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->AddRef();
		}
	}
	return (*this);
}

template <typename T>
INLINE CTRefCountPtr<T>& CTRefCountPtr<T>::operator=(T* pRefCount)
{
	if (m_pRefCount != pRefCount)
	{
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->Release();
		}
		m_pRefCount = pRefCount;
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->AddRef();
		}
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTRefCountPtr<T>& CTRefCountPtr<T>::operator=(X* pRefCount)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pRefCount != static_cast<T*>(pRefCount))
	{
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->Release();
		}
		m_pRefCount = static_cast<T*>(pRefCount);
		if (m_pRefCount != nullptr) 
		{ 
			m_pRefCount->AddRef();
		}
	}
	return (*this);
}

template <typename T>
INLINE CTRefCountPtr<T>::operator T*(void) const
{
	return m_pRefCount;
}

template <typename T>
INLINE T& CTRefCountPtr<T>::operator*(void) const
{
	return (*m_pRefCount);
}

template <typename T>
INLINE T* CTRefCountPtr<T>::operator->(void) const
{
	return m_pRefCount;
}

template <typename T>
INLINE T* CTRefCountPtr<T>::Get(void) const
{
	return m_pRefCount;
}

template <typename T>
INLINE bool CTRefCountPtr<T>::operator==(T* pRefCount) const
{
	return (m_pRefCount == pRefCount);
}

template <typename T>
INLINE bool CTRefCountPtr<T>::operator!=(T* pRefCount) const
{
	return (m_pRefCount != pRefCount);
}

template <typename T>
INLINE bool CTRefCountPtr<T>::operator==(const CTRefCountPtr<T>& aSrc) const
{
	return (m_pRefCount == aSrc.m_pRefCount);
}

template <typename T>
template <typename X>
INLINE bool CTRefCountPtr<T>::operator==(const CTRefCountPtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pRefCount == static_cast<T*>(aSrc.Get()));
}

template <typename T>
INLINE bool CTRefCountPtr<T>::operator!=(const CTRefCountPtr<T>& aSrc) const
{
	return (m_pRefCount != aSrc.m_pRefCount);
}

template <typename T>
template <typename X>
INLINE bool CTRefCountPtr<T>::operator!=(const CTRefCountPtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pRefCount != static_cast<T*>(aSrc.Get()));
}

template <typename T>
template <typename X>
INLINE CTRefCountPtr<X>& CTRefCountPtr<T>::Cast(void)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<T, X>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (CTRefCountPtr<X>&)(*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSharePtr
template <typename T>
INLINE CTSharePtr<T>::CTSharePtr(T* pObject)
: m_pObject(pObject)
{
	m_pCounter = MNEW CSyncCounter(1);
	assert(m_pCounter != nullptr);
}

template <typename T>
INLINE CTSharePtr<T>::~CTSharePtr(void)
{
	if (m_pCounter->Reset() == 0)
	{
		MM_SAFE::DELETE_PTR<T>(m_pObject);
		MM_SAFE::DELETE_PTR<CSyncCounter>(m_pCounter);
	}
	m_pObject  = nullptr;
	m_pCounter = nullptr;
}

template <typename T>
INLINE CTSharePtr<T>::CTSharePtr(const CTSharePtr<T>& aSrc)
: m_pObject(aSrc.m_pObject)
{
	m_pCounter = aSrc.m_pCounter;
	if (m_pCounter != nullptr) 
	{ 
		m_pCounter->Signal();
	}
}

template <typename T>
template <typename X>
INLINE CTSharePtr<T>::CTSharePtr(const CTSharePtr<X>& aSrc)
: m_pObject(static_cast<T*>(aSrc.m_pObject))
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	m_pCounter = aSrc.m_pCounter;
	if (m_pCounter != nullptr) 
	{ 
		m_pCounter->Signal();
	}
}

template <typename T>
template <typename X>
INLINE CTSharePtr<T>::CTSharePtr(const CTWeakPtr<X>& aSrc)
: m_pObject(static_cast<T*>(aSrc.m_pObject))
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (sMCheck(aSrc.m_pObject) && sMCheck(aSrc.m_pCounter) && (aSrc.m_pCounter->GetCount() > 0))
	{
		m_pCounter = aSrc.m_pCounter;
		if (m_pCounter != nullptr) 
		{ 
			m_pCounter->Signal();
		}
	}
	else
	{
		m_pObject  = nullptr;
		m_pCounter = MNEW CSyncCounter(1);
		assert(m_pCounter != nullptr);
	}
}

template <typename T>
INLINE CTSharePtr<T>& CTSharePtr<T>::operator=(const CTSharePtr<T>& aSrc)
{
	if (m_pObject != aSrc.m_pObject)
	{
		if (m_pCounter->Reset() == 0)
		{
			MM_SAFE::DELETE_PTR<T>(m_pObject);
			MM_SAFE::DELETE_PTR<CSyncCounter>(m_pCounter);
		}

		m_pObject  = aSrc.m_pObject;
		m_pCounter = aSrc.m_pCounter;
		if (m_pCounter != nullptr) 
		{ 
			m_pCounter->Signal();
		}
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTSharePtr<T>& CTSharePtr<T>::operator=(const CTSharePtr<X>& aSrc)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pObject != static_cast<T*>(aSrc.m_pObject))
	{
		if (m_pCounter->Reset() == 0)
		{
			MM_SAFE::DELETE_PTR<T>(m_pObject);
			MM_SAFE::DELETE_PTR<CSyncCounter>(m_pCounter);
		}

		m_pObject  = static_cast<T*>(aSrc.m_pObject);
		m_pCounter = aSrc.m_pCounter;
		if (m_pCounter != nullptr) 
		{ 
			m_pCounter->Signal();
		}
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTSharePtr<T>& CTSharePtr<T>::operator=(const CTWeakPtr<X>& aSrc)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (sMCheck(aSrc.m_pObject) && sMCheck(aSrc.m_pCounter) && (aSrc.m_pCounter->GetCount() > 0))
	{
		if (m_pObject != static_cast<T*>(aSrc.m_pObject))
		{
			if (m_pCounter->Reset() == 0)
			{
				MM_SAFE::DELETE_PTR<T>(m_pObject);
				MM_SAFE::DELETE_PTR<CSyncCounter>(m_pCounter);
			}

			m_pObject  = static_cast<T*>(aSrc.m_pObject);
			m_pCounter = aSrc.m_pCounter;
			if (m_pCounter != nullptr) 
			{ 
				m_pCounter->Signal();
			}
		}
	}
	return (*this);
}

template <typename T>
INLINE CTSharePtr<T>& CTSharePtr<T>::operator=(T* pObject)
{
	if (m_pObject != pObject)
	{
		if (m_pCounter->Reset() == 0)
		{
			MM_SAFE::DELETE_PTR<T>(m_pObject);
			MM_SAFE::DELETE_PTR<CSyncCounter>(m_pCounter);
		}
		m_pObject  = pObject;
		m_pCounter = MNEW CSyncCounter(1);
		assert(m_pCounter != nullptr);
	}
	return (*this);
}

template <typename T>
INLINE CTSharePtr<T>::operator T*(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T& CTSharePtr<T>::operator*(void) const
{
	return (*m_pObject);
}

template <typename T>
INLINE T* CTSharePtr<T>::operator->(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T* CTSharePtr<T>::Get(void) const
{
	return m_pObject;
}

template <typename T>
INLINE bool CTSharePtr<T>::Check(void) const
{
	return (sMCheck(m_pObject) && sMCheck(m_pCounter));
}

template <typename T>
INLINE bool CTSharePtr<T>::operator==(T* pObject) const
{
	return (m_pObject == pObject);
}

template <typename T>
INLINE bool CTSharePtr<T>::operator!=(T* pObject) const
{
	return (m_pObject != pObject);
}

template <typename T>
INLINE bool CTSharePtr<T>::operator==(const CTSharePtr<T>& aSrc) const
{
	return (m_pObject == aSrc.m_pObject);
}

template <typename T>
template <typename X>
INLINE bool CTSharePtr<T>::operator==(const CTSharePtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pObject == static_cast<T*>(aSrc.m_pObject));
}

template <typename T>
INLINE bool CTSharePtr<T>::operator!=(const CTSharePtr<T>& aSrc) const
{
	return (m_pObject != aSrc.m_pObject);
}

template <typename T>
template <typename X>
INLINE bool CTSharePtr<T>::operator!=(const CTSharePtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pObject != static_cast<T*>(aSrc.m_pObject));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTWeakPtr
template <typename T>
INLINE CTWeakPtr<T>::CTWeakPtr(void)
: m_pObject(nullptr)
, m_pCounter(nullptr)
{
}

template <typename T>
INLINE CTWeakPtr<T>::~CTWeakPtr(void)
{
	m_pObject  = nullptr;
	m_pCounter = nullptr;
}

template <typename T>
INLINE CTWeakPtr<T>::CTWeakPtr(const CTWeakPtr<T>& aSrc)
: m_pObject(aSrc.m_pObject)
, m_pCounter(aSrc.m_pCounter)
{
}

template <typename T>
template <typename X>
INLINE CTWeakPtr<T>::CTWeakPtr(const CTWeakPtr<X>& aSrc)
: m_pObject(static_cast<T*>(aSrc.m_pObject))
, m_pCounter(aSrc.m_pCounter)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
}

template <typename T>
template <typename X>
INLINE CTWeakPtr<T>::CTWeakPtr(const CTSharePtr<X>& aSrc)
: m_pObject(static_cast<T*>(aSrc.m_pObject))
, m_pCounter(aSrc.m_pCounter)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
}

template <typename T>
INLINE CTWeakPtr<T>& CTWeakPtr<T>::operator=(const CTWeakPtr<T>& aSrc)
{
	if (m_pObject != aSrc.m_pObject)
	{
		m_pObject  = aSrc.m_pObject;
		m_pCounter = aSrc.m_pCounter;
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTWeakPtr<T>& CTWeakPtr<T>::operator=(const CTWeakPtr<X>& aSrc)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pObject != static_cast<T*>(aSrc.m_pObject))
	{
		m_pObject  = static_cast<T*>(aSrc.m_pObject);
		m_pCounter = aSrc.m_pCounter;
	}
	return (*this);
}

template <typename T>
template <typename X>
INLINE CTWeakPtr<T>& CTWeakPtr<T>::operator=(const CTSharePtr<X>& aSrc)
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	if (m_pObject != static_cast<T*>(aSrc.m_pObject))
	{
		m_pObject  = static_cast<T*>(aSrc.m_pObject);
		m_pCounter = aSrc.m_pCounter;
	}
	return (*this);
}

template <typename T>
INLINE CTWeakPtr<T>::operator T*(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T& CTWeakPtr<T>::operator*(void) const
{
	return (*m_pObject);
}

template <typename T>
INLINE T* CTWeakPtr<T>::operator->(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T* CTWeakPtr<T>::Get(void) const
{
	return m_pObject;
}

template <typename T>
INLINE bool CTWeakPtr<T>::Check(void) const
{
	return (sMCheck(m_pObject) && sMCheck(m_pCounter));
}

template <typename T>
INLINE bool CTWeakPtr<T>::operator==(T* pObject) const
{
	return (m_pObject == pObject);
}

template <typename T>
INLINE bool CTWeakPtr<T>::operator!=(T* pObject) const
{
	return (m_pObject != pObject);
}

template <typename T>
INLINE bool CTWeakPtr<T>::operator==(const CTWeakPtr<T>& aSrc) const
{
	return (m_pObject == aSrc.m_pObject);
}

template <typename T>
template <typename X>
INLINE bool CTWeakPtr<T>::operator==(const CTWeakPtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pObject == static_cast<T*>(aSrc.m_pObject));
}

template <typename T>
INLINE bool CTWeakPtr<T>::operator!=(const CTWeakPtr<T>& aSrc) const
{
	return (m_pObject != aSrc.m_pObject);
}

template <typename T>
template <typename X>
INLINE bool CTWeakPtr<T>::operator!=(const CTWeakPtr<X>& aSrc) const
{
#ifdef RUNTIME_DEBUG
	typename TConvertibleDetection<X, T>::TAG tag = ConvertibleTag();
	UNREFERENCED_PARAMETER( tag );
#endif  // RUNTIME_DEBUG
	return (m_pObject != static_cast<T*>(aSrc.m_pObject));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTScopePtr
template <typename T>
INLINE CTScopePtr<T>::CTScopePtr(T* pObject, bool bManaged)
: m_pObject(pObject)
, m_bManaged(bManaged)
{
}

template <typename T>
INLINE CTScopePtr<T>::~CTScopePtr(void)
{
	if (m_bManaged)
	{
		MM_SAFE::DELETE_PTRREF<T>(m_pObject);
	}
	else
	{
		m_pObject = nullptr;
	}
}

template <typename T>
INLINE CTScopePtr<T>::CTScopePtr(const CTScopePtr<T>&)
: m_pObject(nullptr)
, m_bManaged(true)
{
}

template <typename T>
INLINE CTScopePtr<T>& CTScopePtr<T>::operator=(const CTScopePtr<T>&)
{
	return (*this);
}

template <typename T>
INLINE CTScopePtr<T>& CTScopePtr<T>::operator=(T* pObject)
{
	if (m_pObject != pObject)
	{
		if (m_bManaged)
		{
			MM_SAFE::DELETE_PTR<T>(m_pObject);
		}
		m_pObject = pObject;
	}
	return (*this);
}

template <typename T>
INLINE CTScopePtr<T>::operator T*(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T& CTScopePtr<T>::operator*(void) const
{
	return (*m_pObject);
}

template <typename T>
INLINE T* CTScopePtr<T>::operator->(void) const
{
	return m_pObject;
}

template <typename T>
INLINE T* CTScopePtr<T>::Get(void) const
{
	return m_pObject;
}

template <typename T>
INLINE bool CTScopePtr<T>::operator==(T* pObject) const
{
	return (m_pObject == pObject);
}

template <typename T>
INLINE bool CTScopePtr<T>::operator!=(T* pObject) const
{
	return (m_pObject != pObject);
}

#endif // __REF_COUNT_INL__