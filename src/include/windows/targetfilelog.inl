///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetfilelog.inl                                                                     //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows file log                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_FILE_LOG_INL__
#define __TARGET_FILE_LOG_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileLog
SELECTANY CPCXStr CFileLog::LogFilePrefix = TF("LOG");
SELECTANY CPCXStr CFileLog::LogFileName   = TF("%s%04d-%02d-%02d(%04d).log");
SELECTANY CPCXStr CFileLog::LogEnglish    = TF("english");

INLINE void CFileLog::SetLocal(PCXStr pszLanguage)
{
	_tsetlocale(0, pszLanguage);
}

INLINE CFileLog::CFileLog(bool bCreate, Int nLogLevel, PCXStr pszFile, PCXStr pszPrefix)
: m_nLogLevel(nLogLevel & LOGL_ALL)
, m_nFlush(TRUE)
, m_pFile(nullptr)
{
	if (bCreate)
	{
		Create(pszFile, pszPrefix);
	}
}

INLINE CFileLog::~CFileLog(void) 
{
	Close();
}

INLINE CFileLog::CFileLog(const CFileLog&)
: m_nLogLevel(LOGL_DEVFILE)
, m_nFlush(TRUE)
, m_pFile(nullptr)
{ 
}

INLINE CFileLog& CFileLog::operator=(const CFileLog&)
{ 
	return (*this);
}

INLINE Int CFileLog::GetLevel(void)
{
	return m_nLogLevel;
}

INLINE void CFileLog::SetLevel(Int nLogLevel)
{
	m_nLogLevel = (nLogLevel & LOGL_ALL);
}

INLINE void CFileLog::EnableLineFlush(bool bEnabled)
{
	m_nFlush = bEnabled ? TRUE : FALSE;
}

INLINE bool CFileLog::IsCreated(void)
{
	return (m_pFile != nullptr);
}

INLINE void CFileLog::Create(PCXStr pszFile, PCXStr pszPrefix)
{
	Close();
	if (pszFile == nullptr)
	{
		if (pszPrefix == nullptr)
		{
			pszPrefix = LogFilePrefix;
		}

		CPlatform::TIMEINFO ti = { 0 };
		CPlatform::GetTimeInfo(ti);

		CStringFix strName;
		strName.Format(LogFileName, pszPrefix, ti.usYear, ti.usMonth, ti.usDay, CThread::CurrentThreadId());
#ifdef RUNTIME_UNICODE
		m_pFile = _tfsopen(*strName, TF("wt, ccs=UNICODE"), _SH_DENYWR);
#else  // RUNTIME_UNICODE
		m_pFile = _tfsopen(*strName, TF("wt"), _SH_DENYWR);
#endif // RUNTIME_UNICODE
	}
	else
	{
#ifdef RUNTIME_UNICODE
		m_pFile = _tfsopen(pszFile, TF("wt, ccs=UNICODE"), _SH_DENYWR);
#else  // RUNTIME_UNICODE
		m_pFile = _tfsopen(pszFile, TF("wt"), _SH_DENYWR);
#endif // RUNTIME_UNICODE
	}
}

INLINE void CFileLog::Close(void)
{
	if (m_pFile != nullptr)
	{
		fclose(m_pFile);
		m_pFile = nullptr;
	}
}

INLINE void CFileLog::Log(Int nLevel, PCXStr pszLog)
{
	if (IsCreated() && (nLevel & m_nLogLevel))
	{
		CPlatform::TIMEINFO ti = { 0 };
		CPlatform::GetTimeInfo(ti);

		bool bRet = false;
		switch (nLevel)
		{
#ifdef RUNTIME_DEBUG
		case LOGL_TRACE:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[TRACE][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
		case LOGL_DEBUG:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[DEBUG][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
#endif  // RUNTIME_DEBUG
		case LOGL_INFO:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[INFO] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
		case LOGL_DUMP:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[DUMP] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
		case LOGL_WARN:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[WARN] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
		case LOGL_ERROR:
			{
				_ftprintf_s(m_pFile, TF("%02d:%02d:%02d.%03d[ERROR][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszLog);
				bRet = true;
			}
			break;
		default:
			{
			}
		}
		if (bRet && (m_nFlush != FALSE))
		{
			fflush(m_pFile);
		}
	}
}

INLINE void CFileLog::LogV(Int nLevel, PCXStr pszFormat, ...)
{
	va_list vl;
	va_start(vl, pszFormat);
	LogL(nLevel, pszFormat, vl);
	va_end(vl);
}

INLINE void CFileLog::LogL(Int nLevel, PCXStr pszFormat, va_list vl)
{
	if (IsCreated() && (nLevel & m_nLogLevel))
	{
		CPlatform::TIMEINFO ti = { 0 };
		CPlatform::GetTimeInfo(ti);

		Int nLen = 0;
		switch (nLevel)
		{
#ifdef RUNTIME_DEBUG
		case LOGL_TRACE:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[TRACE][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
		case LOGL_DEBUG:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[DEBUG][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
#endif  // RUNTIME_DEBUG
		case LOGL_INFO:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[INFO] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
		case LOGL_DUMP:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[DUMP] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
		case LOGL_WARN:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[WARN] [%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
		case LOGL_ERROR:
			{
				nLen = CXChar::Format(m_szBuf, LMT_BUF, TF("%02d:%02d:%02d.%03d[ERROR][%04X]%s\n"), ti.usHour, ti.usMinute, ti.usSecond, ti.usMSecond, CThread::CurrentThreadId(), pszFormat);
			}
			break;
		default:
			{
			}
		}
		if (nLen > 0)
		{
			_vftprintf_s(m_pFile, m_szBuf, vl);
			if (m_nFlush != FALSE)
			{
				fflush(m_pFile);
			}
		}
	}
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_FILE_LOG_INL__