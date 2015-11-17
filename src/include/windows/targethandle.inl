///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targethandle.inl                                                                      //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows handle                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_HANDLE_INL__
#define __TARGET_HANDLE_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHandle : basic Kernel Objects handle, closed by CloseHandle
INLINE CHandle::CHandle(KeyRef kr)
: m_krHandle(kr)
{
}

INLINE CHandle::~CHandle(void)
{
	Close();
}

INLINE CHandle::CHandle(const CHandle& aSrc)
: m_krHandle(aSrc.m_krHandle)
{
}

INLINE CHandle& CHandle::operator=(const CHandle& aSrc)
{
	if (this != &aSrc)
	{
		Attach(aSrc.m_krHandle);
	}
	return (*this);
}

INLINE CHandle& CHandle::operator=(KeyRef kr)
{
	if (m_krHandle != kr)
	{
		Attach(kr);
	}
	return (*this);
}

INLINE bool CHandle::operator==(KeyRef kr) const
{
	return (m_krHandle == kr);
}

INLINE bool CHandle::operator!=(KeyRef kr) const
{
	return (m_krHandle != kr);
}

INLINE bool CHandle::operator==(const CHandle& aSrc) const
{
	return (m_krHandle == aSrc.m_krHandle);
}

INLINE bool CHandle::operator!=(const CHandle& aSrc) const
{
	return (m_krHandle != aSrc.m_krHandle);
}

INLINE CHandle::operator KeyRef(void) const
{
	return m_krHandle;
}

INLINE bool CHandle::IsValid(void) const
{
	return ((m_krHandle != nullptr) && (m_krHandle != KEYREF_INVALID));
}

INLINE void CHandle::Attach(KeyRef kr)
{
	Close();
	m_krHandle = kr;
}

INLINE KeyRef CHandle::Detach(void)
{
	KeyRef kr  = m_krHandle;
	m_krHandle = nullptr;
	return kr;
}

INLINE void CHandle::Close(void)
{
	if ((m_krHandle != nullptr) && (m_krHandle != KEYREF_INVALID))
	{
		::CloseHandle(m_krHandle);
	}
	m_krHandle = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// module: basic Kernel Objects handle, closed by FreeLibrary(dll or exe)
INLINE CModuleHandle::CModuleHandle(LibRef lr)
: m_lrHandle(lr)
{
}

INLINE CModuleHandle::~CModuleHandle(void)
{
	Close();
}

INLINE CModuleHandle::CModuleHandle(const CModuleHandle& aSrc)
: m_lrHandle(aSrc.m_lrHandle)
{
}

INLINE CModuleHandle& CModuleHandle::operator=(const CModuleHandle& aSrc)
{
	if (this != &aSrc)
	{
		Attach(aSrc.m_lrHandle);
	}
	return (*this);
}

INLINE CModuleHandle& CModuleHandle::operator=(LibRef lr)
{
	if (m_lrHandle != lr)
	{
		Attach(lr);
	}
	return (*this);
}

INLINE bool CModuleHandle::operator==(LibRef lr) const
{
	return (m_lrHandle == lr);
}

INLINE bool CModuleHandle::operator!=(LibRef lr) const
{
	return (m_lrHandle != lr);
}

INLINE bool CModuleHandle::operator==(const CModuleHandle& aSrc) const
{
	return (m_lrHandle == aSrc.m_lrHandle);
}

INLINE bool CModuleHandle::operator!=(const CModuleHandle& aSrc) const
{
	return (m_lrHandle != aSrc.m_lrHandle);
}

INLINE CModuleHandle::operator LibRef(void) const
{
	return m_lrHandle;
}

INLINE bool CModuleHandle::IsValid(void) const
{
	return (m_lrHandle != nullptr);
}

INLINE void CModuleHandle::Attach(LibRef lr)
{
	Close();
	m_lrHandle = lr;
}

INLINE LibRef CModuleHandle::Detach(void)
{
	LibRef lr  = m_lrHandle;
	m_lrHandle = nullptr;
	return lr;
}

INLINE void CModuleHandle::Close(void)
{
	if (m_lrHandle != nullptr)
	{
		::FreeLibrary(m_lrHandle);
		m_lrHandle = nullptr;
	}
}

INLINE FARPROC CModuleHandle::Find(PCStr pszSymbol)
{
	if (m_lrHandle != nullptr)
	{
		return ::GetProcAddress(m_lrHandle, pszSymbol);
	}
	return nullptr;
}

INLINE bool CModuleHandle::Load(PCXStr pszPath)
{
	Close();
	m_lrHandle = ::LoadLibrary(pszPath);
	return (m_lrHandle != nullptr);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_HANDLE_INL__