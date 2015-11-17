///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : ini.h                                                                                 //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : ini windows                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __INI_H__
#define __INI_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIni
class CIni : public MObject
{
public:
	CIni(PCXStr pszFile = nullptr);
	~CIni(void);

	bool CreateINI(PCXStr pszFile);
	void SetINI(PCXStr pszFile, bool bNoReadOnly = false);

	bool IsExist(void);
	bool IsSection(PCXStr pszSection);
	bool CreateSetion(PCXStr pszSection);

	bool GetKeyValue(PCXStr pszSection, PCXStr pszKey, CString& str);
	template <size_t stLenT> bool GetKeyValue(PCXStr pszSection, PCXStr pszKey, CTStringFix<CXChar, stLenT>& strFix);
	bool GetKeyValue(PCXStr pszSection, PCXStr pszKey, PXStr pszValue, Int nLength);
	bool SetKeyValue(PCXStr pszSection, PCXStr pszKey, PCXStr pszValue);
	UInt GetKeyIntValue(PCXStr pszSection, PCXStr pszKey, Int nDefault = 0);	
	bool SetKeyIntValue(PCXStr pszSection, PCXStr pszKey, Int nValue);	
	bool GetKeyStructValue(PCXStr pszSection, PCXStr pszKey, void* pStruct, UInt uSizeStruct);
	bool SetKeyStructValue(PCXStr pszSection, PCXStr pszKey, void* pStruct, UInt uSizeStruct);
private:
	CIni(const CIni&);
	CIni& operator=(const CIni&);
public:
	XChar  m_szINI[LMT_MAX_PATH]; 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "ini.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __INI_H__