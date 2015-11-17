///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : crypto.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : crypto                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#pragma once

#include "streambuf.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMD5
class CMD5 : public MObject
{
public:
	enum MD5_CONST
	{
		MD5C_SD11  = 7,
		MD5C_SD12  = 12,
		MD5C_SD13  = 17,
		MD5C_SD14  = 22,
		MD5C_SD21  = 5,
		MD5C_SD22  = 9,
		MD5C_SD23  = 14,
		MD5C_SD24  = 20,
		MD5C_SD31  = 4,
		MD5C_SD32  = 11,
		MD5C_SD33  = 16,
		MD5C_SD34  = 23,
		MD5C_SD41  = 6,
		MD5C_SD42  = 10,
		MD5C_SD43  = 15,
		MD5C_SD44  = 21,

		MD5C_COUNT = 2,
		MD5C_STATE = 4,
		MD5C_BITS  = 32,
		MD5C_DATA  = 64,
	};

	enum MD5_OUT
	{
		MD5O_NUM   = 16,
		MD5O_STR   = 36,
	};
public:
	CMD5(void);
	~CMD5(void);

	template <size_t stLenT> bool Crypto(CStream& StreamSrc, CTStringFix<CXChar, stLenT>& strFix, size_t stSize = 0, bool bAppend = false);
	bool Crypto(CStream& StreamSrc, CString& str, size_t stSize = 0, bool bAppend = false);
	bool Crypto(CStream& StreamSrc, CStream& StreamDst, size_t stSize = 0);

private:
	CMD5(const CMD5&);
	CMD5& operator=(const CMD5&);

	void Update(CStream& StreamSrc, size_t stSize);
	void Final(PByte pOut);

	void Init(void);
	void Transform(PByte pIn);
	void Encode(PByte pOut, PUInt pIn, UInt uSize);
	void Decode(PUInt pOut, PByte pIn, UInt uSize);

	UInt F(UInt x, UInt y, UInt z);
	UInt G(UInt x, UInt y, UInt z);
	UInt H(UInt x, UInt y, UInt z);
	UInt I(UInt x, UInt y, UInt z);

	UInt ROTATE_LEFT(UInt x, UInt n);

	void FF(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac);
	void GG(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac);
	void HH(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac);
	void II(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac);
private:
	typedef struct tagMD5_DATA
	{
		UInt   uCount[MD5C_COUNT];
		UInt   uState[MD5C_STATE];
		Byte   bBuffer[MD5C_DATA];
		Byte   bPadding[MD5C_DATA];
	}MD5_DATA, *PMD5_DATA;
private:
	MD5_DATA   m_MD5;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "crypto.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __CRYPTO_H__