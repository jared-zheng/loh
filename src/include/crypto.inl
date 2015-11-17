///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : crypto.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : crypto                                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __CRYPTO_INL__
#define __CRYPTO_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMD5
CMD5::CMD5(void)
{
	Init();
}

CMD5::~CMD5(void)
{
}

CMD5::CMD5(const CMD5&)
{
}

CMD5& CMD5::operator=(const CMD5&)
{
	return (*this);
}

template <size_t stLenT>
bool CMD5::Crypto(CStream& StreamSrc, CTStringFix<CXChar, stLenT>& strFix, size_t stSize, bool bAppend)
{
	Update(StreamSrc, stSize);
	Byte bOut[MD5O_NUM] = { 0 };
	Final(bOut);

	if (bAppend)
	{
		strFix.AppendFormat(TF("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"),
		                    bOut[0],  bOut[1],  bOut[2],  bOut[3],  bOut[4],  bOut[5],  bOut[6],  bOut[7],
				            bOut[8],  bOut[9],  bOut[10], bOut[11], bOut[12], bOut[13], bOut[14], bOut[15]);
	}
	else
	{
		strFix.Format(TF("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"),
		              bOut[0],  bOut[1],  bOut[2],  bOut[3],  bOut[4],  bOut[5],  bOut[6],  bOut[7],
				      bOut[8],  bOut[9],  bOut[10], bOut[11], bOut[12], bOut[13], bOut[14], bOut[15]);
	}
	return true;
}

bool CMD5::Crypto(CStream& StreamSrc, CString& str, size_t stSize, bool bAppend)
{
	Update(StreamSrc, stSize);
	Byte bOut[MD5O_NUM] = { 0 };
	Final(bOut);

	if (bAppend)
	{
		str.AppendFormat(TF("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"),
		                 bOut[0],  bOut[1],  bOut[2],  bOut[3],  bOut[4],  bOut[5],  bOut[6],  bOut[7],
				         bOut[8],  bOut[9],  bOut[10], bOut[11], bOut[12], bOut[13], bOut[14], bOut[15]);
	}
	else
	{
		str.Format(TF("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"),
		           bOut[0],  bOut[1],  bOut[2],  bOut[3],  bOut[4],  bOut[5],  bOut[6],  bOut[7],
				   bOut[8],  bOut[9],  bOut[10], bOut[11], bOut[12], bOut[13], bOut[14], bOut[15]);
	}
	return true;
}

bool CMD5::Crypto(CStream& StreamSrc, CStream& StreamDst, size_t stSize)
{
	assert(StreamDst.IsWrite());
	assert(StreamDst.Size() >= MD5O_NUM);

	Update(StreamSrc, stSize);
	Byte bOut[MD5O_NUM] = { 0 };
	Final(bOut);

	return (StreamDst.Write(bOut, MD5O_NUM) > 0);
}

void CMD5::Update(CStream& StreamSrc, size_t stSize)
{
	assert(StreamSrc.IsRead());
	size_t stLen = (size_t)(StreamSrc.Size() - StreamSrc.Tell());
	if (stSize > 0)
	{
		stSize = DEF_BASE::Min<size_t>(stLen, stSize);
	}
	else
	{
		stSize = stLen;
	}

	// Compute number of bytes mod 64
	UInt uIndex = (UInt)((m_MD5.uCount[0] >> 3) & 0X3F);
	if ((m_MD5.uCount[0] += (UInt)(stSize << 3)) < (UInt)(stSize << 3))
	{
		++m_MD5.uCount[1];
	}
	m_MD5.uCount[1] += (UInt)(stSize >> 29);

	size_t i      = 0;
	size_t stPart = MD5C_DATA - uIndex;
	if (stSize >= stPart)
	{
		StreamSrc.Read((m_MD5.bBuffer + uIndex), stPart);
		Transform(m_MD5.bBuffer);

		Byte bBuffer[MD5C_DATA] = { 0 };
		for (i = stPart; (i + 63) < stSize ; i += MD5C_DATA)
		{
			StreamSrc.Read(bBuffer, MD5C_DATA);
			Transform(bBuffer);
		}
		uIndex = 0;
	}
	StreamSrc.Read((m_MD5.bBuffer + uIndex), (stSize - i));
}

void CMD5::Final(PByte pOut)
{
	Byte bits[8] = { 0 };
	Encode(bits, m_MD5.uCount, 8);
	// Pad out to 56 mod 64
	UInt uIndex  = (UInt)((m_MD5.uCount[0] >> 3) & 0X3F);
	UInt uLength = (uIndex < 56) ? (56 - uIndex) : (120 - uIndex);

	CBufReadStream BufReadStream(m_MD5.bPadding, uLength);
	Update(BufReadStream, 0);

	BufReadStream.Close();
	BufReadStream.Create(bits, 8);
	Update(BufReadStream, 0);

	Encode(pOut, m_MD5.uState, MD5O_NUM);
	Init();
}

void CMD5::Init(void)
{
	// http://www.ietf.org/rfc/rfc1321.txt
	// Zeroize sensitive information
	MM_SAFE::Memset(&m_MD5, 0, sizeof(MD5_DATA));

	//m_MD5.uCount[0] = 0;
	//m_MD5.uCount[1] = 0;

	m_MD5.uState[0] = 0X67452301;
	m_MD5.uState[1] = 0XEFCDAB89;
	m_MD5.uState[2] = 0X98BADCFE;
	m_MD5.uState[3] = 0X10325476;

	//MM_SAFE::Memset(m_MD5.bPadding, 0, MD5C_DATA);
	m_MD5.bPadding[0] = 0X80;
}

void CMD5::Transform(PByte pIn)
{
	UInt a = m_MD5.uState[0];
	UInt b = m_MD5.uState[1];
	UInt c = m_MD5.uState[2];
	UInt d = m_MD5.uState[3];
	UInt x[MD5O_NUM] = { 0 };
	Decode(x, pIn, MD5C_DATA);

	FF(a, b, c, d, x[ 0], (UInt)MD5C_SD11, (UInt)0XD76AA478); // 1 
	FF(d, a, b, c, x[ 1], (UInt)MD5C_SD12, (UInt)0XE8C7B756); // 2 
	FF(c, d, a, b, x[ 2], (UInt)MD5C_SD13, (UInt)0X242070DB); // 3 
	FF(b, c, d, a, x[ 3], (UInt)MD5C_SD14, (UInt)0XC1BDCEEE); // 4 
	FF(a, b, c, d, x[ 4], (UInt)MD5C_SD11, (UInt)0XF57C0FAF); // 5 
	FF(d, a, b, c, x[ 5], (UInt)MD5C_SD12, (UInt)0X4787C62A); // 6 
	FF(c, d, a, b, x[ 6], (UInt)MD5C_SD13, (UInt)0XA8304613); // 7 
	FF(b, c, d, a, x[ 7], (UInt)MD5C_SD14, (UInt)0XFD469501); // 8 
	FF(a, b, c, d, x[ 8], (UInt)MD5C_SD11, (UInt)0X698098D8); // 9 
	FF(d, a, b, c, x[ 9], (UInt)MD5C_SD12, (UInt)0X8B44F7AF); // 10
	FF(c, d, a, b, x[10], (UInt)MD5C_SD13, (UInt)0XFFFF5BB1); // 11
	FF(b, c, d, a, x[11], (UInt)MD5C_SD14, (UInt)0X895CD7BE); // 12
	FF(a, b, c, d, x[12], (UInt)MD5C_SD11, (UInt)0X6B901122); // 13
	FF(d, a, b, c, x[13], (UInt)MD5C_SD12, (UInt)0XFD987193); // 14
	FF(c, d, a, b, x[14], (UInt)MD5C_SD13, (UInt)0XA679438E); // 15
	FF(b, c, d, a, x[15], (UInt)MD5C_SD14, (UInt)0X49B40821); // 16

	GG(a, b, c, d, x[ 1], (UInt)MD5C_SD21, (UInt)0XF61E2562); // 17 
	GG(d, a, b, c, x[ 6], (UInt)MD5C_SD22, (UInt)0XC040B340); // 18 
	GG(c, d, a, b, x[11], (UInt)MD5C_SD23, (UInt)0X265E5A51); // 19 
	GG(b, c, d, a, x[ 0], (UInt)MD5C_SD24, (UInt)0XE9B6C7AA); // 20 
	GG(a, b, c, d, x[ 5], (UInt)MD5C_SD21, (UInt)0XD62F105D); // 21 
	GG(d, a, b, c, x[10], (UInt)MD5C_SD22, (UInt)0X02441453); // 22 
	GG(c, d, a, b, x[15], (UInt)MD5C_SD23, (UInt)0XD8A1E681); // 23 
	GG(b, c, d, a, x[ 4], (UInt)MD5C_SD24, (UInt)0XE7D3FBC8); // 24 
	GG(a, b, c, d, x[ 9], (UInt)MD5C_SD21, (UInt)0X21E1CDE6); // 25 
	GG(d, a, b, c, x[14], (UInt)MD5C_SD22, (UInt)0XC33707D6); // 26 
	GG(c, d, a, b, x[ 3], (UInt)MD5C_SD23, (UInt)0XF4D50D87); // 27 
	GG(b, c, d, a, x[ 8], (UInt)MD5C_SD24, (UInt)0X455A14ED); // 28 
	GG(a, b, c, d, x[13], (UInt)MD5C_SD21, (UInt)0XA9E3E905); // 29 
	GG(d, a, b, c, x[ 2], (UInt)MD5C_SD22, (UInt)0XFCEFA3F8); // 30 
	GG(c, d, a, b, x[ 7], (UInt)MD5C_SD23, (UInt)0X676F02D9); // 31 
	GG(b, c, d, a, x[12], (UInt)MD5C_SD24, (UInt)0X8D2A4C8A); // 32 

	HH(a, b, c, d, x[ 5], (UInt)MD5C_SD31, (UInt)0XFFFA3942); // 33 
	HH(d, a, b, c, x[ 8], (UInt)MD5C_SD32, (UInt)0X8771F681); // 34 
	HH(c, d, a, b, x[11], (UInt)MD5C_SD33, (UInt)0X6D9D6122); // 35 
	HH(b, c, d, a, x[14], (UInt)MD5C_SD34, (UInt)0XFDE5380C); // 36 
	HH(a, b, c, d, x[ 1], (UInt)MD5C_SD31, (UInt)0XA4BEEA44); // 37 
	HH(d, a, b, c, x[ 4], (UInt)MD5C_SD32, (UInt)0X4BDECFA9); // 38 
	HH(c, d, a, b, x[ 7], (UInt)MD5C_SD33, (UInt)0XF6BB4B60); // 39 
	HH(b, c, d, a, x[10], (UInt)MD5C_SD34, (UInt)0XBEBFBC70); // 40 
	HH(a, b, c, d, x[13], (UInt)MD5C_SD31, (UInt)0X289B7EC6); // 41 
	HH(d, a, b, c, x[ 0], (UInt)MD5C_SD32, (UInt)0XEAA127FA); // 42 
	HH(c, d, a, b, x[ 3], (UInt)MD5C_SD33, (UInt)0XD4EF3085); // 43 
	HH(b, c, d, a, x[ 6], (UInt)MD5C_SD34, (UInt)0X04881D05); // 44 
	HH(a, b, c, d, x[ 9], (UInt)MD5C_SD31, (UInt)0XD9D4D039); // 45 
	HH(d, a, b, c, x[12], (UInt)MD5C_SD32, (UInt)0XE6DB99E5); // 46 
	HH(c, d, a, b, x[15], (UInt)MD5C_SD33, (UInt)0X1FA27CF8); // 47 
	HH(b, c, d, a, x[ 2], (UInt)MD5C_SD34, (UInt)0XC4AC5665); // 48 
												 
	II(a, b, c, d, x[ 0], (UInt)MD5C_SD41, (UInt)0XF4292244); // 49 
	II(d, a, b, c, x[ 7], (UInt)MD5C_SD42, (UInt)0X432AFF97); // 50 
	II(c, d, a, b, x[14], (UInt)MD5C_SD43, (UInt)0XAB9423A7); // 51 
	II(b, c, d, a, x[ 5], (UInt)MD5C_SD44, (UInt)0XFC93A039); // 52 
	II(a, b, c, d, x[12], (UInt)MD5C_SD41, (UInt)0X655B59C3); // 53 
	II(d, a, b, c, x[ 3], (UInt)MD5C_SD42, (UInt)0X8F0CCC92); // 54 
	II(c, d, a, b, x[10], (UInt)MD5C_SD43, (UInt)0XFFEFF47D); // 55 
	II(b, c, d, a, x[ 1], (UInt)MD5C_SD44, (UInt)0X85845DD1); // 56 
	II(a, b, c, d, x[ 8], (UInt)MD5C_SD41, (UInt)0X6FA87E4F); // 57 
	II(d, a, b, c, x[15], (UInt)MD5C_SD42, (UInt)0XFE2CE6E0); // 58 
	II(c, d, a, b, x[ 6], (UInt)MD5C_SD43, (UInt)0XA3014314); // 59 
	II(b, c, d, a, x[13], (UInt)MD5C_SD44, (UInt)0X4E0811A1); // 60 
	II(a, b, c, d, x[ 4], (UInt)MD5C_SD41, (UInt)0XF7537E82); // 61 
	II(d, a, b, c, x[11], (UInt)MD5C_SD42, (UInt)0XBD3AF235); // 62 
	II(c, d, a, b, x[ 2], (UInt)MD5C_SD43, (UInt)0X2AD7D2BB); // 63 
	II(b, c, d, a, x[ 9], (UInt)MD5C_SD44, (UInt)0XEB86D391); // 64 

	m_MD5.uState[0] += a;
	m_MD5.uState[1] += b;
	m_MD5.uState[2] += c;
	m_MD5.uState[3] += d;
}

void CMD5::Encode(PByte pOut, PUInt pIn, UInt uSize)
{
	UInt i = 0;
	UInt j = 0;
	for (; j < uSize; ++i, j += 4)
	{
		pOut[j + 0] = (Byte)(pIn[i] & 0XFF);
		pOut[j + 1] = (Byte)((pIn[i] >> 8) & 0XFF);
		pOut[j + 2] = (Byte)((pIn[i] >> 16) & 0XFF);
		pOut[j + 3] = (Byte)((pIn[i] >> 24) & 0XFF);
	}
}

void CMD5::Decode(PUInt pOut, PByte pIn, UInt uSize)
{
	UInt i = 0;
	UInt j = 0;
	for (; j < uSize; ++i, j += 4)
	{
		pOut[i] = ((UInt)pIn[j + 0])       | 
			      ((UInt)pIn[j + 1] << 8)  | 
				  ((UInt)pIn[j + 2] << 16) | 
				  ((UInt)pIn[j + 3] << 24);
	}
}

INLINE UInt CMD5::F(UInt x, UInt y, UInt z)
{
	return ((x & y) | ((~x) & z));
}

INLINE UInt CMD5::G(UInt x, UInt y, UInt z)
{
	return ((x & z) | (y & (~z)));
}

INLINE UInt CMD5::H(UInt x, UInt y, UInt z)
{
	return (x ^ y ^ z);
}

INLINE UInt CMD5::I(UInt x, UInt y, UInt z)
{
	return (y ^(x | (~z)));
}

INLINE UInt CMD5::ROTATE_LEFT(UInt x, UInt n)
{
	return ((x << n) | (x >> (MD5C_BITS - n)));
}

INLINE void CMD5::FF(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac)
{
	a += (F(b, c, d) + x + ac);
	a  = ROTATE_LEFT(a, s);
	a += b;
}

INLINE void CMD5::GG(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac)
{
	a += (G(b, c, d) + x + ac);
	a  = ROTATE_LEFT(a, s);
	a += b;
}

INLINE void CMD5::HH(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac)
{
	a += (H(b, c, d) + x + ac);
	a  = ROTATE_LEFT(a, s);
	a += b;	
}

INLINE void CMD5::II(UInt& a, UInt b, UInt c, UInt d, UInt x, UInt s, UInt ac)
{
	a += (I(b, c, d) + x + ac);
	a  = ROTATE_LEFT(a, s);
	a += b;
}

#endif // __CRYPTO_INL__