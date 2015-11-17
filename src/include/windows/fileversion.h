///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : fileversion.h                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : fileversion windows                                                                        //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_VERSION_H__
#define __FILE_VERSION_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileVersion : #pragma comment(lib, "version.lib")
class CFileVersion : public MObject
{
public:
	enum FILE_VERSION
	{
		FILE_VERSION_BUF = 4096,
	};
public:
	CFileVersion(void);
	~CFileVersion(void);

	bool   GetFileVersion(PCXStr pszFileName);
	bool   GetFileVersion(ULong& ulMajor, ULong& ulMinor);
	bool   GetProductVersion(ULong& ulMajor, ULong& ulMinor);
	PCXStr QueryFileVersion(PCXStr pszQuery);

	PCXStr GetFileVersion(void);
	PCXStr GetProductVersion(void);
	PCXStr GetInternalName(void);
	PCXStr GetCompanyName(void);
	PCXStr GetLegalCopyright(void);
	PCXStr GetOriginalFilename(void);
	PCXStr GetProductName(void);
	PCXStr GetFileDescription(void);
private:
	CFileVersion(const CFileVersion&);
	CFileVersion& operator=(const CFileVersion&);
private:
	ULong               m_ulLangCharset;
	ULong               m_ulInfoSize;
	VS_FIXEDFILEINFO*   m_pFileInfo;
	Byte                m_bBuffer[FILE_VERSION_BUF];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "fileversion.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __FILE_VERSION_H__