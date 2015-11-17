///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetuuid.inl                                                                        //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : universally unique identifier                                                              //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_UUID_INL__
#define __TARGET_UUID_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUUID

SELECTANY CPCXStr CUUID::FROMAT_DIGITS_BRACES  = TF("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}");
SELECTANY CPCXStr CUUID::FROMAT_DIGITS_HYPHENS = TF("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X");
SELECTANY CPCXStr CUUID::FROMAT_DIGITS_AVERAGE = TF("%08X-%04X%04X-%02X%02X%02X%02X-%02X%02X%02X%02X");
SELECTANY CPCXStr CUUID::FROMAT_DIGITS_ONLY    = TF("%08X%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X");

INLINE CUUID::CUUID(void)
{
	MM_SAFE::Memset(&m_UUID, 0, sizeof(UID));
}

INLINE CUUID::~CUUID(void)
{
}

INLINE CUUID::CUUID(const UID& uuid)
{
	m_UUID = uuid;
}

INLINE CUUID::CUUID(PCXStr pszUUID)
{
	ToUUID(pszUUID);
}

INLINE CUUID::CUUID(const CUUID& aSrc)
{
	m_UUID = aSrc.m_UUID;
}

INLINE CUUID& CUUID::operator=(const CUUID& aSrc)
{
	if (this != &aSrc)
	{
		m_UUID = aSrc.m_UUID;
	}
	return (*this);
}

INLINE bool CUUID::operator==(const CUUID& aSrc) const
{
	return (MM_SAFE::Memcmp(&m_UUID, &(aSrc.m_UUID), sizeof(UID)) == 0);
}

INLINE bool CUUID::operator!=(const CUUID& aSrc) const
{
	return (MM_SAFE::Memcmp(&m_UUID, &(aSrc.m_UUID), sizeof(UID)) != 0);
}

INLINE void CUUID::Serialize(CStream& stream)
{
	if (stream.IsRead())
	{
		stream.Read(&m_UUID, sizeof(UID));
	}
	else if (stream.IsWrite())
	{
		stream.Write(&m_UUID, sizeof(UID));
	}
}

INLINE void CUUID::GetUUID(UID& uuid) const
{
	uuid = m_UUID;
}

INLINE void CUUID::SetUUID(const UID& uuid)
{
	m_UUID = uuid;
}

INLINE void CUUID::ToString(CString& str, bool bAppend, UUID_FROMAT eFormat) const
{
	PCXStr pszFormat = nullptr;
	switch (eFormat) 
	{
	case UUID_FROMAT_DIGITS_ONLY:
		{
			pszFormat = FROMAT_DIGITS_ONLY;
		}
		break;
	case UUID_FROMAT_DIGITS_AVERAGE:
		{
			pszFormat = FROMAT_DIGITS_AVERAGE;
		}
		break;
	case UUID_FROMAT_DIGITS_HYPHENS:
		{
			pszFormat = FROMAT_DIGITS_HYPHENS;
		}
		break;
	case UUID_FROMAT_DIGITS_BRACES:
	default:
		{
			pszFormat = FROMAT_DIGITS_BRACES;
		}
	}
	if (bAppend)
	{
		str.AppendFormat(pszFormat,
				         m_UUID.Data1, m_UUID.Data2, m_UUID.Data3, 
				         m_UUID.Data4[0], m_UUID.Data4[1],
				         m_UUID.Data4[2], m_UUID.Data4[3], 
				         m_UUID.Data4[4], m_UUID.Data4[5], 
				         m_UUID.Data4[6], m_UUID.Data4[7]);
	}
	else
	{
		str.Format(pszFormat,
				   m_UUID.Data1, m_UUID.Data2, m_UUID.Data3, 
				   m_UUID.Data4[0], m_UUID.Data4[1],
				   m_UUID.Data4[2], m_UUID.Data4[3], 
				   m_UUID.Data4[4], m_UUID.Data4[5], 
				   m_UUID.Data4[6], m_UUID.Data4[7]);
	}
}

template <size_t stLenT>
INLINE void CUUID::ToString(CTStringFix<CXChar, stLenT>& strFix, bool bAppend, UUID_FROMAT eFormat) const
{
	PCXStr pszFormat = nullptr;
	switch (eFormat) 
	{
	case UUID_FROMAT_DIGITS_ONLY:
		{
			pszFormat = FROMAT_DIGITS_ONLY;
		}
		break;
	case UUID_FROMAT_DIGITS_AVERAGE:
		{
			pszFormat = FROMAT_DIGITS_AVERAGE;
		}
		break;
	case UUID_FROMAT_DIGITS_HYPHENS:
		{
			pszFormat = FROMAT_DIGITS_HYPHENS;
		}
		break;
	case UUID_FROMAT_DIGITS_BRACES:
	default:
		{
			pszFormat = FROMAT_DIGITS_BRACES;
		}
	}
	if (bAppend)
	{
		strFix.AppendFormat(pszFormat,
					        m_UUID.Data1, m_UUID.Data2, m_UUID.Data3, 
					        m_UUID.Data4[0], m_UUID.Data4[1],
					        m_UUID.Data4[2], m_UUID.Data4[3], 
					        m_UUID.Data4[4], m_UUID.Data4[5], 
					        m_UUID.Data4[6], m_UUID.Data4[7]);
	}
	else
	{
		strFix.Format(pszFormat,
					  m_UUID.Data1, m_UUID.Data2, m_UUID.Data3, 
					  m_UUID.Data4[0], m_UUID.Data4[1],
					  m_UUID.Data4[2], m_UUID.Data4[3], 
					  m_UUID.Data4[4], m_UUID.Data4[5], 
					  m_UUID.Data4[6], m_UUID.Data4[7]);
	}
}

INLINE void CUUID::ToString(PXStr pszBuf, size_t stLen, UUID_FROMAT eFormat) const
{
	assert(stLen >= (size_t)UUIDC_LEN_MAX);
	assert(pszBuf != nullptr);

	PCXStr pszFormat = nullptr;
	switch (eFormat) 
	{
	case UUID_FROMAT_DIGITS_ONLY:
		{
			pszFormat = FROMAT_DIGITS_ONLY;
		}
		break;
	case UUID_FROMAT_DIGITS_AVERAGE:
		{
			pszFormat = FROMAT_DIGITS_AVERAGE;
		}
		break;
	case UUID_FROMAT_DIGITS_HYPHENS:
		{
			pszFormat = FROMAT_DIGITS_HYPHENS;
		}
		break;
	case UUID_FROMAT_DIGITS_BRACES:
	default:
		{
			pszFormat = FROMAT_DIGITS_BRACES;
		}
	}
	CXChar::Format(pszBuf, stLen, pszFormat,
	               m_UUID.Data1, m_UUID.Data2, m_UUID.Data3, 
	               m_UUID.Data4[0], m_UUID.Data4[1],
	               m_UUID.Data4[2], m_UUID.Data4[3], 
	               m_UUID.Data4[4], m_UUID.Data4[5], 
	               m_UUID.Data4[6], m_UUID.Data4[7]);
}

INLINE bool CUUID::ToUUID(PCXStr pszBuf, UUID_FROMAT eFormat)
{
	assert(pszBuf != nullptr);
	switch (eFormat) {
	case UUID_FROMAT_DIGITS_BRACES:
		{
			if ((pszBuf     == nullptr) ||
				(pszBuf[0]  != TF('{')) ||
				(pszBuf[9]  != TF('-')) ||
				(pszBuf[14] != TF('-')) ||
				(pszBuf[19] != TF('-')) ||
				(pszBuf[24] != TF('-')) ||
				(pszBuf[37] != TF('}')) )
			{
				return false;
			}
			PXStr pszEnd    = nullptr;
			m_UUID.Data1    = CXChar::ToULong((pszBuf + 1), &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data2    = (UShort)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data3    = (UShort)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;

			ULLong ullData  = CXChar::ToULLong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data4[0] = (UChar)(ullData >> 8);
			m_UUID.Data4[1] = (UChar)(ullData);

			ullData         = CXChar::ToULLong(pszEnd, &pszEnd, RADIXT_HEX);
			m_UUID.Data4[2] = (UChar)(ullData >> 40);
			m_UUID.Data4[3] = (UChar)(ullData >> 32);
			m_UUID.Data4[4] = (UChar)(ullData >> 24);
			m_UUID.Data4[5] = (UChar)(ullData >> 16);
			m_UUID.Data4[6] = (UChar)(ullData >> 8);
			m_UUID.Data4[7] = (UChar)(ullData);
		}
		break;
	case UUID_FROMAT_DIGITS_HYPHENS:
		{
			if ((pszBuf     == nullptr) || 
				(pszBuf[8]  != TF('-')) ||
				(pszBuf[13] != TF('-')) ||
				(pszBuf[18] != TF('-')) ||
				(pszBuf[23] != TF('-')) )
			{
				return false;
			}
			PXStr pszEnd    = nullptr;
			m_UUID.Data1    = CXChar::ToULong(pszBuf, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data2    = (UShort)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data3    = (UShort)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;

			ULLong ullData  = CXChar::ToULLong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data4[0] = (UChar)(ullData >> 8);
			m_UUID.Data4[1] = (UChar)(ullData);

			ullData         = CXChar::ToULLong(pszEnd, &pszEnd, RADIXT_HEX);
			m_UUID.Data4[2] = (UChar)(ullData >> 40);
			m_UUID.Data4[3] = (UChar)(ullData >> 32);
			m_UUID.Data4[4] = (UChar)(ullData >> 24);
			m_UUID.Data4[5] = (UChar)(ullData >> 16);
			m_UUID.Data4[6] = (UChar)(ullData >> 8);
			m_UUID.Data4[7] = (UChar)(ullData);
		}
		break;
	case UUID_FROMAT_DIGITS_AVERAGE:
		{
			if ((pszBuf     == nullptr) || 
				(pszBuf[8]  != TF('-')) ||
				(pszBuf[17] != TF('-')) ||
				(pszBuf[26] != TF('-')) )
			{
				return false;
			}
			PXStr pszEnd    = nullptr;
			m_UUID.Data1    = CXChar::ToULong(pszBuf, &pszEnd, RADIXT_HEX); ++pszEnd;
			UInt   uData    = (UInt)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;

			m_UUID.Data2    = (UShort)(uData >> 16);
			m_UUID.Data3    = (UShort)(uData);

			uData           = (UInt)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data4[0] = (UChar)(uData >> 24);
			m_UUID.Data4[1] = (UChar)(uData >> 16);
			m_UUID.Data4[2] = (UChar)(uData >> 8);
			m_UUID.Data4[3] = (UChar)(uData);

			uData           = (UInt)CXChar::ToULong(pszEnd, &pszEnd, RADIXT_HEX); ++pszEnd;
			m_UUID.Data4[4] = (UChar)(uData >> 24);
			m_UUID.Data4[5] = (UChar)(uData >> 16);
			m_UUID.Data4[6] = (UChar)(uData >> 8);
			m_UUID.Data4[7] = (UChar)(uData);
		}
		break;
	case UUID_FROMAT_DIGITS_ONLY:
		{
			if (pszBuf == nullptr)
			{
				return false;
			}
			PXStr pszEnd    = nullptr;
			ULLong ullData  = CXChar::ToULLong(pszBuf, &pszEnd, RADIXT_HEX);
			m_UUID.Data1    = (ULong)(ullData >> 32);
			m_UUID.Data2    = (UShort)(ullData >> 16);
			m_UUID.Data3    = (UShort)(ullData);

			ullData         = CXChar::ToULLong(pszEnd, &pszEnd, RADIXT_HEX);
			m_UUID.Data4[0] = (UChar)(ullData >> 56);
			m_UUID.Data4[1] = (UChar)(ullData >> 48);
			m_UUID.Data4[2] = (UChar)(ullData >> 40);
			m_UUID.Data4[3] = (UChar)(ullData >> 32);
			m_UUID.Data4[4] = (UChar)(ullData >> 24);
			m_UUID.Data4[5] = (UChar)(ullData >> 16);
			m_UUID.Data4[6] = (UChar)(ullData >> 8);
			m_UUID.Data4[7] = (UChar)(ullData);
		}
		break;
	default:
		{
			return false;
		}
	}
	return true;
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_UUID_INL__