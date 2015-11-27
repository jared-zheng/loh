///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : timescope.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : time scope                                                                                 //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TIME_SCOPE_INL__
#define __TIME_SCOPE_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTimeScope
INLINE CTimeScope::CTimeScope(void)
: m_llTimeScope(0)
{
}

INLINE CTimeScope::~CTimeScope(void)
{
}

INLINE CTimeScope::CTimeScope(LLong llTime)
: m_llTimeScope(llTime)
{
}

INLINE CTimeScope::CTimeScope(Int nDays, Int nHours, Int nMinutes, Int nSeconds)
: m_llTimeScope(0)
{
	m_llTimeScope = nSeconds + TIME_SCOPE_MINUTE_SECOND * (nMinutes + TIME_SCOPE_HOUR_MINUTE * (nHours + TIME_SCOPE_DAY_HOUR * nDays));
}

INLINE CTimeScope::CTimeScope(const CTimeScope& aSrc)
: m_llTimeScope(aSrc.m_llTimeScope)
{
}

INLINE CTimeScope& CTimeScope::operator=(const CTimeScope& aSrc)
{
	if (this != &aSrc)
	{
		m_llTimeScope = aSrc.m_llTimeScope;
	}
	return (*this);
}

INLINE CTimeScope& CTimeScope::operator+=(const CTimeScope& aSrc)
{
	m_llTimeScope += aSrc.m_llTimeScope;
	return (*this);
}

INLINE CTimeScope& CTimeScope::operator-=(const CTimeScope& aSrc)
{
	m_llTimeScope -= aSrc.m_llTimeScope;
	return (*this);
}

INLINE CTimeScope CTimeScope::operator+(const CTimeScope& aSrc) const
{
	return (CTimeScope(m_llTimeScope + aSrc.m_llTimeScope));
}

INLINE CTimeScope CTimeScope::operator-(const CTimeScope& aSrc) const
{
	return (CTimeScope(m_llTimeScope - aSrc.m_llTimeScope));
}

INLINE bool CTimeScope::operator<=(const CTimeScope& aSrc) const
{
	return (m_llTimeScope <= aSrc.m_llTimeScope);
}

INLINE bool CTimeScope::operator<(const CTimeScope& aSrc) const
{
	return (m_llTimeScope < aSrc.m_llTimeScope);
}

INLINE bool CTimeScope::operator>=(const CTimeScope& aSrc) const
{
	return (m_llTimeScope >= aSrc.m_llTimeScope);
}

INLINE bool CTimeScope::operator>(const CTimeScope& aSrc) const
{
	return (m_llTimeScope > aSrc.m_llTimeScope);
}

INLINE bool CTimeScope::operator==(const CTimeScope& aSrc) const
{
	return (m_llTimeScope == aSrc.m_llTimeScope);
}

INLINE bool CTimeScope::operator!=(const CTimeScope& aSrc) const
{
	return (m_llTimeScope != aSrc.m_llTimeScope);
}

INLINE LLong CTimeScope::GetScope(void) const
{
	return m_llTimeScope;
}

INLINE LLong CTimeScope::GetDays(void) const
{
	return (m_llTimeScope / (TIME_SCOPE_DAY_HOUR * TIME_SCOPE_HOUR_MINUTE * TIME_SCOPE_MINUTE_SECOND));
}

INLINE LLong CTimeScope::GetTotalHours(void) const
{
	return (m_llTimeScope / (TIME_SCOPE_HOUR_MINUTE * TIME_SCOPE_MINUTE_SECOND));
}

INLINE LLong CTimeScope::GetTotalMinutes(void) const
{
	return (m_llTimeScope / TIME_SCOPE_MINUTE_SECOND);
}

INLINE LLong CTimeScope::GetTotalSeconds(void) const
{
	return m_llTimeScope;
}

INLINE Int CTimeScope::GetHours(void) const
{
	return (Int)(GetTotalHours() - GetDays() * TIME_SCOPE_DAY_HOUR);
}

INLINE Int CTimeScope::GetMinutes(void) const
{
	return (Int)(GetTotalMinutes() - GetTotalHours() * TIME_SCOPE_HOUR_MINUTE);
}

INLINE Int CTimeScope::GetSeconds(void) const
{
	return (Int)(GetTotalSeconds() - GetTotalMinutes() * TIME_SCOPE_MINUTE_SECOND);
}

INLINE void CTimeScope::Serialize(CStream& stream)
{
	if (stream.IsRead())
	{
		stream >> m_llTimeScope;
	}
	else if (stream.IsWrite())
	{
		stream << m_llTimeScope;
	}
}

INLINE CString CTimeScope::Format(PCXStr pszFormat) const
{
	CString strFormat;
	if ((pszFormat != nullptr) && (*pszFormat != 0))
	{
		//	the only valid formats:
		//	%D - # of days
		//	%H - hour in 24 hour format
		//	%M - minute (0-59)
		//	%S - seconds (0-59)
		for (XChar ch = 0; (ch = *pszFormat) != 0; ++pszFormat)
		{
			if (ch == TF('%'))
			{
				++pszFormat;
				ch = *pszFormat;
				switch (ch)
				{
				case TF('D'):
					{
						strFormat.AppendFormat(TF("%lld"), GetDays());
					}
					break;
				case TF('H'):
					{
						strFormat.AppendFormat(TF("%02d"), GetHours());
					}
					break;
				case TF('M'):
					{
						strFormat.AppendFormat(TF("%02d"), GetMinutes());
					}
					break;
				case TF('S'):
					{
						strFormat.AppendFormat(TF("%02d"), GetSeconds());
					}
					break;
				default:
					{
						strFormat.AppendBuffer(ch);
					}
				}
			}
			else
			{
				strFormat.AppendBuffer(ch);
			}
		}
	}
	else
	{
		strFormat.Format(TF("%lld-%02d-%02d-%02d"), GetDays(), GetHours(), GetMinutes(), GetSeconds());
	}
	return strFormat;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTime
INLINE CTime::CTime(void)
: m_llTime(0)
{
	CPlatform::TIMEINFO ti;
	CPlatform::GetTimeInfo(ti);
	CTime timeT((Int)ti.usYear, (Int)ti.usMonth, (Int)ti.usDay, (Int)ti.usHour, (Int)ti.usMinute, (Int)ti.usSecond);
	m_llTime = timeT.m_llTime;
}

INLINE CTime::~CTime(void)
{
}

INLINE CTime::CTime(LLong llTime)
: m_llTime(llTime)
{
}

INLINE CTime::CTime(Int nYear, Int nMonth, Int nDay, Int nHour, Int nMinute, Int nSecond, Int nDST)
: m_llTime(0)
{
	assert(nYear >= 1900);
	assert((nMonth >= 1) && (nMonth <= 12));
	assert((nDay >= 1) && (nDay <= 31));
	assert((nHour >= 0) && (nHour <= 23));
	assert((nMinute >= 0) && (nMinute <= 59));
	assert((nSecond >= 0) && (nSecond <= 59));

	struct tm atm;
	atm.tm_sec   = nSecond;
	atm.tm_min   = nMinute;
	atm.tm_hour  = nHour;
	atm.tm_mday  = nDay;
	atm.tm_mon   = nMonth - TIME_MONTH_BASED;   // tm_mon is 0 based
	atm.tm_year  = nYear - TIME_YEAR_BASED; // tm_year is 1900 based
	atm.tm_isdst = nDST;

	m_llTime = mktime(&atm);
	assert(m_llTime != -1);      // indicates an illegal input time
}

INLINE CTime::CTime(const CPlatform::TIMEINFO& ti, Int nDST)
: m_llTime(0)
{
	CTime timeT((Int)ti.usYear, (Int)ti.usMonth, (Int)ti.usDay, (Int)ti.usHour, (Int)ti.usMinute, (Int)ti.usSecond, nDST);
	m_llTime = timeT.m_llTime;
}

INLINE CTime::CTime(const CTime& aSrc)
: m_llTime(aSrc.m_llTime)
{
}

INLINE CTime& CTime::operator=(const CTime& aSrc)
{
	if (this != &aSrc)
	{
		m_llTime = aSrc.m_llTime;
	}
	return (*this);
}

INLINE CTime& CTime::operator+=(const CTimeScope& scope)
{
	m_llTime += scope.GetScope();
	return (*this);
}

INLINE CTime& CTime::operator-=(const CTimeScope& scope)
{
	m_llTime -= scope.GetScope();
	return (*this);
}

INLINE CTime CTime::operator+(const CTimeScope& scope) const
{
	return (CTime(m_llTime + scope.GetScope()));
}

INLINE CTime CTime::operator-(const CTimeScope& scope) const
{
	return (CTime(m_llTime - scope.GetScope()));
}

INLINE CTimeScope CTime::operator-(const CTime& aSrc) const
{
	return (CTimeScope(m_llTime - aSrc.m_llTime));
}

INLINE bool CTime::operator<=(const CTime& aSrc) const
{
	return (m_llTime <= aSrc.m_llTime);
}

INLINE bool CTime::operator<(const CTime& aSrc) const
{
	return (m_llTime < aSrc.m_llTime);
}

INLINE bool CTime::operator>=(const CTime& aSrc) const
{
	return (m_llTime >= aSrc.m_llTime);
}

INLINE bool CTime::operator>(const CTime& aSrc) const
{
	return (m_llTime > aSrc.m_llTime);
}

INLINE bool CTime::operator==(const CTime& aSrc) const
{
	return (m_llTime == aSrc.m_llTime);
}

INLINE bool CTime::operator!=(const CTime& aSrc) const
{
	return (m_llTime != aSrc.m_llTime);
}

INLINE LLong CTime::GetTime(void) const
{
	return m_llTime;
}

INLINE Int CTime::GetYear(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? (ptm->tm_year + TIME_YEAR_BASED) : 0);
}

INLINE Int CTime::GetMonth(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? (ptm->tm_mon + TIME_MONTH_BASED) : 0);
}

INLINE Int CTime::GetDay(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? ptm->tm_mday : -1);
}

INLINE Int CTime::GetHour(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? ptm->tm_hour : -1);
}

INLINE Int CTime::GetMinute(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? ptm->tm_min : -1);
}

INLINE Int CTime::GetSecond(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? ptm->tm_sec : -1);
}

INLINE Int CTime::GetDayOfWeek(void) const
{
	struct tm  atm;
	struct tm* ptm = GetTm(atm);
	return ((ptm != nullptr) ? (ptm->tm_wday + TIME_WEEK_BASED) : 0);
}

INLINE void CTime::Serialize(CStream& stream)
{
	if (stream.IsRead())
	{
		stream >> m_llTime;
	}
	else if (stream.IsWrite())
	{
		stream << m_llTime;
	}
}

#endif // __TIME_SCOPE_INL__