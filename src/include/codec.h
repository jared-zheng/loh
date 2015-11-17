///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : codec.h                                                                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : codec                                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CODEC_H__
#define __CODEC_H__

#pragma once

#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBase64
class CBase64 : public MObject
{
public:
	enum BASE64_LINE
	{
		BASE64_LINE_LEN = 76,
	};
	static CPCStr     EncodeTable;
	static const Char DecodeTable[];
public:  
	template <size_t stLenT> 
	static bool Encode(CTStringFix<CChar, stLenT>& strFixIn, CCString& strOut);
	template <size_t stLenT>
	static bool Decode(CTStringFix<CChar, stLenT>& strFixIn, CCString& strOut);

	static bool Encode(CCString& strIn, CCString& strOut);  
	static bool Decode(CCString& strIn, CCString& strOut); 
private:
	CBase64(void);  
	~CBase64(void);  
	CBase64(const CBase64&);
	CBase64& operator=(const CBase64&);

	static bool Encode(PCStr pszIn, size_t stInSize, CCString& strOut); 
	static bool Decode(PCStr pszIn, size_t stInSize, CCString& strOut); 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "codec.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __CODEC_H__