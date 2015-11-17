///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : stream.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : serialize                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_INL__
#define __STREAM_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStream
INLINE CStream::CStream(Int nMode)
: m_nMode(nMode)
{
}

INLINE CStream::~CStream(void)
{
}

INLINE CStream::CStream(const CStream&)
{
}

INLINE CStream& CStream::operator=(const CStream&)
{
	return (*this);
}

INLINE ULLong CStream::Read(void*, ULLong)
{
	return 0;
}

INLINE ULLong CStream::ReadBits(void*, ULLong)
{
	return 0;
}

INLINE ULLong CStream::Write(const void*, ULLong)
{
	return 0;
}

INLINE ULLong CStream::WriteBits(const void*, ULLong)
{
	return 0;
}

INLINE ULLong CStream::Tell(void) const
{
	return 0;
}

INLINE ULLong CStream::Size(void) const
{
	return 0;
}

INLINE bool CStream::Flush(void)
{
	return false;
}

INLINE void CStream::Close(void)
{
	m_nMode = STREAMM_NONE;
}

INLINE bool CStream::Refer(CStream&)
{
	return false;
}

INLINE Int CStream::GetMode(void)
{
	return m_nMode;
}

INLINE void CStream::SetError(void) 
{
	m_nMode |= STREAMM_ERROR;
}

INLINE bool CStream::IsError(void)
{
	return ((m_nMode & STREAMM_ERROR) != 0);
}

INLINE bool CStream::IsByteSwap(void)
{
	return ((m_nMode & STREAMM_BYTESWAP) != 0);
}

INLINE bool CStream::IsEnd(void)
{
	if (IsError() == false)
	{
		return (Tell() >= Size());
	}
	return true;
}

INLINE bool CStream::IsRead(void)
{
	if (IsError() == false)
	{
		return ((m_nMode & STREAMM_READ) != 0);
	}
	return false;
}

INLINE bool CStream::IsWrite(void)
{
	if (IsError() == false)
	{
		return ((m_nMode & STREAMM_WRITE) != 0);
	}
	return false;
}

INLINE void CStream::SetByteSwap(bool bEnabled)
{
	if (bEnabled)
	{
		m_nMode |= STREAMM_BYTESWAP;
	}
	else
	{
		m_nMode &= (~STREAMM_BYTESWAP);
	}
}

// write operations
INLINE CStream& CStream::operator<<(Char c)
{
	if (IsWrite())
	{
		Write(&c, sizeof(Char));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(Short s)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			s = (Short)CPlatform::ByteSwap((UShort)s);
		}
		Write(&s, sizeof(Short));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(Int n)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			n = (Int)CPlatform::ByteSwap((UInt)n);
		}
		Write(&n, sizeof(Int));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(Long l)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			l = (Long)CPlatform::ByteSwap((ULong)l);
		}
		Write(&l, sizeof(Long));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(LLong ll)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			ll = (LLong)CPlatform::ByteSwap((ULLong)ll);
		}
		Write(&ll, sizeof(LLong));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(UChar uc)
{
	if (IsWrite())
	{
		Write(&uc, sizeof(UChar));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(UShort us)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			us = CPlatform::ByteSwap(us);
		}
		Write(&us, sizeof(UShort));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(UInt u)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			u = CPlatform::ByteSwap(u);
		}
		Write(&u, sizeof(UInt));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(ULong ul)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			ul = CPlatform::ByteSwap(ul);
		}
		Write(&ul, sizeof(ULong));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(ULLong ull)
{
	if (IsWrite())
	{
		if (IsByteSwap())
		{
			ull = CPlatform::ByteSwap(ull);
		}
		Write(&ull, sizeof(ULLong));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(Float f)
{
	if (IsWrite())
	{
		Write(&f, sizeof(Float));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(Double d)
{
	if (IsWrite())
	{
		Write(&d, sizeof(Double));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(bool b)
{
	if (IsWrite())
	{
		Write(&b, sizeof(bool));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(WChar w)
{
	if (IsWrite())
	{
		Write(&w, sizeof(WChar));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(void* p)
{
	if (IsWrite())
	{
		Write(&p, sizeof(void*));
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(CString& str)
{
	if (IsWrite())
	{
		UInt uLen = (UInt)str.Length();
		Write(&uLen, sizeof(UInt));
		if (uLen > 0)
		{
			Write(*str, uLen * sizeof(XChar));
		}
	}
	return (*this);
}

template <size_t stLenT>
INLINE CStream& CStream::operator<<(CTStringFix<CXChar, stLenT>& strFix)
{
	if (IsWrite())
	{
		UInt uLen = (UInt)strFix.Length();
		Write(&uLen, sizeof(UInt));
		if (uLen > 0)
		{
			Write(*strFix, uLen * sizeof(XChar));
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(CStringRef& strRef)
{
	if (IsWrite())
	{
		UInt uLen = (UInt)strRef.Length();
		Write(&uLen, sizeof(UInt));
		if (uLen > 0)
		{
			Write(*strRef, uLen * sizeof(XChar));
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator<<(PCXStr psz)
{
	if (IsWrite() && (psz != nullptr))
	{
		UInt uLen = (UInt)CXChar::Length(psz);
		Write(&uLen, sizeof(UInt));
		if (uLen > 0)
		{
			Write(psz, uLen * sizeof(XChar));
		}
	}
	return (*this);
}

// read operations
INLINE CStream& CStream::operator>>(Char& c)
{
	if (IsRead())
	{
		Read(&c, sizeof(Char));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(Short& s)
{
	if (IsRead())
	{
		Read(&s, sizeof(Short));
		if (IsByteSwap())
		{
			s = (Short)CPlatform::ByteSwap((UShort)s);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(Int& n)
{
	if (IsRead())
	{
		Read(&n, sizeof(Int));
		if (IsByteSwap())
		{
			n = (Int)CPlatform::ByteSwap((UInt)n);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(Long& l)
{
	if (IsRead())
	{
		Read(&l, sizeof(Long));
		if (IsByteSwap())
		{
			l = (Long)CPlatform::ByteSwap((ULong)l);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(LLong& ll)
{
	if (IsRead())
	{
		Read(&ll, sizeof(LLong));
		if (IsByteSwap())
		{
			ll = (LLong)CPlatform::ByteSwap((ULLong)ll);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(UChar& uc)
{
	if (IsRead())
	{
		Read(&uc, sizeof(UChar));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(UShort& us)
{
	if (IsRead())
	{
		Read(&us, sizeof(UShort));
		if (IsByteSwap())
		{
			us = CPlatform::ByteSwap(us);
		}
	}
	return (*this);
}


INLINE CStream& CStream::operator>>(UInt& u)
{
	if (IsRead())
	{
		Read(&u, sizeof(UInt));
		if (IsByteSwap())
		{
			u = CPlatform::ByteSwap(u);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(ULong& ul)
{
	if (IsRead())
	{
		Read(&ul, sizeof(ULong));
		if (IsByteSwap())
		{
			ul = CPlatform::ByteSwap(ul);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(ULLong& ull)
{
	if (IsRead())
	{
		Read(&ull, sizeof(ULLong));
		if (IsByteSwap())
		{
			ull = CPlatform::ByteSwap(ull);
		}
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(Float& f)
{
	if (IsRead())
	{
		Read(&f, sizeof(Float));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(Double& d)
{
	if (IsRead())
	{
		Read(&d, sizeof(Double));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(bool& b)
{
	if (IsRead())
	{
		Read(&b, sizeof(bool));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(WChar& w)
{
	if (IsRead())
	{
		Read(&w, sizeof(WChar));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(void*& p)
{
	if (IsRead())
	{
		Read(&p, sizeof(void*));
	}
	return (*this);
}

INLINE CStream& CStream::operator>>(CString& str)
{
	if (IsRead())
	{
		UInt uLen = 0;
		Read(&uLen, sizeof(UInt));
		if (uLen > 0)
		{
			str.SetBufferLength(uLen + 1);
			Read(*str, uLen * sizeof(XChar));
			str.ResetLength();
		}
	}
	return (*this);
}

template <size_t stLenT>
INLINE CStream& CStream::operator>>(CTStringFix<CXChar, stLenT>& strFix)
{
	if (IsRead())
	{
		UInt uLen = 0;
		Read(&uLen, sizeof(UInt));
		if ((uLen > 0) && ((size_t)uLen < stLenT))
		{
			Read(*strFix, uLen * sizeof(XChar));
			strFix.ResetLength(uLen);
		}
	}
	return (*this);
}

#endif // __STREAM_INL__