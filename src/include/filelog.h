///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : filelog.h                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : file log                                                                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __FILE_LOG_H__
#define __FILE_LOG_H__

#pragma once

#include "thread.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileLog
class CFileLog : public MObject
{
public:
	static CPCXStr LogFilePrefix;
	static CPCXStr LogFileName;
	static CPCXStr LogEnglish;
public:
	static void SetLocal(PCXStr pszLanguage = LogEnglish);
public:
	CFileLog(bool bCreate = true, Int nLogLevel = LOGL_ALL, PCXStr pszFile = nullptr, PCXStr pszPrefix = nullptr);
	~CFileLog(void);

	Int   GetLevel(void);
	void  SetLevel(Int nLogLevel = LOGL_ALL);

	void  EnableLineFlush(bool bEnabled = true);

	bool  IsCreated(void);
	void  Create(PCXStr pszFile, PCXStr pszPrefix);
	void  Close(void);

	void  Log(Int nLevel, PCXStr pszLog);
	void  LogV(Int nLevel, PCXStr pszFormat, ...);
	void  LogL(Int nLevel, PCXStr pszFormat, va_list vl);

private:
	CFileLog(const CFileLog&);
	CFileLog& operator=(const CFileLog&);
private:
	Int     m_nLogLevel;
	Int     m_nFlush;
	FILE*   m_pFile;
	XChar   m_szBuf[LMT_BUF];
};

#ifdef RUNTIME_DEBUG
	#define LOG_TRACE(__log__, __str__)             (__log__).Log(LOGL_TRACE, (__str__))
	#define LOG_DEBUG(__log__, __str__)             (__log__).Log(LOGL_DEBUG, (__str__))
	#define LOGV_TRACE(__log__, ...)                (__log__).LogV(LOGL_TRACE, __VA_ARGS__)
	#define LOGV_DEBUG(__log__, ...)                (__log__).LogV(LOGL_DEBUG, __VA_ARGS__)
	#define LOGL_TRACE(__log__, __fmt__, __vl__)    (__log__).LogL(LOGL_TRACE, (__fmt__), (__vl__))
	#define LOGL_DEBUG(__log__, __fmt__, __vl__)    (__log__).LogL(LOGL_DEBUG, (__fmt__), (__vl__))
#else   // RUNTIME_DEBUG
	#define LOG_TRACE(__log__, __str__)
	#define LOG_DEBUG(__log__, __str__)
	#define LOGV_TRACE(__log__, ...)
	#define LOGV_DEBUG(__log__, ...)
	#define LOGL_TRACE(__log__, __fmt__, __vl__)
	#define LOGL_DEBUG(__log__, __fmt__, __vl__)
#endif  // RUNTIME_DEBUG
#define LOG_INFO(__log__, __str__)                   (__log__).Log(LOGL_INFO,  (__str__))
#define LOG_DUMP(__log__, __str__)                   (__log__).Log(LOGL_DUMP,  (__str__))
#define LOG_WARN(__log__, __str__)                   (__log__).Log(LOGL_WARN,  (__str__))
#define LOG_ERROR(__log__, __str__)                  (__log__).Log(LOGL_ERROR, (__str__))
#define LOGV_INFO(__log__, ...)                      (__log__).LogV(LOGL_INFO,  __VA_ARGS__)
#define LOGV_DUMP(__log__, ...)                      (__log__).LogV(LOGL_DUMP,  __VA_ARGS__)
#define LOGV_WARN(__log__, ...)                      (__log__).LogV(LOGL_WARN,  __VA_ARGS__)
#define LOGV_ERROR(__log__, ...)                     (__log__).LogV(LOGL_ERROR, __VA_ARGS__)
#define LOGL_INFO(__log__, __fmt__, __vl__)          (__log__).LogL(LOGL_INFO,  (__fmt__), (__vl__))
#define LOGL_DUMP(__log__, __fmt__, __vl__)          (__log__).LogL(LOGL_DUMP,  (__fmt__), (__vl__))
#define LOGL_WARN(__log__, __fmt__, __vl__)          (__log__).LogL(LOGL_WARN,  (__fmt__), (__vl__))
#define LOGL_ERROR(__log__, __fmt__, __vl__)         (__log__).LogL(LOGL_ERROR, (__fmt__), (__vl__))

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetfilelog.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetfilelog.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetfilelog.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetfilelog.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetfilelog.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetfilelog.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __FILE_LOG_H__