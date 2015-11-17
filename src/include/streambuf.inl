///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : streambuf.inl                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : buffer serialize                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_BUF_INL__
#define __STREAM_BUF_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufStreamBase
INLINE CBufStreamBase::CBufStreamBase(Int nMode)
: CStream(nMode|STREAMM_BUFFER)
{
}

INLINE CBufStreamBase::~CBufStreamBase(void)
{
}

INLINE CBufStreamBase::CBufStreamBase(const CBufStreamBase&)
{
}

INLINE CBufStreamBase& CBufStreamBase::operator=(const CBufStreamBase&)
{
	return (*this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufStreamFix
template <size_t stFix>
INLINE CTBufStreamFix<stFix>::CTBufStreamFix(Int nMode)
: CBufStreamBase(nMode|STREAMM_BUFFER)
, m_ullPos(0)
{
}

template <size_t stFix>
INLINE CTBufStreamFix<stFix>::~CTBufStreamFix(void)
{
}

template <size_t stFix>
INLINE CTBufStreamFix<stFix>::CTBufStreamFix(const CTBufStreamFix<stFix>&)
: m_ullPos(0)
{
}

template <size_t stFix>
INLINE CTBufStreamFix<stFix>& CTBufStreamFix<stFix>::operator=(const CTBufStreamFix<stFix>&)
{
	return (*this);
}

template <size_t stFix>
INLINE ULLong CTBufStreamFix<stFix>::Tell(void) const
{
	return (m_ullPos);
}

template <size_t stFix>
INLINE ULLong CTBufStreamFix<stFix>::Size(void) const
{
	return (stFix);
}

template <size_t stFix>
INLINE ULLong CTBufStreamFix<stFix>::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= stFix);
	assert(eFrom < SEEKO_BOUND);
	if (stFix > 0)
	{
		switch (eFrom)
		{
		case SEEKO_BEGIN:
			{
				if (llPos >= 0)
				{
					m_ullPos = DEF_BASE::Min<ULLong>((ULLong)llPos, (ULLong)stFix);
				}
			}
			break;
		case SEEKO_CURRENT:
			{
				if (llPos < 0)
				{
					llPos = DEF_BASE::Abs<LLong>(llPos);
					if ((ULLong)llPos < m_ullPos)
					{
						m_ullPos -= (ULLong)llPos;
					}
					else
					{
						m_ullPos  = 0;
					}
				}
				else if ((ULLong)llPos <= ((ULLong)stFix - m_ullPos))
				{
					m_ullPos += (ULLong)llPos;
				}
			}
			break;
		case SEEKO_END:
			{
				if (llPos <= 0)
				{
					llPos = DEF_BASE::Abs<LLong>(llPos);
					if ((ULLong)llPos <= (ULLong)stFix)
					{
						m_ullPos = (ULLong)stFix - (ULLong)llPos;
					}
				}
			}
			break;
		default:
			{
				assert(0);
			}
		}
	}
	return m_ullPos;
}

template <size_t stFix>
INLINE PByte CTBufStreamFix<stFix>::GetBuf(void)
{
	return (m_pBuf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufReadStreamFix
template <size_t stFix>
INLINE CTBufReadStreamFix<stFix>::CTBufReadStreamFix(void)
: CTBufStreamFix<stFix>(STREAMM_READ)
{
}

template <size_t stFix>
INLINE CTBufReadStreamFix<stFix>::~CTBufReadStreamFix(void)
{
}

template <size_t stFix>
INLINE CTBufReadStreamFix<stFix>::CTBufReadStreamFix(const CTBufReadStreamFix<stFix>&)
: CTBufStreamFix<stFix>(STREAMM_READ)
{
}

template <size_t stFix>
INLINE CTBufReadStreamFix<stFix>& CTBufReadStreamFix<stFix>::operator=(const CTBufReadStreamFix<stFix>&)
{
	return (*this);
}

template <size_t stFix>
INLINE ULLong CTBufReadStreamFix<stFix>::Read(void* pV, ULLong ullLenBytes)
{
	//assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		return 0;
	}
	if (IsEnd())
	{
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, ((ULLong)stFix - m_ullPos));
	MM_SAFE::Memcpy(pV, (rsize_t)ullLenBytes, (m_pBuf + m_ullPos), (rsize_t)ullLenBytes);
	m_ullPos += ullLenBytes;
	return ullLenBytes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufWriteStreamFix
template <size_t stFix>
INLINE CTBufWriteStreamFix<stFix>::CTBufWriteStreamFix(void)
: CTBufStreamFix<stFix>(STREAMM_WRITE)
{
}

template <size_t stFix>
INLINE CTBufWriteStreamFix<stFix>::~CTBufWriteStreamFix(void)
{
}

template <size_t stFix>
INLINE CTBufWriteStreamFix<stFix>::CTBufWriteStreamFix(const CTBufWriteStreamFix<stFix>&)
: CTBufStreamFix(STREAMM_WRITE)
{
}

template <size_t stFix>
INLINE CTBufWriteStreamFix<stFix>& CTBufWriteStreamFix<stFix>::operator=(const CTBufWriteStreamFix<stFix>&)
{
	return (*this);
}

template <size_t stFix>
INLINE ULLong CTBufWriteStreamFix<stFix>::Write(const void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, ((ULLong)stFix - m_ullPos));
	MM_SAFE::Memcpy((m_pBuf + m_ullPos), (rsize_t)ullLenBytes, pV, (rsize_t)ullLenBytes);
	m_ullPos += ullLenBytes;
	return ullLenBytes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufStream
INLINE CBufStream::CBufStream(Int nMode, PByte pBuf, ULLong ullSize, bool bManage)
: CBufStreamBase(nMode)
, m_bManage(false)
, m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
	Create(pBuf, ullSize, bManage);
}

INLINE CBufStream::~CBufStream(void)
{
	Close();
}

INLINE CBufStream::CBufStream(const CBufStream&)
: CBufStreamBase(0)
, m_bManage(false)
, m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
}

INLINE CBufStream& CBufStream::operator=(const CBufStream&)
{
	return (*this);
}

INLINE ULLong CBufStream::Tell(void) const
{
	return (m_ullPos);
}

INLINE ULLong CBufStream::Size(void) const
{
	return (m_ullSize);
}

INLINE ULLong CBufStream::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= m_ullSize);
	assert(eFrom < SEEKO_BOUND);
	if (m_ullSize > 0)
	{
		switch (eFrom)
		{
		case SEEKO_BEGIN:
			{
				if (llPos >= 0)
				{
					m_ullPos = DEF_BASE::Min<ULLong>((ULLong)llPos, m_ullSize);
				}
			}
			break;
		case SEEKO_CURRENT:
			{
				if (llPos < 0)
				{
					llPos = DEF_BASE::Abs<LLong>(llPos);
					if ((ULLong)llPos < m_ullPos)
					{
						m_ullPos -= (ULLong)llPos;
					}
					else
					{
						m_ullPos  = 0;
					}
				}
				else if ((ULLong)llPos <= (m_ullSize - m_ullPos))
				{
					m_ullPos += (ULLong)llPos;
				}
			}
			break;
		case SEEKO_END:
			{
				if (llPos <= 0)
				{
					llPos = DEF_BASE::Abs<LLong>(llPos);
					if ((ULLong)llPos <= m_ullSize)
					{
						m_ullPos = m_ullSize - (ULLong)llPos;
					}
				}
			}
			break;
		default:
			{
				assert(0);
			}
		}
	}
	return m_ullPos;
}

INLINE void CBufStream::Close(void)
{
	if (m_bManage && (m_pBuf != nullptr))
	{
		FREE( m_pBuf );
	}
	m_pBuf     = nullptr;
	m_ullPos   = 0;
	m_ullSize  = 0;
	m_bManage  = false;
}

INLINE PByte CBufStream::GetBuf(void)
{
	return (m_pBuf);
}

INLINE bool CBufStream::Create(PByte pBuf, ULLong ullSize, bool bManage)
{
	if (m_bManage && (m_pBuf != nullptr))
	{
		FREE(m_pBuf);
	}
	if (pBuf == nullptr)
	{
		assert(bManage == true);
		m_bManage = true;
		m_ullSize = ullSize;
		m_ullPos  = 0;
		m_pBuf    = (PByte)ALLOC( (size_t)ullSize );
		return (m_pBuf != nullptr);
	}
	else
	{
		m_bManage = bManage;
		m_ullSize = ullSize;
		m_ullPos  = 0;
		m_pBuf    = pBuf;
		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufReadStream
INLINE CBufReadStream::CBufReadStream(PByte pBuf, ULLong ullSize, bool bManage)
: CBufStream(STREAMM_READ, pBuf, ullSize, bManage)
{
}

INLINE CBufReadStream::~CBufReadStream(void)
{
}

INLINE CBufReadStream::CBufReadStream(const CBufReadStream&)
: CBufStream(STREAMM_READ)
{
}

INLINE CBufReadStream& CBufReadStream::operator=(const CBufReadStream&)
{
	return (*this);
}

INLINE ULLong CBufReadStream::Read(void* pV, ULLong ullLenBytes)
{
	//assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		return 0;
	}
	if (IsEnd())
	{
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullPos));
	MM_SAFE::Memcpy(pV, (rsize_t)ullLenBytes, (m_pBuf + m_ullPos), (rsize_t)ullLenBytes);
	m_ullPos += ullLenBytes;
	return ullLenBytes;
}

INLINE bool CBufReadStream::Refer(CStream& aSrc)
{
	if (aSrc.GetMode() & STREAMM_BUFFER)
	{
		Close();

		CBufStreamBase* pBufStream = static_cast<CBufStreamBase*>(&aSrc);
		if (aSrc.IsRead())
		{
			return Create(pBufStream->GetBuf(), aSrc.Size());
		}
		else
		{
			assert(aSrc.IsWrite());
			return Create(pBufStream->GetBuf(), aSrc.Tell());
		}
	}
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufWriteStream
INLINE CBufWriteStream::CBufWriteStream(PByte pBuf, ULLong ullSize, bool bManage)
: CBufStream(STREAMM_WRITE, pBuf, ullSize, bManage)
{
}

INLINE CBufWriteStream::~CBufWriteStream(void)
{
}

INLINE CBufWriteStream::CBufWriteStream(const CBufWriteStream&)
: CBufStream(STREAMM_WRITE)
{
}

INLINE CBufWriteStream& CBufWriteStream::operator=(const CBufWriteStream&)
{
	return (*this);
}

INLINE ULLong CBufWriteStream::Write(const void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		return 0;
	}
	if (m_ullSize == 0)
	{
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullPos));
	MM_SAFE::Memcpy((m_pBuf + m_ullPos), (rsize_t)ullLenBytes, pV, (rsize_t)ullLenBytes);
	m_ullPos += ullLenBytes;
	return ullLenBytes;
}

#endif // __STREAM_BUF_INL__