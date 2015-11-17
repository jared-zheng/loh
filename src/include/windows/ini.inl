///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : ini.inl                                                                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : ini windows                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __INI_INL__
#define __INI_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIni
INLINE CIni::CIni(PCXStr pszFile)
{ 
	SetINI(pszFile);
}

INLINE CIni::~CIni(void)
{
}

INLINE CIni::CIni(const CIni&)
{
}

INLINE CIni& CIni::operator=(const CIni&)
{
	return (*this);
}

INLINE bool CIni::CreateINI(PCXStr pszFile)
{
	HANDLE h = ::CreateFile(pszFile, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (h != INVALID_HANDLE_VALUE)
	{  
		::CloseHandle(h);
		SetINI(pszFile);
	}
	return (h != INVALID_HANDLE_VALUE);
}

INLINE void CIni::SetINI(PCXStr pszFile, bool bNoReadOnly)
{
	if (pszFile != nullptr)
	{
		CXChar::Copy(m_szINI, LMT_MAX_PATH, pszFile);
		m_szINI[LMT_MAX_PATH - 1] = 0;

		if (bNoReadOnly)
		{
			ULong ulRet = ::GetFileAttributes(m_szINI);
			if ((ulRet != INVALID_FILE_ATTRIBUTES) && (ulRet & FILE_ATTRIBUTE_READONLY))
			{
				::SetFileAttributes(m_szINI, (ulRet & (~FILE_ATTRIBUTE_READONLY)));
			}
		}
	}
	else
	{
		MM_SAFE::Memset(m_szINI, 0, sizeof(XChar) * LMT_MAX_PATH);
	}
}

INLINE bool CIni::IsExist(void)
{
	ULong ulRet = ::GetFileAttributes(m_szINI);
	return (ulRet != INVALID_FILE_ATTRIBUTES);
}

INLINE bool CIni::IsSection(PCXStr pszSection)
{
	XChar szTmp[LMT_KEY] = { 0 };
	return (::GetPrivateProfileString(pszSection, nullptr, nullptr, szTmp, LMT_KEY, m_szINI) > 0);
}

INLINE bool CIni::CreateSetion(PCXStr pszSection)
{
	return (::WritePrivateProfileSection(pszSection, nullptr, m_szINI) ? true : false);
}

INLINE bool CIni::GetKeyValue(PCXStr pszSection, PCXStr pszKey, CString& str)
{
	XChar szBuf[LMT_BUF] = { 0 };
	if (GetKeyValue(pszSection, pszKey, szBuf, LMT_BUF))
	{
		str = szBuf;
		return true;
	}
	str.Empty();
	return false;
}

template <size_t stLenT>
INLINE bool CIni::GetKeyValue(PCXStr pszSection, PCXStr pszKey, CTStringFix<CXChar, stLenT>& strFix)
{
	bool bRet = GetKeyValue(pszSection, pszKey, *strFix, stLenT);
	strFix.ResetLength();
	return bRet;
}

INLINE bool CIni::GetKeyValue(PCXStr pszSection, PCXStr pszKey, PXStr pszValue, Int nLength)
{
	assert(nLength > 0);
	return (::GetPrivateProfileString(pszSection, pszKey, nullptr, pszValue, nLength, m_szINI) > 0);
}

INLINE bool CIni::SetKeyValue(PCXStr pszSection, PCXStr pszKey, PCXStr pszValue)
{
	return (::WritePrivateProfileString(pszSection, pszKey, pszValue, m_szINI) ? true : false);
}

INLINE UInt CIni::GetKeyIntValue(PCXStr pszSection, PCXStr pszKey, Int nDefault)
{
	return ::GetPrivateProfileInt(pszSection, pszKey, nDefault, m_szINI);
}

INLINE bool CIni::SetKeyIntValue(PCXStr pszSection, PCXStr pszKey, Int nValue)
{
	XChar szTmp[LMT_KEY] = { 0 };
	CXChar::ToString(nValue, szTmp, LMT_KEY);
	return (::WritePrivateProfileString(pszSection, pszKey, szTmp, m_szINI) ? true : false);
}

INLINE bool CIni::GetKeyStructValue(PCXStr pszSection, PCXStr pszKey, void* pStruct, UInt uSizeStruct)
{
	return (::GetPrivateProfileStruct(pszSection, pszKey, pStruct, uSizeStruct, m_szINI) ? true : false);
}

INLINE bool CIni::SetKeyStructValue(PCXStr pszSection, PCXStr pszKey, void* pStruct, UInt uSizeStruct)
{
	return (::WritePrivateProfileStruct(pszSection, pszKey, pStruct, uSizeStruct, m_szINI) ? true : false);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __INI_INL__