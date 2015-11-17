///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : codec.inl                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : codec                                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CODEC_INL__
#define __CODEC_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBase64
SELECTANY CPCStr     CBase64::EncodeTable   = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; 

SELECTANY const Char CBase64::DecodeTable[] =
{
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 62, 0 , 0 , 0 , 63, // +,  '/'
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0 , 0 , // '0'-'9'
	0 , 0 , 0 , 0 , 0 , 0,  1 , 2 , 3 , 4 , 5 , 6 , 
	7 , 8 , 9 , 10, 11, 12, 13, 14, 15, 16, 17, 18, 
	19, 20, 21, 22, 23, 24, 25, 0 , 0 , 0 , 0 , 0 , // 'A'-'Z'
    0 , 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
	37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 
	49, 50, 51, // 'a'-'z'
};

INLINE CBase64::CBase64(void)
{
}

INLINE CBase64::~CBase64(void)
{
}

INLINE CBase64::CBase64(const CBase64&)
{
}

INLINE CBase64& CBase64::operator=(const CBase64&)
{
	return (*this);
}

template <size_t stLenT> 
INLINE bool CBase64::Encode(CTStringFix<CChar, stLenT>& strFixIn, CCString& strOut)
{
	if (strFixIn.Length() > 0)
	{
		return Encode(*strFixIn, strFixIn.Length(), strOut);
	}
	return false;
}

template <size_t stLenT> 
INLINE bool CBase64::Decode(CTStringFix<CChar, stLenT>& strFixIn, CCString& strOut)
{
	if (strFixIn.Length() > 0)
	{
		return Decode(*strFixIn, strFixIn.Length(), strOut);
	}
	return false;
}

INLINE bool CBase64::Encode(CCString& strIn, CCString& strOut)
{
	if (strIn.Length() > 0)
	{
		return Encode(*strIn, strIn.Length(), strOut);
	}
	return false;
}

INLINE bool CBase64::Decode(CCString& strIn, CCString& strOut)
{
	if (strIn.Length() > 0)
	{
		return Decode(*strIn, strIn.Length(), strOut);
	}
	return false;
}

bool CBase64::Encode(PCStr pszIn, size_t stInSize, CCString& strOut)
{
	size_t stLen = 8 + (stInSize * 4) / 3;
	strOut.SetBufferLength(stLen);
	stLen = 0;

	Char cTmp[4] = { 0 };
	for (size_t i = 0; i < (stInSize / 3); ++i)
	{
		cTmp[0] = *pszIn;
		++pszIn;
		cTmp[1] = *pszIn;
		++pszIn;
		cTmp[2] = *pszIn;
		++pszIn;

		strOut += EncodeTable[(cTmp[0] & 0xFC) >> 2];
		strOut += EncodeTable[((cTmp[0] & 0x03) << 4) | ((cTmp[1] & 0xF0) >> 4)];
		strOut += EncodeTable[((cTmp[1] & 0x0F) << 2) | ((cTmp[2] & 0xC0) >> 6)];
		strOut += EncodeTable[cTmp[2] & 0x3F];

		stLen += 4;
		if (stLen == BASE64_LINE_LEN)
		{
			stLen   = 0;
			strOut += "\r\n";
		}
	}
	stLen = stInSize % 3;
	if (stLen == 1)
	{
		cTmp[0] = *pszIn;
		++pszIn;

		strOut += EncodeTable[(cTmp[0] & 0xFC) >> 2]; 
		strOut += EncodeTable[(cTmp[0] & 0x03) << 4]; 
		strOut += "==";
	}
	else if (stLen == 2)
	{
		cTmp[0] = *pszIn;
		++pszIn;
		cTmp[1] = *pszIn;
		++pszIn;

		strOut += EncodeTable[( cTmp[0] & 0xFC) >> 2]; 
		strOut += EncodeTable[((cTmp[0] & 0x03) << 4) | ((cTmp[1] & 0xF0) >> 4)]; 
		strOut += EncodeTable[( cTmp[1] & 0x0F) << 2]; 
		strOut += '=';
	}
	return true;
}

bool CBase64::Decode(PCStr pszIn, size_t stInSize, CCString& strOut)
{
	size_t stLen = 1 + (stInSize * 3) / 4;
	strOut.SetBufferLength(stLen);
	stLen = stInSize;

	UInt uData  = 0;
	Char cTmp[4] = { 0 };
	while (stLen >= 4)
	{
		if ((*pszIn != '\r') && (*pszIn != '\n'))
		{
			cTmp[0] = pszIn[0];
			cTmp[1] = pszIn[1];
			cTmp[2] = pszIn[2]; // '=' ?
			cTmp[3] = pszIn[3]; // '=' ?

			stLen  -= 4;
			pszIn  += 4;

			uData   = DecodeTable[cTmp[0]] << 18;
			uData  += DecodeTable[cTmp[1]] << 12;
			strOut += (Char)((uData & 0x00FF0000) >> 16);
			if (cTmp[2] != '=')
			{
				uData  += DecodeTable[cTmp[2]] << 6;
				strOut += (Char)((uData & 0x0000FF00) >> 8);
				if (cTmp[3] != '=')
				{
					uData  += DecodeTable[cTmp[3]];
					strOut += (Char)(uData & 0x000000FF);
				}
			}
		}
		else // \r\n
		{
			++pszIn;
			--stLen;
		}
	}
	return true;
}

#endif // __CODEC_INL__