///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : singleton.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : singleton                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SINGLETON_INL__
#define __SINGLETON_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingleton
template <typename T>
SELECTANY CTSingleton<T>* CTSingleton<T>::ms_pSingleton = nullptr;

template <typename T>
INLINE CTSingleton<T>::CTSingleton(void)
{
}

template <typename T>
INLINE CTSingleton<T>::~CTSingleton(void)
{
}

template <typename T>
INLINE CTSingleton<T>::CTSingleton(const CTSingleton<T>&)
{
}

template <typename T>
INLINE CTSingleton<T>& CTSingleton<T>::operator=(const CTSingleton<T>&)
{
	return (*this);
} 

template <typename T>
INLINE T* CTSingleton<T>::GetInstance(void)
{
	return static_cast<T*>(ms_pSingleton);
} 

template <typename T>
INLINE T* CTSingleton<T>::NewInstance(void)
{
	if (ms_pSingleton == nullptr)
	{
		ms_pSingleton = MNEW CTSingleton<T>;
	}
	assert(ms_pSingleton);
	return static_cast<T*>(ms_pSingleton);
}

template <typename T>
INLINE void CTSingleton<T>::DelInstance(void)
{
	if (ms_pSingleton != nullptr)
	{
		MDELETE ms_pSingleton;
		ms_pSingleton = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonSync
template <typename T>
SELECTANY CTSingletonSync<T>* CTSingletonSync<T>::ms_pSingleton = nullptr;

template <typename T>
INLINE CTSingletonSync<T>::CTSingletonSync(void)
{
}

template <typename T>
INLINE CTSingletonSync<T>::~CTSingletonSync(void)
{
}

template <typename T>
INLINE CTSingletonSync<T>::CTSingletonSync(const CTSingletonSync<T>&)
{
}

template <typename T>
INLINE CTSingletonSync<T>& CTSingletonSync<T>::operator=(const CTSingletonSync<T>&)
{
	return (*this);
}    

template <typename T>
INLINE T* CTSingletonSync<T>::GetInstance(void)
{
	return static_cast<T*>(ms_pSingleton);
} 

template <typename T>
INLINE T* CTSingletonSync<T>::NewInstance(CSyncLock& Lock)
{
	CSyncLockWaitScope scope(Lock);
	if (ms_pSingleton == nullptr)
	{
		ms_pSingleton = MNEW CTSingletonSync<T>;
	}
	assert(ms_pSingleton);
	return static_cast<T*>(ms_pSingleton);
}

template <typename T>
INLINE void CTSingletonSync<T>::DelInstance(CSyncLock& Lock)
{
	CSyncLockWaitScope scope(Lock);
	if (ms_pSingleton != nullptr)
	{
		MDELETE ms_pSingleton;
		ms_pSingleton = nullptr;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonInst
template <typename T>
SELECTANY CTSingletonInst<T> CTSingletonInst<T>::ms_Singleton;

template <typename T>
INLINE CTSingletonInst<T>::CTSingletonInst(void)
{
}

template <typename T>
INLINE CTSingletonInst<T>::~CTSingletonInst(void)
{
}

template <typename T>
INLINE CTSingletonInst<T>::CTSingletonInst(const CTSingletonInst<T>&)
{
}

template <typename T>
INLINE CTSingletonInst<T>& CTSingletonInst<T>::operator=(const CTSingletonInst<T>&)
{
	return (*this);
} 

template <typename T>
INLINE T* CTSingletonInst<T>::GetInstance(void)
{
	return static_cast<T*>(&ms_Singleton);
} 

template <typename T>
INLINE T* CTSingletonInst<T>::NewInstance(void)
{
	return static_cast<T*>(&ms_Singleton);
}

template <typename T>
INLINE void CTSingletonInst<T>::DelInstance(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTSingletonUM : singleton create/destroy in system heap
template <typename T>
SELECTANY CTSingletonUM<T>* CTSingletonUM<T>::ms_pSingleton = nullptr;

template <typename T>
INLINE CTSingletonUM<T>::CTSingletonUM(void)
{
}

template <typename T>
INLINE CTSingletonUM<T>::~CTSingletonUM(void)
{
}

template <typename T>
INLINE CTSingletonUM<T>::CTSingletonUM(const CTSingletonUM<T>&)
{
}

template <typename T>
INLINE CTSingletonUM<T>& CTSingletonUM<T>::operator=(const CTSingletonUM<T>&)
{
	return (*this);
} 

template <typename T>
INLINE T* CTSingletonUM<T>::GetInstance(void)
{
	return static_cast<T*>(ms_pSingleton);
} 

template <typename T>
INLINE T* CTSingletonUM<T>::NewInstance(void)
{
	if (ms_pSingleton == nullptr)
	{
		ms_pSingleton = new CTSingletonUM<T>;
	}
	assert(ms_pSingleton);
	return static_cast<T*>(ms_pSingleton);
}

template <typename T>
INLINE void CTSingletonUM<T>::DelInstance(void)
{
	if (ms_pSingleton != nullptr)
	{
		delete ms_pSingleton;
		ms_pSingleton = nullptr;
	}
}

#endif // __SINGLETON_INL__