///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : exception.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : exception base                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __EXCEPTION_INL__
#define __EXCEPTION_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CException
INLINE CException::CException(UInt uCode)
: m_uCode(uCode)
{
}

INLINE CException::CException(UInt uCode, PCXStr pszInfo)
: m_uCode(uCode)
, m_strInfo(pszInfo)
{
}

INLINE CException::~CException(void)
{
}

INLINE CException::CException(UInt uCode, const CString& strInfo)
: m_uCode(uCode)
, m_strInfo(strInfo)
{
}

INLINE CException::CException(const CException& aSrc)
: m_uCode(aSrc.m_uCode)
, m_strInfo(aSrc.m_strInfo)
{
}

INLINE CException& CException::operator=(const CException& aSrc)
{
	if (this != &aSrc)
	{
		m_uCode   = aSrc.m_uCode;
		m_strInfo = aSrc.m_strInfo;
	}
	return (*this);
}

INLINE UInt CException::GetCode(void) const
{
	return m_uCode;
}

INLINE void CException::SetCode(UInt uCode)
{
	m_uCode = uCode;
}

INLINE const CString& CException::GetInfo(void) const
{
	return m_strInfo;
}

INLINE void CException::SetInfo(PCXStr pszInfo)
{
	m_strInfo = pszInfo;
}

#endif // __EXCEPTION_INL__