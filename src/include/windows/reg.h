///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : reg.h                                                                                 //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : reg windows                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_H__
#define __REG_H__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CReg
class CReg : public MObject
{
public:
	CReg(HKEY hKey = nullptr);
	~CReg(void);

	operator HKEY(void) const;

	Long Create(HKEY   hKeyParent, 
				PCXStr pszKeyName,
				PXStr  pszClass = REG_NONE, 
				ULong  ulOptions = REG_OPTION_NON_VOLATILE, 
				REGSAM samDesired = KEY_READ | KEY_WRITE,
				PULong pulDisposition = nullptr);

	Long Open(HKEY  hKeyParent,
			  PCXStr pszKeyName,
			  REGSAM samDesired = KEY_READ | KEY_WRITE);

	Long Close(void);
	Long Flush(void); // Flush the key's data to disk
	Long DeleteSubKey(PCXStr pszSubKey);
	Long DeleteValue(PCXStr pszValue);
	Long EnumKey(ULong ulIndex, PXStr pszName, PULong pulNameLength, FILETIME* pftLastWriteTime = nullptr);
	Long NotifyChangeKeyValue(BOOL bWatchSubtree, ULong ulNotifyFilter, HANDLE hEvent, BOOL bAsync = TRUE);
	Long RecurseDeleteKey(PCXStr pszKey);
	Long GetKeySecurity(SECURITY_INFORMATION& si, PSECURITY_DESCRIPTOR psd, PULong pulBytes);
	Long SetKeySecurity(SECURITY_INFORMATION& si, PSECURITY_DESCRIPTOR psd);

	Long QueryValue(PCXStr pszValueName, PULong pdwType, void* pData, PULong pulBytes);
	Long QueryBinaryValue(PCXStr pszValueName, void* pValue, PULong pulBytes);
	Long QueryULongValue(PCXStr pszValueName, ULong& ulValue);
	Long QueryULLongValue(PCXStr pszValueName, ULLong &ullValue);
	Long QueryStringValue(PCXStr pszValueName, PXStr pszValue, PULong pulChars);
	Long QueryMultiStringValue(PCXStr pszValueName, PXStr pszValue, PULong pulChars);

	Long SetValue(PCXStr pszValueName, ULong ulType, const void* pValue, ULong ulBytes);
	Long SetBinaryValue(PCXStr pszValueName, const void* pValue, ULong ulBytes);
	Long SetULongValue(PCXStr pszValueName, ULong ulValue);
	Long SetULLongValue(PCXStr pszValueName, ULLong ullValue);
	Long SetStringValue(PCXStr pszValueName, PCXStr pszValue, ULong ulType = REG_SZ);
	Long SetMultiStringValue(PCXStr pszValueName, PCXStr pszValue);

	Long SetKeyValue(PCXStr pszKeyName, PCXStr pszValue, PCXStr pszValueName = nullptr);
	static Long WINAPI SetValue(HKEY hKeyParent, PCXStr pszKeyName, PCXStr pszValue, PCXStr pszValueName = nullptr);
private:
	CReg(const CReg&);
	CReg& operator=(const CReg&);
private:
	HKEY m_hKey;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "reg.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __REG_H__