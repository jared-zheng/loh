///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : reg.inl                                                                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : reg windows                                                                                //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __REG_INL__
#define __REG_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CReg
INLINE CReg::CReg(HKEY hKey)
: m_hKey(hKey)
{
}

INLINE CReg::~CReg(void)
{
	Close();
}

INLINE CReg::CReg(const CReg&)
{
}

INLINE CReg& CReg::operator=(const CReg&)
{
	return (*this);
}

INLINE CReg::operator HKEY(void) const
{
	return m_hKey;
}

INLINE Long CReg::Create(HKEY hKeyParent, PCXStr pszKeyName, PXStr pszClass, ULong ulOptions, REGSAM samDesired, PULong pulDisposition)
{
	assert(hKeyParent != nullptr);
	ULong ulRet  = 0;
	HKEY  hKey = nullptr;
	Long  lRet = ::RegCreateKeyEx(hKeyParent, pszKeyName, 0, pszClass, ulOptions, samDesired, nullptr, &hKey, &ulRet);

	if (pulDisposition != nullptr)
	{
		*pulDisposition = ulRet;
	}
	if (lRet == ERROR_SUCCESS)
	{
		lRet   = Close();
		assert(lRet == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRet;
}

INLINE Long CReg::Open(HKEY hKeyParent, PCXStr pszKeyName, REGSAM samDesired)
{
	assert(hKeyParent != nullptr);

	HKEY hKey = nullptr;
	Long lRet = ::RegOpenKeyEx(hKeyParent, pszKeyName, 0, samDesired, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		lRet = Close();
		assert(lRet == ERROR_SUCCESS);
		m_hKey = hKey;
	}
	return lRet;
}

INLINE Long CReg::Close(void)
{
	Long lRet = ERROR_SUCCESS;
	if (m_hKey != nullptr)
	{
		lRet = ::RegCloseKey(m_hKey);
		m_hKey = nullptr;
	}
	return lRet;
}

INLINE Long CReg::Flush(void)
{
	assert(m_hKey != nullptr);
	return ::RegFlushKey(m_hKey);
}

INLINE Long CReg::DeleteSubKey(PCXStr pszSubKey)
{
	assert(m_hKey != nullptr);
	return ::RegDeleteKey(m_hKey, pszSubKey);
}

INLINE Long CReg::DeleteValue(PCXStr pszValue)
{
	assert(m_hKey != nullptr);
	return ::RegDeleteValue(m_hKey, (PXStr)pszValue);
}

INLINE Long CReg::EnumKey(ULong ulIndex, PXStr pszName, PULong pulNameLength, FILETIME* pftLastWriteTime)
{
	assert(m_hKey != nullptr);

	FILETIME ftLastWriteTime = { 0 };
	if (pftLastWriteTime == nullptr)
	{
		pftLastWriteTime = &ftLastWriteTime;
	}
	return ::RegEnumKeyEx(m_hKey, ulIndex, pszName, pulNameLength, nullptr, nullptr, nullptr, pftLastWriteTime);
}

INLINE Long CReg::NotifyChangeKeyValue(BOOL bWatchSubtree, ULong ulNotifyFilter, HANDLE hEvent, BOOL bAsync)
{
	assert(m_hKey != nullptr);
	assert((hEvent != nullptr) || (bAsync == FALSE));
	return ::RegNotifyChangeKeyValue(m_hKey, bWatchSubtree, ulNotifyFilter, hEvent, bAsync);
}

INLINE Long CReg::RecurseDeleteKey(PCXStr pszKey)
{
	CReg key;
	Long lRet = key.Open(m_hKey, pszKey, KEY_READ | KEY_WRITE);
	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}

	FILETIME ftTime = { 0 };
	ULong ulSize = LMT_MAX_PATH;
	XChar szBuffer[LMT_MAX_PATH] = { 0 };

	while (::RegEnumKeyEx(key.m_hKey, 0, szBuffer, &ulSize, nullptr, nullptr, nullptr, &ftTime) == ERROR_SUCCESS)
	{
		lRet = key.RecurseDeleteKey(szBuffer);
		if (lRet != ERROR_SUCCESS)
		{
			return lRet;
		}
		ulSize = LMT_MAX_PATH;
	}
	return DeleteSubKey(pszKey);
}

INLINE Long CReg::GetKeySecurity(SECURITY_INFORMATION& si, PSECURITY_DESCRIPTOR psd, PULong pulBytes)
{
	assert(m_hKey != nullptr);
	assert(pulBytes != nullptr);
	return ::RegGetKeySecurity(m_hKey, si, psd, pulBytes);
}

INLINE Long CReg::SetKeySecurity(SECURITY_INFORMATION& si, PSECURITY_DESCRIPTOR psd)
{
	assert(m_hKey != nullptr);
	assert(psd != nullptr);
	return ::RegSetKeySecurity(m_hKey, si, psd);
}

INLINE Long CReg::QueryValue(PCXStr pszValueName, PULong pdwType, void* pData, PULong pulBytes)
{
	assert(m_hKey != nullptr);
	return(::RegQueryValueEx(m_hKey, pszValueName, nullptr, pdwType, (PByte)(pData), pulBytes));
}

INLINE Long CReg::QueryBinaryValue(PCXStr pszValueName, void* pValue, PULong pulBytes)
{
	assert(pulBytes != nullptr);
	assert(m_hKey != nullptr);

	ULong ulType = 0;
	Long  lRet = ::RegQueryValueEx(m_hKey, pszValueName, nullptr, &ulType, (PByte)(pValue), pulBytes);
	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}
	if (ulType != REG_BINARY)
	{
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;
}

INLINE Long CReg::QueryULongValue(PCXStr pszValueName, ULong& ulValue)
{
	assert(m_hKey != nullptr);

	ULong ulSize = sizeof(ULong);
	ULong ulType = 0;
	Long lRet = ::RegQueryValueEx(m_hKey, pszValueName, nullptr, &ulType, (PByte)(&ulValue), &ulSize);
	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}
	if (ulType != REG_DWORD)
	{
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;
}

INLINE Long CReg::QueryULLongValue(PCXStr pszValueName, ULLong& ullValue)
{
	assert(m_hKey != nullptr);

	ULong ulSize = sizeof(ULLong);
	ULong ulType = 0;
	Long lRet = ::RegQueryValueEx(m_hKey, pszValueName, nullptr, &ulType, (PByte)(&ullValue), &ulSize);
	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}
	if (ulType != REG_QWORD)
	{
		return ERROR_INVALID_DATA;
	}
	return ERROR_SUCCESS;
}

INLINE Long CReg::QueryStringValue(PCXStr pszValueName, PXStr pszValue, PULong pulChars)
{
	assert(m_hKey != nullptr);
	assert(pulChars != nullptr);

	ULong ulSize = (*pulChars) * sizeof(XChar);
	*pulChars    = 0;

	ULong ulType = 0;
	Long lRet = ::RegQueryValueEx(m_hKey, pszValueName, nullptr, &ulType, (PByte)(pszValue), &ulSize);

	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}
	if ((ulType != REG_SZ) && (ulType != REG_EXPAND_SZ))
	{
		return ERROR_INVALID_DATA;
	}
	if ((pszValue != nullptr) && ((ulSize % sizeof(XChar) != 0) || (pszValue[ulSize / sizeof(XChar) -1] != 0)))
	{
		return ERROR_INVALID_DATA;
	}
	*pulChars = ulSize / sizeof(XChar);
	return ERROR_SUCCESS;
}

INLINE Long CReg::QueryMultiStringValue(PCXStr pszValueName, PXStr pszValue, PULong pulChars)
{
	assert(m_hKey != nullptr);
	assert(pulChars != nullptr);

	if ((pszValue != nullptr) && (*pulChars < 2))
	{
		return ERROR_INSUFFICIENT_BUFFER;
	}

	ULong ulSize = (*pulChars) * sizeof(XChar);
	*pulChars = 0;

	ULong ulType = 0;
	Long lRet = ::RegQueryValueEx(m_hKey, pszValueName, nullptr, &ulType, (PByte)(pszValue), &ulSize);
	if (lRet != ERROR_SUCCESS)
	{
		return lRet;
	}
	if (ulType != REG_MULTI_SZ)
	{
		return ERROR_INVALID_DATA;
	}
	if ((pszValue != nullptr) && 
	    ((ulSize % sizeof(XChar) != 0) || 
		 (ulSize / sizeof(XChar) < 2)  || 
		 (pszValue[ulSize / sizeof(XChar) - 1] != 0) || 
		 (pszValue[ulSize / sizeof(XChar) - 2] != 0)))
	{
		return ERROR_INVALID_DATA;
	}
	*pulChars = ulSize / sizeof(XChar);
	return ERROR_SUCCESS;
}

INLINE Long CReg::SetValue(PCXStr pszValueName, ULong ulType, const void* pValue, ULong ulBytes)
{
	assert(m_hKey != nullptr);
	return ::RegSetValueEx(m_hKey, pszValueName, 0, ulType, (const PByte)(pValue), ulBytes);
}

INLINE Long CReg::SetBinaryValue(PCXStr pszValueName, const void* pData, ULong ulBytes)
{
	assert(m_hKey != nullptr);
	return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_BINARY, (const PByte)(pData), ulBytes);
}

INLINE Long CReg::SetULongValue(PCXStr pszValueName, ULong ulValue)
{
	assert(m_hKey != nullptr);
	return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_DWORD, (const PByte)(&ulValue), sizeof(ULong));
}

INLINE Long CReg::SetULLongValue(PCXStr pszValueName, ULLong ullValue)
{
	assert(m_hKey != nullptr);
	return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_QWORD, (const PByte)(&ullValue), sizeof(ULLong));
}

INLINE Long CReg::SetStringValue(PCXStr pszValueName, PCXStr pszValue, ULong ulType)
{
	assert(m_hKey != nullptr);
	assert(pszValue != nullptr);
	assert((ulType == REG_SZ) || (ulType == REG_EXPAND_SZ));

	ULong ulSize = (ULong)CXChar::Length(pszValue) + 1;
	ulSize *= sizeof(XChar);

	return ::RegSetValueEx(m_hKey, pszValueName, 0, ulType, (const PByte)(pszValue), ulSize);
}

INLINE Long CReg::SetMultiStringValue(PCXStr pszValueName, PCXStr pszValue)
{
	assert(m_hKey != nullptr);
	assert(pszValue != nullptr);

	PCXStr pszTemp = pszValue;
	ULong  ulAllSize = 0;
	ULong  ulSize = 0;

	// Find the total length (in bytes) of all of the strings, including the
	// terminating '\0' of each string, and the second '\0' that terminates
	// the list.
	do
	{
		ulSize = (ULong)CXChar::Length(pszTemp) + 1;
		pszTemp   += ulSize; 
		ulAllSize += ulSize * sizeof(XChar);
	} while (ulSize != 1);

	return ::RegSetValueEx(m_hKey, pszValueName, 0, REG_MULTI_SZ, (const PByte)(pszValue), ulAllSize);
}

INLINE Long CReg::SetKeyValue(PCXStr pszKeyName, PCXStr pszValue, PCXStr pszValueName)
{
	assert(pszValue != nullptr);

	CReg key;
	Long lRet = key.Create(m_hKey, pszKeyName);
	if (lRet == ERROR_SUCCESS)
	{
		lRet = key.SetStringValue(pszValueName, pszValue);
	}
	return lRet;
}

INLINE Long WINAPI CReg::SetValue(HKEY hKeyParent, PCXStr pszKeyName, PCXStr pszValue, PCXStr pszValueName)
{
	assert(pszValue != nullptr);
	CReg key;
	Long lRet = key.Create(hKeyParent, pszKeyName);
	if (lRet == ERROR_SUCCESS)
	{
		lRet = key.SetStringValue(pszValueName, pszValue);
	}
	return lRet;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __REG_INL__