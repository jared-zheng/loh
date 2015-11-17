///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : filepath.h                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : file & folder path                                                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_PATH_H__
#define __FILE_PATH_H__

#pragma once

#include "container.h"
#include "sync.h"
#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFilePath
class CFilePath : public MObject
{
public:
	typedef CTArray<CString, CString::CStringTraits>            ARY_STRING;
	typedef CTArray<CStringFix, CStringFix::CStringFixTraits>   ARY_FIXSTRING;

public:
	static CPCXStr ModulePath;
	static CPCXStr WildCard;
	static CPCXStr LinkFile;
	static CPCXStr LinkFolder;
public:
	CFilePath(bool bModulePath = true);
	~CFilePath(void);
	// LMT_MAX_PATH
	bool   Copy(PCXStr pszDst, PCXStr pszSrc, bool bReplaceExisting = false, bool bEvenIfReadOnly = false, PCXStr pszDstKey = nullptr, PCXStr pszSrcKey = nullptr);
	bool   Move(PCXStr pszDst, PCXStr pszSrc, bool bReplaceExisting = false, bool bEvenIfReadOnly = false, PCXStr pszDstKey = nullptr, PCXStr pszSrcKey = nullptr);
	// LMT_MAX_PATH
	bool   Delete(PCXStr pszFileName, PCXStr pszKey = nullptr); 
	// LMT_MAX_PATH
	Int    Find(ARY_STRING& strFiles, PCXStr pszFileName, PCXStr pszKey = nullptr); 
	Int    Find(ARY_FIXSTRING& strFixFiles, PCXStr pszFileName, PCXStr pszKey = nullptr); 

	// folder path must end with '\\', path < LMT_MAX_PATH
	bool   CreateFolder(PCXStr pszPath, PCXStr pszKey = nullptr);
	void   DeleteFolder(PCXStr pszPath, PCXStr pszKey = nullptr);

	// folder path end with '\\', auto add, path < LMT_MAX_PATH
	bool   SetPath(PCXStr pszKey, PCXStr pszPath); 
	// str, strFix & pszFile include file name; str[0], strFix[0] & pszFile[0] = 0, get path only
	bool   GetFullPath(PCXStr pszKey, CString& str);
	bool   GetFullPath(PCXStr pszKey, CStringFix& strFix);
	bool   GetFullPath(PCXStr pszKey, PXStr pszFile, size_t stLen);
private:
	CFilePath(const CFilePath&);
	CFilePath& operator=(const CFilePath&);

	bool   CheckFilePath(PCXStr pszPath, bool bReplaceExisting = false, bool bEvenIfReadOnly = false);

	Int    FindFiles(ARY_STRING& strFiles, PCXStr pszFile);
	Int    FindFiles(ARY_FIXSTRING& strFixFiles, PCXStr pszFile);

	bool   CreateFolders(PCXStr pszFolder);
	void   DeleteFolders(PCXStr pszFolder);

	bool   GetPath(PCXStr pszKey, PXStr pszFile);
	void   SetModulePath(PCXStr pszKey); 
private:
	typedef struct tagFILE_PATH
	{
	public:
		tagFILE_PATH(void)
		: stLen(0)
		{
		}

		~tagFILE_PATH(void)
		{
		}
	public:
		size_t   stLen;
#ifdef RUNTIME_DEBUG
		XChar    szKey[LMT_KEY];   // debug verify only
#endif  // RUNTIME_DEBUG
		XChar    szPath[LMT_MAX_PATH];
	}FILE_PATH, *PFILE_PATH;

	typedef CTMap<size_t, FILE_PATH>         MAP_FILEPATH;
	typedef CTMap<size_t, FILE_PATH>::PAIR   MAP_FILEPATH_PAIR;
private:
	MAP_FILEPATH   m_FilePath;
	CSyncLock      m_FPLock; 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetfilepath.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetfilepath.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetfilepath.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetfilepath.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetfilepath.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetfilepath.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __FILE_PATH_H__