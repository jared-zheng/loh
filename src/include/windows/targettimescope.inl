///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targettimescope.inl                                                                   //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : windows time scope                                                                         //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_TIME_SCOPE_INL__
#define __TARGET_TIME_SCOPE_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTime
INLINE CTime::CTime(ULLong ullFileTime, Int nDST)
: m_llTime(0)
{
	ULARGE_INTEGER uiTime;
	uiTime.QuadPart = ullFileTime;  
	FILETIME ft;
	ft.dwHighDateTime = uiTime.HighPart;
	ft.dwLowDateTime  = uiTime.LowPart;

	FILETIME lft;
	if (::FileTimeToLocalFileTime(&ft, &lft) != FALSE)
	{
		CPlatform::TIMEINFO ti;
		if (::FileTimeToSystemTime(&lft, (PSYSTEMTIME)&ti) != FALSE)
		{
			CTime timeT(ti, nDST);
			m_llTime = timeT.m_llTime;
		}
	}
}

INLINE struct tm* CTime::GetTm(struct tm& TmRef, bool bLocal) const
{
	errno_t err = 0;
	struct tm atm;
	if (bLocal)
	{
		err = _localtime64_s(&atm, &m_llTime);
	}
	else
	{
		err = _gmtime64_s(&atm, &m_llTime);
	}
	if (err != 0)
	{
		return nullptr;    // indicates that m_time was not initialized!
	}
	TmRef = atm;
	return (&TmRef);
}

INLINE CString CTime::Format(PCXStr pszFormat) const
{
	CString strFormat;
	if ((pszFormat != nullptr) && (*pszFormat != 0))
	{
		struct tm atm;
		if (GetTm(atm) != nullptr)
		{
			XChar szBuf[LMT_BUF];
			if (_tcsftime(szBuf, LMT_BUF, pszFormat, &atm) > 0)
			{
				strFormat = szBuf;
			}
		}
	}
	return strFormat;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_TIME_SCOPE_INL__