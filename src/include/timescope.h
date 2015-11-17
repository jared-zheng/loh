///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : timescope.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : time scope                                                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TIME_SCOPE_H__
#define __TIME_SCOPE_H__

#pragma once

#include "stream.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTimeScope
class CTimeScope : public MObject
{
public:
	enum TIME_SCOPE_CONST
	{
		TIME_SCOPE_DAY_HOUR      = 24,
		TIME_SCOPE_HOUR_MINUTE   = 60,
		TIME_SCOPE_MINUTE_SECOND = 60,
	};
public:
	CTimeScope(void);
	CTimeScope(LLong llTime);
	CTimeScope(Int nDays, Int nHours, Int nMinutes, Int nSeconds);
	~CTimeScope(void);

	CTimeScope(const CTimeScope& aSrc);
	CTimeScope& operator=(const CTimeScope& aSrc);

	CTimeScope& operator+=(const CTimeScope& aSrc);
	CTimeScope& operator-=(const CTimeScope& aSrc);
	CTimeScope  operator+(const CTimeScope& aSrc) const;
	CTimeScope  operator-(const CTimeScope& aSrc) const;

	bool        operator<=(const CTimeScope& aSrc) const;
	bool        operator<(const CTimeScope& aSrc) const;
	bool        operator>=(const CTimeScope& aSrc) const;
	bool        operator>(const CTimeScope& aSrc) const;
	bool        operator==(const CTimeScope& aSrc) const;
	bool        operator!=(const CTimeScope& aSrc) const;

	LLong       GetScope(void) const;
	LLong       GetDays(void) const;

	LLong       GetTotalHours(void) const;
	LLong       GetTotalMinutes(void) const;
	LLong       GetTotalSeconds(void) const;

	Int         GetHours(void) const;
	Int         GetMinutes(void) const;
	Int         GetSeconds(void) const;

	void        Serialize(CStream&);
	CString     Format(PCXStr pszFormat = nullptr) const;
private:
	LLong       m_llTimeScope;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTime
class CTime : public MObject
{
public:
	enum TIME_CONST
	{
		TIME_MONTH_BASED = 1,
		TIME_WEEK_BASED  = 1,
		TIME_YEAR_BASED  = 1900,
	};
public:
	CTime(void);
	CTime(LLong llTime);
	CTime(ULLong ullFileTime, Int nDST = -1);
	CTime(Int nYear, Int nMonth, Int nDay, Int nHour, Int nMinute, Int nSecond, Int nDST = -1);
	CTime(const CPlatform::TIMEINFO& ti, Int nDST = -1);
	~CTime(void);

	CTime(const CTime& aSrc);
	CTime& operator=(const CTime& aSrc);

	CTime&     operator+=(const CTimeScope& scope);
	CTime&     operator-=(const CTimeScope& scope);
	CTime      operator+(const CTimeScope& scope) const;
	CTime      operator-(const CTimeScope& scope) const;
	CTimeScope operator-(const CTime& aSrc) const;

	bool       operator<=(const CTime& aSrc) const;
	bool       operator<(const CTime& aSrc) const;
	bool       operator>=(const CTime& aSrc) const;
	bool       operator>(const CTime& aSrc) const;
	bool       operator==(const CTime& aSrc) const;
	bool       operator!=(const CTime& aSrc) const;

	struct tm* GetTm(struct tm& TmRef, bool bLocal = true) const;
	LLong      GetTime(void) const;

	Int        GetYear(void) const;
	Int        GetMonth(void) const;
	Int        GetDay(void) const;
	Int        GetHour(void) const;
	Int        GetMinute(void) const;
	Int        GetSecond(void) const;
	Int        GetDayOfWeek(void) const;

	void       Serialize(CStream&);
	CString    Format(PCXStr pszFormat) const;
private:
	LLong      m_llTime;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "timescope.inl"

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targettimescope.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targettimescope.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targettimescope.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targettimescope.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targettimescope.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targettimescope.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __TIME_SCOPE_H__