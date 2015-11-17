///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : targetstreamfile.inl                                                                  //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : file serialize                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __TARGET_STREAM_FILE_INL__
#define __TARGET_STREAM_FILE_INL__

#pragma once

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileReadStream
INLINE CFileReadStream::CFileReadStream(void)
: CFileStream(STREAMM_READ | STREAMM_FILE)
, m_ullSize(0)
, m_ullPos(0)
, m_ullBufSize(0)
, m_ullBufBase(0)
, m_ullBufPos(0)
, m_pBuf(nullptr)
{
}

INLINE CFileReadStream::~CFileReadStream(void)
{
	Close();
}

INLINE CFileReadStream::CFileReadStream(const CFileReadStream&)
: m_ullSize(0)
, m_ullPos(0)
, m_ullBufSize(0)
, m_ullBufBase(0)
, m_ullBufPos(0)
, m_pBuf(nullptr)
{
}

INLINE CFileReadStream& CFileReadStream::operator=(const CFileReadStream&)
{
	return (*this);
}

INLINE ULLong CFileReadStream::Read(void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		DEV_DEBUG(TF("file read stream out buf invalid"));
		return 0;
	}
	if ((m_hFile.IsValid() == false) || IsEnd())
	{
		DEV_DEBUG(TF("file read stream invalid handle or EOF"));
		return 0;
	}
	if (m_ullBufSize > 0)
	{
		if ((m_pBuf == nullptr) && (PreRead() == 0))
		{
			DEV_DEBUG(TF("file read stream read failed"));
			return 0;
		}
		assert(m_ullBufSize >= m_ullBufPos);
		ULLong ullRead = 0;
		ULLong ullSize = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullBufBase - m_ullBufPos));
		if (ullSize > (m_ullBufSize - m_ullBufPos)) // read size > buffer size
		{
			// 1. read data from buffer first
			assert(m_ullPos >= (m_ullBufBase + m_ullBufPos));
			if (m_ullPos > (m_ullBufBase + m_ullBufPos))
			{
				ullRead = (m_ullBufSize - m_ullBufPos);
				MM_SAFE::Memcpy(pV, (rsize_t)ullRead, (m_pBuf + m_ullBufPos), (rsize_t)ullRead);

				m_ullBufPos  = m_ullBufSize;
				ullSize     -= ullRead;
			}

			// 2. read data from file directly(times of buffer size)
			ULLong ullDirect = (ullSize / m_ullBufSize) * m_ullBufSize;
			if (ullDirect > 0)
			{
				if (::ReadFile(m_hFile.m_krHandle, ((PByte)pV + ullRead), (ULong)ullDirect, (PULong)&ullDirect, nullptr) != FALSE)
				{
					ullSize      -= ullDirect;
					ullRead      += ullDirect;
					m_ullPos     += ullDirect;
					m_ullBufBase  = m_ullPos;
					m_ullBufPos   = 0;
				}
				else
				{
					DEV_DEBUG(TF("file read failed1 : %d"), ::GetLastError());
					SetError();
					return ullRead;
				}
			}
		}
		if (ullSize > 0) // remain data copy to buffer, then read data from buffer
		{
			if (m_ullPos == (m_ullBufBase + m_ullBufPos)) // buffer is empty
			{
				ullSize = DEF_BASE::Min<ULLong>(ullSize, PreRead());
			}
			if (ullSize > 0) // read data size is within buffer size
			{
				MM_SAFE::Memcpy(((PByte)pV + ullRead), (rsize_t)ullSize, (m_pBuf + m_ullBufPos), (rsize_t)ullSize);
				m_ullBufPos += ullSize;
				ullRead     += ullSize;
			}
		}
		return ullRead;
	}
	else // m_ullBufSize = 0
	{
		assert(m_ullBufBase == 0);
		assert(m_ullBufPos  == m_ullPos);
		assert(m_pBuf == nullptr);
		ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullPos));
		if (::ReadFile(m_hFile.m_krHandle, pV, (ULong)ullLenBytes, (PULong)&ullLenBytes, nullptr) != FALSE)
		{
			m_ullPos    += ullLenBytes;
			m_ullBufPos += ullLenBytes;
		}
		else
		{
			DEV_DEBUG(TF("file read failed2 : %d"), ::GetLastError());
			SetError();
			ullLenBytes = 0;
		}
		return ullLenBytes;
	}
}

INLINE ULLong CFileReadStream::Tell(void) const
{
	return (m_ullBufBase + m_ullBufPos);
}

INLINE ULLong CFileReadStream::Size(void) const
{
	return m_ullSize;
}

INLINE ULLong CFileReadStream::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= m_ullSize);
	assert(eFrom < SEEKO_BOUND);
	if ((m_hFile.IsValid()) && (IsError() == false))
	{
		if ((eFrom == SEEKO_CURRENT) && (m_ullBufSize > 0))
		{
			if (llPos > 0)
			{
				if ((ULLong)llPos <= (m_ullBufSize - m_ullBufPos))
				{
					m_ullBufPos += (ULLong)llPos;
					return (m_ullBufBase + m_ullBufPos);
				}
				else
				{
					llPos -= (LLong)(m_ullBufSize - m_ullBufPos);
				}
			}
			else if (llPos == 0)
			{
				return (m_ullBufBase + m_ullBufPos);
			}
			else
			{
				ULLong ullPos = (ULLong)DEF_BASE::Abs<LLong>(llPos);
				if (ullPos <= m_ullBufPos)
				{
					m_ullBufPos -= ullPos;
					return (m_ullBufBase + m_ullBufPos);
				}
				else
				{
					ullPos  = m_ullBufSize - m_ullBufPos;
					llPos  -= (LLong)ullPos;
				}
			}
		}
		LARGE_INTEGER liPos = { 0 };
		liPos.QuadPart = llPos;
		LARGE_INTEGER liNewPos = { 0 };

		if (::SetFilePointerEx(m_hFile.m_krHandle, liPos, &liNewPos, eFrom) != FALSE)
		{
			m_ullPos     = (ULLong)liNewPos.QuadPart;
			m_ullBufBase = m_ullPos;
			m_ullBufPos  = 0;
		}
	}
	return m_ullPos;
}

INLINE void CFileReadStream::Close(void)
{
	m_hFile.Close();

	m_ullSize = 0;
	m_ullPos = 0;
	m_ullBufSize = 0;
	m_ullBufBase = 0;
	m_ullBufPos = 0;
	if (m_pBuf != nullptr)
	{
		FREE(m_pBuf);
		m_pBuf = nullptr;
	}
	CStream::Close();
}

INLINE bool CFileReadStream::Create(PCXStr pszFile, UInt uFlag)
{
	if (m_hFile.IsValid() == false)
	{
		ULong ulAccess = GENERIC_READ;
		if (uFlag & FILEF_ACS_WRITE)
		{
			ulAccess |= GENERIC_WRITE;
		}
		if (uFlag & FILEF_ACS_EXECUTE)
		{
			ulAccess |= GENERIC_EXECUTE;
		}
		if (uFlag & FILEF_ACS_ALL)
		{
			ulAccess = GENERIC_ALL;
		}

		ULong ulShare = 0;
		if (uFlag & FILEF_SHARE_READ)
		{
			ulShare |= FILE_SHARE_READ;
		}
		else if (uFlag & FILEF_SHARE_WRITE)
		{
			ulShare |= FILE_SHARE_WRITE;
		}
		else if (uFlag & FILEF_SHARE_DELETE)
		{
			ulShare |= FILE_SHARE_DELETE;
		}

		ULong ulAttrs = 0;
		if (uFlag & FILEF_ATTR_READONLY)
		{
			ulAttrs |= FILE_ATTRIBUTE_READONLY;
		}
		if (uFlag & FILEF_ATTR_HIDDEN)
		{
			ulAttrs |= FILE_ATTRIBUTE_HIDDEN;
		}
		if (uFlag & FILEF_ATTR_TEMP)
		{
			ulAttrs |= FILE_ATTRIBUTE_TEMPORARY;
		}
		if (uFlag & FILEF_ATTR_AUTODEL)
		{
			ulAttrs |= FILE_FLAG_DELETE_ON_CLOSE;
			ulShare |= FILE_SHARE_DELETE;
		}
		if (uFlag & FILEF_ATTR_ASYNC)
		{
			ulAttrs |= FILE_FLAG_OVERLAPPED;
		}
		if (uFlag & FILEF_ATTR_NOCACHE)
		{
			ulAttrs |= FILE_FLAG_NO_BUFFERING;
		}
		else
		{
			if (uFlag & FILEF_ATTR_CACHE_RAND)
			{
				ulAttrs |= FILE_FLAG_RANDOM_ACCESS;
			}
			else if (uFlag & FILEF_ATTR_CACHE_LINEAR)
			{
				ulAttrs |= FILE_FLAG_SEQUENTIAL_SCAN;
			}
		}

		m_hFile = ::CreateFile(pszFile, ulAccess, ulShare, nullptr, OPEN_EXISTING, ulAttrs, nullptr);
		DEV_DEBUG(TF("file read create or open return : %d"), ::GetLastError());
		if (m_hFile != KEYREF_INVALID)
		{
			LARGE_INTEGER liSize = { 0 };
			if (::GetFileSizeEx(m_hFile.m_krHandle, &liSize) != FALSE)
			{
				m_ullSize = (ULLong)liSize.QuadPart;
				if ((uFlag & FILEF_EXT_NOBUF) == 0)
				{
					m_ullBufSize = DEF_BASE::Maxmin<ULLong>((ULLong)READ_BUF_MINSIZE, m_ullSize, (ULLong)READ_BUF_MAXSIZE);
					m_ullBufSize = DEF_BASE::Align<ULLong>(m_ullBufSize, (ULLong)READ_BUF_MINSIZE) - MEM_CHUNK_OFFSET;
				}
				return true;
			}
		}
		SetError();
	}
	return false;
}

INLINE CHandle& CFileReadStream::GetFileHandle(void)
{
	return (m_hFile);
}

INLINE ULLong CFileReadStream::PreRead(void)
{
	if ((IsError() == false) && (m_ullBufSize > 0))
	{
		if (m_pBuf == nullptr)
		{
			m_pBuf = reinterpret_cast<PByte>( ALLOC( (size_t)m_ullBufSize ) );
		}
		if (m_pBuf != nullptr)
		{
			ULLong ullSize = DEF_BASE::Min<ULLong>(m_ullBufSize, (m_ullSize - m_ullPos));
			if (::ReadFile(m_hFile.m_krHandle, m_pBuf, (ULong)ullSize, (PULong)&ullSize, nullptr) != FALSE)
			{
				m_ullBufBase  = m_ullPos;
				m_ullBufPos   = 0;
				m_ullPos     += ullSize;
				return ullSize;
			}
			DEV_DEBUG(TF("file read failed3 : %d"), ::GetLastError());
			SetError();
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileWriteStream
INLINE CFileWriteStream::CFileWriteStream(ULLong ullBuf)
: CFileStream(STREAMM_WRITE | STREAMM_FILE)
, m_ullPos(0)
, m_ullBufSize(ullBuf)
, m_ullBufPos(0)
, m_pBuf(nullptr)
{
}

INLINE CFileWriteStream::~CFileWriteStream(void)
{
	Close();
}

INLINE CFileWriteStream::CFileWriteStream(const CFileWriteStream&)
: m_ullPos(0)
, m_ullBufSize(0)
, m_ullBufPos(0)
, m_pBuf(nullptr)
{
}

INLINE CFileWriteStream& CFileWriteStream::operator=(const CFileWriteStream&)
{
	return (*this);
}

INLINE ULLong CFileWriteStream::Write(const void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		DEV_DEBUG(TF("file write stream in buf invalid"));
		return 0;
	}
	if ((m_hFile.IsValid() == false) || IsError())
	{
		DEV_DEBUG(TF("file write stream invalid handle or error"));
		return 0;
	}
	if (m_pBuf != nullptr)
	{
		assert(m_ullBufSize > 0);
		ULLong ullSize = m_ullBufSize - m_ullBufPos;
		if (ullSize > ullLenBytes)
		{
			MM_SAFE::Memcpy((m_pBuf + m_ullBufPos), (rsize_t)ullLenBytes, pV, (rsize_t)ullLenBytes);
			m_ullBufPos += ullLenBytes;
			ullSize = ullLenBytes;
		}
		else if (ullLenBytes > m_ullBufSize)
		{
			ULong ulRet = 0;
			// write buffer data
			if (m_ullBufPos > 0)
			{
				::WriteFile(m_hFile.m_krHandle, m_pBuf, (ULong)m_ullBufPos, &ulRet, nullptr);
				m_ullPos    += m_ullBufPos;
				m_ullBufPos  = 0;
			}
			//
			if (::WriteFile(m_hFile.m_krHandle, pV, (ULong)ullLenBytes, &ulRet, nullptr) != FALSE)
			{
				m_ullPos += ullLenBytes;
				ullSize   = ullLenBytes;
			}
			else
			{
				DEV_DEBUG(TF("file write failed1 : %d"), ::GetLastError());
				SetError();
				ullSize = 0;
			}
		}
		else
		{
			if (ullSize > 0)
			{
				ULong ulRet = 0;
				MM_SAFE::Memcpy((m_pBuf + m_ullBufPos), (rsize_t)ullSize, pV, (rsize_t)ullSize);
				::WriteFile(m_hFile.m_krHandle, m_pBuf, (ULong)m_ullBufSize, &ulRet, nullptr);
				m_ullPos    += m_ullBufSize;
				m_ullBufPos  = 0;
			}
			m_ullBufPos = ullLenBytes - ullSize;
			if (m_ullBufPos > 0)
			{
				MM_SAFE::Memcpy(m_pBuf, (rsize_t)m_ullBufPos, ((const PByte)pV + ullSize), (rsize_t)m_ullBufPos);
			}
			ullSize = ullLenBytes;
		}
		return ullSize;
	}
	else
	{
		assert(m_ullBufPos == 0);
		ULong ulRet = 0;
		if (::WriteFile(m_hFile.m_krHandle, pV, (ULong)ullLenBytes, &ulRet, nullptr) != FALSE)
		{
			m_ullPos += ullLenBytes;
			return ullLenBytes;
		}
		else
		{
			DEV_DEBUG(TF("file write failed2 : %d"), ::GetLastError());
			SetError();
		}
	}
	return 0;
}

INLINE ULLong CFileWriteStream::Tell(void) const
{
	return (m_ullPos + m_ullBufPos);
}

INLINE ULLong CFileWriteStream::Size(void) const
{
	return (m_ullPos + m_ullBufPos);
}

INLINE ULLong CFileWriteStream::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= Tell());
	assert(eFrom < SEEKO_BOUND);
	if ((m_hFile.IsValid()) && (IsError() == false))
	{
		if (m_ullBufPos > 0)
		{
			ULong ulRet = 0;
			::WriteFile(m_hFile.m_krHandle, m_pBuf, (ULong)m_ullBufPos, &ulRet, nullptr);
			m_ullPos    += m_ullBufPos;
			m_ullBufPos  = 0;
		}
		LARGE_INTEGER liPos = { 0 };
		liPos.QuadPart = llPos;
		LARGE_INTEGER liNewPos = { 0 };

		if (::SetFilePointerEx(m_hFile.m_krHandle, liPos, &liNewPos, eFrom) != FALSE)
		{
			m_ullPos     = (ULLong)liNewPos.QuadPart;
			m_ullBufPos  = 0;
		}
	}
	return m_ullPos;
}

INLINE bool CFileWriteStream::Flush(void)
{
	if (m_hFile.IsValid())
	{
		if (m_ullBufPos > 0)
		{
			ULong ulRet = 0;
			::WriteFile(m_hFile.m_krHandle, m_pBuf, (ULong)m_ullBufPos, &ulRet, nullptr);
			m_ullPos    += m_ullBufPos;
			m_ullBufPos  = 0;
		}
		::FlushFileBuffers(m_hFile.m_krHandle);
		return true;
	}
	return false;
}

INLINE void CFileWriteStream::Close(void)
{
	Flush();
	m_hFile.Close();

	m_ullPos = 0;
	m_ullBufSize = 0;
	m_ullBufPos = 0;
	if (m_pBuf != nullptr)
	{
		FREE(m_pBuf);
		m_pBuf = nullptr;
	}
	CStream::Close();
}

INLINE bool CFileWriteStream::Create(PCXStr pszFile, UInt uFlag)
{
	if (m_hFile.IsValid() == false)
	{
		ULong ulNewOrOpen = OPEN_ALWAYS;
		if (uFlag & FILEF_NEW_NOEXIST)
		{
			ulNewOrOpen = CREATE_NEW;
		}
		else if (uFlag & FILEF_NEW_ALWAYS)
		{
			ulNewOrOpen = CREATE_ALWAYS;
		}
		else if (uFlag & FILEF_OPEN_EXIST)
		{
			ulNewOrOpen = OPEN_EXISTING;
		}
		else if (uFlag & FILEF_OPEN_TRUNCATE)
		{
			ulNewOrOpen = TRUNCATE_EXISTING;
		}

		ULong ulAccess = GENERIC_WRITE;
		if (uFlag & FILEF_ACS_READ)
		{
			ulAccess |= GENERIC_READ;
		}
		if (uFlag & FILEF_ACS_EXECUTE)
		{
			ulAccess |= GENERIC_EXECUTE;
		}
		if (uFlag & FILEF_ACS_ALL)
		{
			ulAccess = GENERIC_ALL;
		}

		ULong ulShare = 0;
		if (uFlag & FILEF_SHARE_READ)
		{
			ulShare |= FILE_SHARE_READ;
		}
		else if (uFlag & FILEF_SHARE_WRITE)
		{
			ulShare |= FILE_SHARE_WRITE;
		}
		else if (uFlag & FILEF_SHARE_DELETE)
		{
			ulShare |= FILE_SHARE_DELETE;
		}

		ULong ulAttrs = 0;
		if (uFlag & FILEF_ATTR_HIDDEN)
		{
			ulAttrs |= FILE_ATTRIBUTE_HIDDEN;
		}
		if (uFlag & FILEF_ATTR_TEMP)
		{
			ulAttrs |= FILE_ATTRIBUTE_TEMPORARY;
		}
		if (uFlag & FILEF_ATTR_AUTODEL)
		{
			ulAttrs |= FILE_FLAG_DELETE_ON_CLOSE;
			ulShare |= FILE_SHARE_DELETE;
		}
		if (uFlag & FILEF_ATTR_ASYNC)
		{
			ulAttrs |= FILE_FLAG_OVERLAPPED;
		}
		if (uFlag & FILEF_ATTR_NOCACHE)
		{
			ulAttrs |= FILE_FLAG_NO_BUFFERING;
		}
		else
		{
			if (uFlag & FILEF_ATTR_CACHE_RAND)
			{
				ulAttrs |= FILE_FLAG_RANDOM_ACCESS;
			}
			else if (uFlag & FILEF_ATTR_CACHE_LINEAR)
			{
				ulAttrs |= FILEF_ATTR_CACHE_LINEAR;
			}
		}

		if ((pszFile == nullptr) && (uFlag & FILEF_ATTR_TEMP))
		{
			XChar szPath[LMT_MAX_PATH + LMT_MAX_PATH] = { 0 };

			PXStr pszTemp = (szPath + LMT_MAX_PATH);
			ULong ulRet   = ::GetTempPath(LMT_MAX_PATH, szPath);
			if ((ulRet > 0) && (ulRet < LMT_MAX_PATH))
			{
				if (::GetTempFileName(szPath, TF("CREEK"), 0, pszTemp) > 0)
				{
					m_hFile = ::CreateFile(pszTemp, GENERIC_WRITE, ulShare, nullptr, ulNewOrOpen, ulAttrs, nullptr);
					DEV_DEBUG(TF("file write create or open temp return : %d"), ::GetLastError());
				}
			}
		}
		else
		{
			m_hFile.m_krHandle = ::CreateFile(pszFile, ulAccess, ulShare, nullptr, ulNewOrOpen, ulAttrs, nullptr);
			DEV_DEBUG(TF("file write create or open return : %d"), ::GetLastError());
		}
		if (m_hFile != KEYREF_INVALID)
		{
			if (uFlag & FILEF_EXT_APPEND)
			{
				Seek(0, SEEKO_END);
			}
			if ((uFlag & FILEF_EXT_NOBUF) == 0)
			{
				m_ullBufSize = DEF_BASE::Maxmin<ULLong>((ULLong)WRITE_BUF_MINSIZE, m_ullBufSize, (ULLong)WRITE_BUF_MAXSIZE);
				m_ullBufSize = DEF_BASE::Align<ULLong>(m_ullBufSize, (ULLong)WRITE_BUF_MINSIZE) - MEM_CHUNK_OFFSET;
				assert(m_pBuf == nullptr);
				m_pBuf = reinterpret_cast<PByte>( ALLOC( (size_t)m_ullBufSize ) );
				if (m_pBuf != nullptr)
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		SetError();
	}
	return false;
}

INLINE CHandle& CFileWriteStream::GetFileHandle(void)
{
	return (m_hFile);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMapReadStream
INLINE CMapReadStream::CMapReadStream(Int nMode)
: CFileStream(STREAMM_READ | (nMode & STREAMM_FILE) | STREAMM_MAPPING)
, m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
}

INLINE CMapReadStream::~CMapReadStream(void)
{
	Close();
}

INLINE CMapReadStream::CMapReadStream(const CMapReadStream&)
: m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
}

INLINE CMapReadStream& CMapReadStream::operator=(const CMapReadStream&)
{
	return (*this);
}

INLINE ULLong CMapReadStream::Read(void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		DEV_DEBUG(TF("map read stream out buf invalid"));
		return 0;
	}
	if ((m_hFile.IsValid() == false) || IsEnd())
	{
		DEV_DEBUG(TF("map read stream invalid handle or EOF"));
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullPos));
	MM_SAFE::Memcpy(pV, (rsize_t)ullLenBytes, (m_pBuf + m_ullPos), (rsize_t)ullLenBytes);
	m_ullPos  += ullLenBytes;
	return ullLenBytes;
}

INLINE ULLong CMapReadStream::Tell(void) const
{
	return m_ullPos;
}

INLINE ULLong CMapReadStream::Size(void) const
{
	return m_ullSize;
}

INLINE ULLong CMapReadStream::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= m_ullSize);
	assert(eFrom < SEEKO_BOUND);
	if ((m_hFile.IsValid()) && (m_ullSize > 0))
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

INLINE void CMapReadStream::Close(void)
{
	ExitMap();
	m_hFile.Close();
	CStream::Close();
}

INLINE bool CMapReadStream::Create(PCXStr pszFile, ULLong ullSize, UInt uFlag, PCXStr pszName)
{
	if (m_hFile.IsValid() == false)
	{
		if (GetMode() & STREAMM_FILE)
		{
			ULong ulAttrs = 0;
			if (uFlag & FILEF_ATTR_HIDDEN)
			{
				ulAttrs |= FILE_ATTRIBUTE_HIDDEN;
			}
			if (uFlag & FILEF_ATTR_TEMP)
			{
				ulAttrs |= FILE_ATTRIBUTE_TEMPORARY;
			}
			if (uFlag & FILEF_ATTR_AUTODEL)
			{
				ulAttrs |= FILE_FLAG_DELETE_ON_CLOSE;
			}
			if (uFlag & FILEF_ATTR_ASYNC)
			{
				ulAttrs |= FILE_FLAG_OVERLAPPED;
			}
			if (uFlag & FILEF_ATTR_NOCACHE)
			{
				ulAttrs |= FILE_FLAG_NO_BUFFERING;
			}
			else
			{
				if (uFlag & FILEF_ATTR_CACHE_RAND)
				{
					ulAttrs |= FILE_FLAG_RANDOM_ACCESS;
				}
				else if (uFlag & FILEF_ATTR_CACHE_LINEAR)
				{
					ulAttrs |= FILE_FLAG_SEQUENTIAL_SCAN;
				}
			}

			KeyRef krFile = ::CreateFile(pszFile, GENERIC_READ, 
			                             (uFlag & FILEF_ATTR_AUTODEL) ? (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE) : (FILE_SHARE_READ|FILE_SHARE_WRITE), 
				                         nullptr, OPEN_EXISTING, ulAttrs, nullptr);
			if (krFile != KEYREF_INVALID)
			{
				ULARGE_INTEGER uliSize = { 0 };
				if (ullSize > 0)
				{
					::GetFileSizeEx(krFile, (PLARGE_INTEGER)&uliSize);
					uliSize.QuadPart  = DEF_BASE::Min<ULLong>(uliSize.QuadPart, ullSize);
				}
				m_hFile.m_krHandle = ::CreateFileMapping(krFile, nullptr, PAGE_READWRITE, uliSize.HighPart, uliSize.LowPart, pszName);

				::CloseHandle(krFile);
			}
		}
		else
		{
			ULARGE_INTEGER uliSize = { 0 };
			uliSize.QuadPart  = ullSize;
			m_hFile.m_krHandle = ::CreateFileMapping(KEYREF_INVALID, nullptr, PAGE_READWRITE, uliSize.HighPart, uliSize.LowPart, pszName);
		}
	}
	return m_hFile.IsValid();
}

INLINE PByte CMapReadStream::InitMap(ULLong ullSize, ULLong ullOffset)
{
	if (m_hFile.IsValid() && (m_pBuf == nullptr))
	{
		ULARGE_INTEGER uliOffset = { 0 };
		uliOffset.QuadPart = ullOffset;
		m_pBuf = reinterpret_cast<PByte>(::MapViewOfFile(m_hFile.m_krHandle, FILE_MAP_READ, uliOffset.HighPart, uliOffset.LowPart, (SIZE_T)ullSize));
		if (m_pBuf != nullptr)
		{
			m_ullSize = ullSize;
		}
		else
		{
			SetError();
		}
	}
	return (m_pBuf);
}

INLINE void CMapReadStream::ExitMap(void)
{
	if (m_pBuf != nullptr)
	{
		::UnmapViewOfFile(m_pBuf);
		m_pBuf = nullptr;
	}
	m_ullSize = 0;
	m_ullPos  = 0;
}

INLINE PByte CMapReadStream::GetMap(void)
{
	return (m_pBuf);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMapWriteStream
INLINE CMapWriteStream::CMapWriteStream(Int nMode)
: CFileStream(STREAMM_WRITE | (nMode & STREAMM_FILE) | STREAMM_MAPPING)
, m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
}

INLINE CMapWriteStream::~CMapWriteStream(void)
{
	Close();
}

INLINE CMapWriteStream::CMapWriteStream(const CMapWriteStream&)
: m_ullSize(0)
, m_ullPos(0)
, m_pBuf(nullptr)
{
}

INLINE CMapWriteStream& CMapWriteStream::operator=(const CMapWriteStream&)
{
	return (*this);
}

INLINE ULLong CMapWriteStream::Write(const void* pV, ULLong ullLenBytes)
{
	assert(ullLenBytes > 0);
	assert(pV != nullptr);
	if ((pV == nullptr) || (ullLenBytes == 0))
	{
		DEV_DEBUG(TF("map write stream out buf invalid"));
		return 0;
	}
	if ((m_hFile.IsValid() == false) || IsError())
	{
		DEV_DEBUG(TF("map write stream invalid handle or error"));
		return 0;
	}
	ullLenBytes = DEF_BASE::Min<ULLong>(ullLenBytes, (m_ullSize - m_ullPos));
	MM_SAFE::Memcpy((m_pBuf + m_ullPos), (rsize_t)ullLenBytes, pV, (rsize_t)ullLenBytes);
	m_ullPos += ullLenBytes;
	return ullLenBytes;
}

INLINE ULLong CMapWriteStream::Tell(void) const
{
	return m_ullPos;
}

INLINE ULLong CMapWriteStream::Size(void) const
{
	return m_ullSize;
}

INLINE ULLong CMapWriteStream::Seek(LLong llPos, SEEK_OP eFrom)
{
	assert((ULLong)DEF_BASE::Abs<LLong>(llPos) <= m_ullSize);
	assert(eFrom < SEEKO_BOUND);
	if ((m_hFile.IsValid()) && (m_ullSize > 0))
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

INLINE bool CMapWriteStream::Flush(void)
{
	if (m_pBuf != nullptr)
	{
		::FlushViewOfFile(m_pBuf, (SIZE_T)m_ullPos);
		return true;
	}
	return false;
}

INLINE void CMapWriteStream::Close(void)
{
	ExitMap();
	m_hFile.Close();
	CStream::Close();
}

INLINE bool CMapWriteStream::Create(PCXStr pszFile, ULLong ullSize, UInt uFlag, PCXStr pszName)
{
	if (m_hFile.IsValid() == false)
	{
		if (GetMode() & STREAMM_FILE)
		{
			ULong ulNewOrOpen = OPEN_ALWAYS;
			if (uFlag & FILEF_NEW_NOEXIST)
			{
				ulNewOrOpen = CREATE_NEW;
			}
			else if (uFlag & FILEF_NEW_ALWAYS)
			{
				ulNewOrOpen = CREATE_ALWAYS;
			}
			else if (uFlag & FILEF_OPEN_EXIST)
			{
				ulNewOrOpen = OPEN_EXISTING;
			}
			else if (uFlag & FILEF_OPEN_TRUNCATE)
			{
				ulNewOrOpen = TRUNCATE_EXISTING;
			}

			ULong ulAttrs = 0;
			if (uFlag & FILEF_ATTR_HIDDEN)
			{
				ulAttrs |= FILE_ATTRIBUTE_HIDDEN;
			}
			if (uFlag & FILEF_ATTR_TEMP)
			{
				ulAttrs |= FILE_ATTRIBUTE_TEMPORARY;
			}
			if (uFlag & FILEF_ATTR_AUTODEL)
			{
				ulAttrs |= FILE_FLAG_DELETE_ON_CLOSE;
			}
			if (uFlag & FILEF_ATTR_ASYNC)
			{
				ulAttrs |= FILE_FLAG_OVERLAPPED;
			}
			if (uFlag & FILEF_ATTR_NOCACHE)
			{
				ulAttrs |= FILE_FLAG_NO_BUFFERING;
			}
			else
			{
				if (uFlag & FILEF_ATTR_CACHE_RAND)
				{
					ulAttrs |= FILE_FLAG_RANDOM_ACCESS;
				}
				else if (uFlag & FILEF_ATTR_CACHE_LINEAR)
				{
					ulAttrs |= FILE_FLAG_SEQUENTIAL_SCAN;
				}
			}

			KeyRef krFile = KEYREF_INVALID;
			if ((pszFile == nullptr) && (uFlag & FILEF_ATTR_TEMP))
			{
				XChar szPath[LMT_MAX_PATH + LMT_MAX_PATH] = { 0 };
				PXStr pszTemp = (szPath + LMT_MAX_PATH);
				ULong ulRet   = ::GetTempPath(LMT_MAX_PATH, szPath);
				if ((ulRet > 0) && (ulRet < LMT_MAX_PATH))
				{
					if (::GetTempFileName(szPath, TF("CREEK"), 0, pszTemp) > 0)
					{
						krFile = ::CreateFile(pszTemp, GENERIC_WRITE, 
							                  (uFlag & FILEF_ATTR_AUTODEL) ? (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE) : (FILE_SHARE_READ|FILE_SHARE_WRITE), 
											  nullptr, ulNewOrOpen, ulAttrs, nullptr);
					}
				}
			}
			else
			{
				krFile = ::CreateFile(pszFile, GENERIC_WRITE, 
									  (uFlag & FILEF_ATTR_AUTODEL) ? (FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE) : (FILE_SHARE_READ|FILE_SHARE_WRITE), 
									  nullptr, ulNewOrOpen, ulAttrs, nullptr);
			}

			if (krFile != KEYREF_INVALID)
			{
				ULARGE_INTEGER uliSize = { 0 };
				if (ullSize > 0)
				{
					::GetFileSizeEx(krFile, (PLARGE_INTEGER)&uliSize);
					uliSize.QuadPart  = DEF_BASE::Min<ULLong>(uliSize.QuadPart, ullSize);
				}
				m_hFile.m_krHandle = ::CreateFileMapping(krFile, nullptr, PAGE_READWRITE, uliSize.HighPart, uliSize.LowPart, pszName);

				::CloseHandle(krFile);
			}
		}
		else
		{
			ULARGE_INTEGER uliSize = { 0 };
			uliSize.QuadPart  = ullSize;
			m_hFile.m_krHandle = ::CreateFileMapping(KEYREF_INVALID, nullptr, PAGE_READWRITE, uliSize.HighPart, uliSize.LowPart, pszName);
		}
	}
	return m_hFile.IsValid();
}

INLINE PByte CMapWriteStream::InitMap(ULLong ullSize, ULLong ullOffset)
{
	if (m_hFile.IsValid() && (m_pBuf == nullptr))
	{
		ULARGE_INTEGER uliOffset = { 0 };
		uliOffset.QuadPart = ullOffset;
		m_pBuf = reinterpret_cast<PByte>(::MapViewOfFile(m_hFile.m_krHandle, FILE_MAP_WRITE, uliOffset.HighPart, uliOffset.LowPart, (SIZE_T)ullSize));
		if (m_pBuf != nullptr)
		{
			m_ullSize = ullSize;
		}
		else
		{
			SetError();
		}
	}
	return (m_pBuf);
}

INLINE void CMapWriteStream::ExitMap(void)
{
	if (m_pBuf != nullptr)
	{
		::UnmapViewOfFile(m_pBuf);
		m_pBuf = nullptr;
	}
	m_ullSize = 0;
	m_ullPos  = 0;
}

INLINE PByte CMapWriteStream::GetMap(void)
{
	return (m_pBuf);
}

#endif // (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)

#endif // __TARGET_STREAM_FILE_INL__