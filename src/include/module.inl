///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : module.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : lib load helper                                                                            //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MODULE_INL__
#define __MODULE_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CModule
INLINE CModule::CModule(LibRef libr, bool bUnload)
: m_bUnload(bUnload)
, m_Module(libr)
{
}

INLINE CModule::~CModule(void)
{
	Close();
}

INLINE CModule::CModule(const CModule& aSrc)
: m_bUnload(false)
, m_Module(aSrc.m_Module)
{
}

INLINE CModule& CModule::operator=(const CModule& aSrc)
{
	if (this != &aSrc)
	{
		Close();
		m_bUnload           = false;
		m_Module.m_lrHandle = aSrc.m_Module.m_lrHandle;
	}
	return (*this);
}

INLINE CModule& CModule::operator=(LibRef libr)
{
	if (m_Module != libr)
	{
		Close();
		m_bUnload           = true;
		m_Module.m_lrHandle = libr;
	}
	return (*this);
}

INLINE bool CModule::operator==(LibRef libr) const
{
	return (m_Module == libr);
}

INLINE bool CModule::operator!=(LibRef libr) const
{
	return (m_Module != libr);
}

INLINE bool CModule::operator==(const CModule& aSrc) const
{
	return (m_Module == aSrc.m_Module);
}

INLINE bool CModule::operator!=(const CModule& aSrc) const
{
	return (m_Module != aSrc.m_Module);
}

INLINE CModule::operator LibRef(void) const
{
	return m_Module.m_lrHandle;
}

template <typename I>
INLINE I CModule::Find(PCStr pszSymbol)
{
	return reinterpret_cast<I>(m_Module.Find(pszSymbol));
}

INLINE bool CModule::Attach(PCXStr pszPath, bool bUnload)
{
	Close();
	m_bUnload = bUnload;
	return m_Module.Load(pszPath);
}

INLINE void CModule::Attach(LibRef libr, bool bUnload)
{
	Close();
	m_bUnload = bUnload;
	m_Module.m_lrHandle = libr;
}

INLINE LibRef CModule::Detach(void)
{
	m_bUnload = false;
	return m_Module.Detach();
}

INLINE void CModule::Close(void)
{
	if (m_bUnload == true)
	{
		m_Module.Close();
	}
	else
	{
		m_Module.Detach();
	}
	m_bUnload = false;
}

#endif // __MODULE_INL__