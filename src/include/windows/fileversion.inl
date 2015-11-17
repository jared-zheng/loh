///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : fileversion.inl                                                                       //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : fileversion windows                                                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_VERSION_INL__
#define __FILE_VERSION_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileVersion
INLINE CFileVersion::CFileVersion(void)
: m_ulLangCharset(0)
, m_ulInfoSize(0)
, m_pFileInfo(nullptr)
{
}

INLINE CFileVersion::~CFileVersion(void)
{
}

INLINE CFileVersion::CFileVersion(const CFileVersion&)
: m_ulLangCharset(0)
, m_ulInfoSize(0)
, m_pFileInfo(nullptr)
{
}

INLINE CFileVersion& CFileVersion::operator=(const CFileVersion&)
{
	return (*this);
}

INLINE bool CFileVersion::GetFileVersion(PCXStr pszFileName)
{
	m_pFileInfo = nullptr;
	MM_SAFE::Memset(m_bBuffer, 0, FILE_VERSION_BUF);

	ULong ulHandle = 0;
	ULong ulSize   = ::GetFileVersionInfoSize(pszFileName, &ulHandle);
	if ((ulSize == 0) || (ulSize > FILE_VERSION_BUF))
	{
		DEV_DEBUG(TF("Get File Version failed, none info size or info size %d > buffer[%d]"), ulSize, FILE_VERSION_BUF);
		return false;
	}
	m_ulInfoSize = ulSize;
	if (::GetFileVersionInfo(pszFileName, ulHandle, FILE_VERSION_BUF, m_bBuffer) == FALSE)
	{
		DEV_DEBUG(TF("Get File Version failed by ERROR ID : %d"), ::GetLastError());
		return false;
	}
	UInt   uQuerySize = 0;
	PULong pulTable   = nullptr;
	if (::VerQueryValue(m_bBuffer, TF("\\VarFileInfo\\Translation"), (void**)&pulTable, &uQuerySize) == FALSE)
	{
		DEV_DEBUG(TF("Query File Version [\\VarFileInfo\\Translation] failed by ERROR ID : %d"), ::GetLastError());
		return false;
	}
	m_ulLangCharset = (ULong)MAKELONG(HIWORD(pulTable[0]), LOWORD(pulTable[0]));

	if (::VerQueryValue(m_bBuffer, TF("\\"), (void**)&(m_pFileInfo), &uQuerySize) == FALSE)
	{
		DEV_DEBUG(TF("Query File Version [\\] failed by ERROR ID : %d"), ::GetLastError());
		return false;
	}
	if ((uQuerySize != sizeof(VS_FIXEDFILEINFO)) || (m_pFileInfo == nullptr))
	{
		DEV_DEBUG(TF("Get File Version failed by ERROR ID : %d"), ::GetLastError());
		return false;
	}
	return true;
}

INLINE bool CFileVersion::GetFileVersion(ULong& ulMajor, ULong& ulMinor)
{
	if (m_pFileInfo != nullptr)
	{
		ulMajor = m_pFileInfo->dwFileVersionMS;
		ulMinor = m_pFileInfo->dwFileVersionLS;
		return true;
	}
	return false;
}

INLINE bool CFileVersion::GetProductVersion(ULong& ulMajor, ULong& ulMinor)
{
	if (m_pFileInfo != nullptr)
	{
		ulMajor = m_pFileInfo->dwProductVersionMS;
		ulMinor = m_pFileInfo->dwProductVersionLS;
		return true;
	}
	return false;
}

INLINE PCXStr CFileVersion::QueryFileVersion(PCXStr pszQuery)
{
	if (m_pFileInfo != nullptr)
	{
		UInt   uQuerySize  = 0;
		PXStr  pszQueryRet = nullptr;

		XChar  szBuf[LMT_MAX_PATH] = { 0 };
		CXChar::Format(szBuf, LMT_MAX_PATH, TF("\\StringFileInfo\\%08lx\\%s"), m_ulLangCharset, pszQuery);
		::VerQueryValue(m_bBuffer, szBuf, (void**)&(pszQueryRet), &uQuerySize);
		return pszQueryRet;
	}
	return nullptr;
}

INLINE PCXStr CFileVersion::GetFileVersion(void) 
{ 
	return QueryFileVersion(TF("FileVersion"));
}

INLINE PCXStr CFileVersion::GetProductVersion(void) 
{
	return QueryFileVersion(TF("ProductVersion"));
}

INLINE PCXStr CFileVersion::GetInternalName(void) 
{ 
	return QueryFileVersion(TF("InternalName")); 
}

INLINE PCXStr CFileVersion::GetCompanyName(void) 
{ 
	return QueryFileVersion(TF("CompanyName")); 
} 

INLINE PCXStr CFileVersion::GetLegalCopyright(void) 
{ 
	return QueryFileVersion(TF("LegalCopyright")); 
}

INLINE PCXStr CFileVersion::GetOriginalFilename(void) 
{ 
	return QueryFileVersion(TF("OriginalFilename"));
}

INLINE PCXStr CFileVersion::GetProductName(void) 
{ 
	return QueryFileVersion(TF("ProductName")); 
}

INLINE PCXStr CFileVersion::GetFileDescription(void) 
{ 
	return QueryFileVersion(TF("FileDescription"));
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __FILE_VERSION_INL__