///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : subsystem.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : subsystem load helper                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SUBSYSTEM_INL__
#define __SUBSYSTEM_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent
INLINE CComponent::CComponent(void)
{
}

INLINE CComponent::~CComponent(void)
{
}

INLINE CComponent::CComponent(const CComponent&)
{
}

INLINE CComponent& CComponent::operator=(const CComponent&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSubSystem
SELECTANY CPCStr CSubSystem::CreateSubSystem = (CPCStr)("CreateSubSystem");

INLINE CSubSystem::CSubSystem(void)
{
}

INLINE CSubSystem::~CSubSystem(void)
{
}

INLINE CSubSystem::CSubSystem(const CSubSystem&)
{
}

INLINE CSubSystem& CSubSystem::operator=(const CSubSystem&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTLoader
template <typename T>
INLINE CTLoader<T>::CTLoader(void)
: m_pSubSystem(nullptr)
, m_bInited(false)
{
	m_szErrMsg[0] = 0;
	m_szErrMsg[LMT_MAX_PATH - 1] = 0;
}

template <typename T>
INLINE CTLoader<T>::~CTLoader(void)
{
	Unload();
}

template <typename T>
INLINE CTLoader<T>::CTLoader(const CTLoader<T>&)
: m_pSubSystem(nullptr)
, m_bInited(false)
{
}

template <typename T>
INLINE CTLoader<T>& CTLoader<T>::operator=(const CTLoader<T>&)
{
	return (*this);
}

template <typename T>
INLINE T* CTLoader<T>::operator->(void) const
{
	assert(m_pSubSystem != nullptr);
	return m_pSubSystem;
}

template <typename T>
INLINE T* CTLoader<T>::GetSubSystem(void) const
{
	return m_pSubSystem;
}

template <typename T>
INLINE LibRef CTLoader<T>::GetLib(void) const
{
	return m_Module;
}

template <typename T>
INLINE bool CTLoader<T>::IsInited(void) const
{
	return (m_bInited);
}

template <typename T>
INLINE bool CTLoader<T>::IsLoaded(void) const
{
	return (m_pSubSystem != nullptr);
}

template <typename T>
INLINE bool CTLoader<T>::Load(const CUUID& uuId, PCXStr pszLoadPath, bool bInit)
{
	if (m_pSubSystem == nullptr)
	{
		if (pszLoadPath == nullptr)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Load module file failed, path invalid!"));
			return false;
		}
		m_Module.Load(pszLoadPath);
		if (m_Module.IsValid() == false)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Load module file[%s]failed, ERROR ID = %#X, plz check file exists or file is corrupted!"), pszLoadPath, ::GetLastError());
			return false;
		}

		CreateSubSystemFunc* pCreateFunc = reinterpret_cast<CreateSubSystemFunc*>(m_Module.Find((PCSTR)CSubSystem::CreateSubSystem));

		if (pCreateFunc == nullptr)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Interface not found on module[%s, handle=%#X], ERROR ID = %#X, plz check file is corrupted or file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
			return false;
		}

		m_pSubSystem = reinterpret_cast<T*>(pCreateFunc(uuId));
		if (m_pSubSystem == nullptr)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Create interface on module[%s, handle=%#X]failed, ERROR ID = %#X, plz check file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
			return false;
		}

		if (bInit)
		{
			m_bInited = (m_pSubSystem->Init() == RET_OKAY);
			if (m_bInited == false)
			{
				CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("init interface on module[%s, handle=%#X]failed, ERROR ID = %#X, plz check file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
				return false;
			}
		}
	}
	return true;
}

template <typename T>
INLINE void CTLoader<T>::Unload(void)
{
	Long lRet = 0;
	if (m_pSubSystem != nullptr)
	{
		m_pSubSystem->Exit();

		lRet = m_pSubSystem->Release();
		m_pSubSystem = nullptr;
	}
	if ((lRet <= 0) && (m_Module.IsValid() != false))
	{
		m_Module.Close();
	}
}

template <typename T>
INLINE PCXStr CTLoader<T>::GetErrMsg(void) const
{
	return m_szErrMsg;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTLoaderRef
template <typename T>
INLINE CTLoaderRef<T>::CTLoaderRef(LibRef libr)
: m_Module(libr)
, m_pSubSystem(nullptr)
, m_bInited(false)
{
	m_szErrMsg[0] = 0;
	m_szErrMsg[LMT_MAX_PATH - 1] = 0;
	assert(libr != nullptr);
}

template <typename T>
INLINE CTLoaderRef<T>::~CTLoaderRef(void)
{
	Unload();
}

template <typename T>
INLINE CTLoaderRef<T>::CTLoaderRef(const CTLoaderRef<T>&)
: m_pSubSystem(nullptr)
, m_bInited(false)
{
}

template <typename T>
INLINE CTLoaderRef<T>& CTLoaderRef<T>::operator=(const CTLoaderRef<T>&)
{
	return (*this);
}

template <typename T>
INLINE T* CTLoaderRef<T>::operator->(void) const
{
	assert(m_pSubSystem != nullptr);
	return m_pSubSystem;
}

template <typename T>
INLINE T* CTLoaderRef<T>::GetSubSystem(void) const
{
	return m_pSubSystem;
}

template <typename T>
INLINE LibRef CTLoaderRef<T>::GetLib(void) const
{
	return m_Module;
}

template <typename T>
INLINE bool CTLoaderRef<T>::IsInited(void) const
{
	return (m_bInited);
}

template <typename T>
INLINE bool CTLoaderRef<T>::IsLoaded(void) const
{
	return (m_pSubSystem != nullptr);
}

template <typename T>
INLINE bool CTLoaderRef<T>::Load(const CUUID& uuId, bool bInit)
{
	if (m_pSubSystem == nullptr)
	{
		if (m_Module.IsValid() == false)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Module file handle is null!"));
			return false;
		}
		CreateSubSystemFunc* pCreateFunc = reinterpret_cast<CreateSubSystemFunc*>(m_Module.Find((PCSTR)CSubSystem::CreateSubSystem));
		if (pCreateFunc == nullptr)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Interface not found on module[%s, handle=%#X], ERROR ID = %#X, plz check file is corrupted or file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
			return false;
		}

		m_pSubSystem = reinterpret_cast<T*>(pCreateFunc(uuId));
		if (m_pSubSystem == nullptr)
		{
			CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("Create interface on module[%s, handle=%#X]failed, ERROR ID = %#X, plz check file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
			return false;
		}

		if (bInit)
		{
			m_bInited = (m_pSubSystem->Init() == RET_OKAY);
			if (m_bInited == false)
			{
				CXChar::Format(m_szErrMsg, LMT_MAX_PATH, TF("init interface on module[%s, handle=%#X]failed, ERROR ID = %#X, plz check file version is matched!"), pszLoadPath, m_Module.m_lrHandle, ::GetLastError());
				return false;
			}
		}
	}
	return true;
}

template <typename T>
INLINE void CTLoaderRef<T>::Unload(void)
{
	if (m_pSubSystem != nullptr)
	{
		m_pSubSystem->Exit();
		m_pSubSystem->Release();
		m_pSubSystem = nullptr;
	}
	m_Module.Detach();
}

template <typename T>
INLINE PCXStr CTLoaderRef<T>::GetErrMsg(void) const
{
	return m_szErrMsg;
}

#endif // __SUBSYSTEM_INL__