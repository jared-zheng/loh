///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : streamfile.h                                                                          //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : file serialize                                                                             //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_FILE_H__
#define __STREAM_FILE_H__

#pragma once

#include "stream.h"
#include "handle.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileStream
class NOVTABLE CFileStream ABSTRACT: public CStream
{
public:
	enum FILE_FLAG
	{
		FILEF_ACS_ALL           = 0x00000001, // windows : GENERIC_ALL;     linux : O_RDWR | O_CREAT mode-S_IX---
		FILEF_ACS_EXECUTE       = 0x00000002, // windows : GENERIC_EXECUTE; linux : O_CREAT mode-S_IX---
		FILEF_ACS_WRITE         = 0x00000004, // windows : GENERIC_WRITE;   linux : O_WRONLY
		FILEF_ACS_READ          = 0x00000008, // windows : GENERIC_READ;    linux : O_RDONLY 
		FILEF_ACS_MASK          = 0x0000000F,

		FILEF_NEW_NOEXIST       = 0x00000000, // windows : CREATE_NEW;        linux : O_CREAT | O_EXCL
		FILEF_NEW_ALWAYS        = 0x00000010, // windows : CREATE_ALWAYS;     linux : O_CREAT | O_TRUNC
		FILEF_OPEN_EXIST        = 0x00000020, // windows : OPEN_EXISTING;     linux : ---
		FILEF_OPEN_ALWAYS       = 0x00000040, // windows : OPEN_ALWAYS;       linux : O_CREAT
		FILEF_OPEN_TRUNCATE     = 0x00000080, // windows : TRUNCATE_EXISTING; linux : O_TRUNC
		FILEF_NEW_OPEN_MASK     = 0x000000F0,

		FILEF_ATTR_READONLY     = 0x00000100, // FILE_ATTRIBUTE_READONLY
		FILEF_ATTR_HIDDEN       = 0x00000200, // FILE_ATTRIBUTE_HIDDEN
		FILEF_ATTR_TEMP         = 0x00000400, // FILE_ATTRIBUTE_TEMPORARY
		FILEF_ATTR_AUTODEL      = 0x00000800, // FILE_FLAG_DELETE_ON_CLOSE(combine FILEF_SHARE_DELETE)

		// FILEF_ATTR_ASYNC | FILEF_ATTR_NOCACHE
		// FILEF_ATTR_CACHE_RAND & FILEF_ATTR_CACHE_LINEAR flag has no effect if the file system does not support cached I/O and FILEF_ATTR_NOCACHE
		FILEF_ATTR_ASYNC        = 0x00001000, // FILE_FLAG_OVERLAPPED
		FILEF_ATTR_NOCACHE      = 0x00002000, // FILE_FLAG_NO_BUFFERING
		FILEF_ATTR_CACHE_RAND   = 0x00004000, // FILE_FLAG_RANDOM_ACCESS
		FILEF_ATTR_CACHE_LINEAR = 0x00008000, // FILE_FLAG_SEQUENTIAL_SCAN

		FILEF_SHARE_NONE        = 0x01000000, // no share
		FILEF_SHARE_READ        = 0x02000000, // FILE_SHARE_READ
		FILEF_SHARE_WRITE       = 0x04000000, // FILE_SHARE_WRITE
		FILEF_SHARE_DELETE      = 0x08000000, // FILE_SHARE_DELETE
		FILEF_SHARE_MASK        = 0x0F000000,

		FILEF_EXT_APPEND        = 0x10000000, // write stream seek to end after open, O_APPEND 
		FILEF_EXT_NOBUF         = 0x20000000, // read  stream no buffering, for read once
	};
protected:
	CFileStream(Int nMode = STREAMM_FILE);
	virtual ~CFileStream(void);
private:
	CFileStream(const CFileStream&);
	CFileStream& operator=(const CFileStream&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileReadStream
// m_ullBufBase  m_ullBufBase + m_ullBufPos     m_ullPos
//     V               V                          V
// +---.---------------.--------------------------.---------+
// |   |<-             m_pBuf[...     ]         ->|          |
// |                                                         |
// |<-                m_ullSize                            ->|
class CFileReadStream : public CFileStream
{
public:
	enum READ_BUF
	{
		READ_BUF_MINSIZE = 4096,
		READ_BUF_MAXSIZE = 4096 * 1024,
	};
public:
	CFileReadStream(void);
	virtual ~CFileReadStream(void);

	virtual ULLong Read(void* pV, ULLong ullLenBytes) OVERRIDE;

	virtual ULLong Tell(void) const OVERRIDE;
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;

	virtual void   Close(void) OVERRIDE;

	bool     Create(PCXStr pszFile, UInt uFlag);
	CHandle& GetFileHandle(void);
private:
	CFileReadStream(const CFileReadStream&);
	CFileReadStream& operator=(const CFileReadStream&);

	ULLong   PreRead(void);
private:
	ULLong   m_ullSize;
	ULLong   m_ullPos;
	ULLong   m_ullBufSize;
	ULLong   m_ullBufBase;
	ULLong   m_ullBufPos;
	PByte    m_pBuf;
	CHandle  m_hFile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CFileWriteStream
//                       m_ullPos      m_ullBufPos
//                         V              V
// +-----------------------+              .
//                         |<-    m_pBuf[...    ]       ->|    
class CFileWriteStream : public CFileStream
{
public:
	enum WRITE_BUF
	{
		WRITE_BUF_MINSIZE = 4096,
		WRITE_BUF_MAXSIZE = 4096 * 1024,
	};
public:
	CFileWriteStream(ULLong ullBuf = (ULLong)WRITE_BUF_MINSIZE);
	virtual ~CFileWriteStream(void);

	virtual ULLong Write(const void* pV, ULLong ullLenBytes) OVERRIDE;

	virtual ULLong Tell(void) const OVERRIDE;
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;
	virtual bool   Flush(void) OVERRIDE;

	virtual void   Close(void) OVERRIDE;
	// pszFile == nullptr and uFlag & FILEF_ATTR_TEMP open a temp file
	bool     Create(PCXStr pszFile, UInt uFlag);
	CHandle& GetFileHandle(void);
private:
	CFileWriteStream(const CFileWriteStream&);
	CFileWriteStream& operator=(const CFileWriteStream&);
private:
	ULLong   m_ullPos;
	ULLong   m_ullBufSize;
	ULLong   m_ullBufPos;
	PByte    m_pBuf;
	CHandle  m_hFile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMapReadStream
class CMapReadStream : public CFileStream
{
public:
	// nMode = 0 share memory, nMode = STREAMM_FILE mapping file
	CMapReadStream(Int nMode = STREAMM_FILE);
	virtual ~CMapReadStream(void);

	virtual ULLong Read(void* pV, ULLong ullLenBytes) OVERRIDE;

	virtual ULLong Tell(void) const OVERRIDE;
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;

	virtual void   Close(void) OVERRIDE;

	bool    Create(PCXStr pszFile, ULLong ullSize, UInt uFlag, PCXStr pszName = nullptr);

	PByte   InitMap(ULLong ullSize, ULLong ullOffset = 0);
	void    ExitMap(void);
	PByte   GetMap(void);
private:
	CMapReadStream(const CMapReadStream&);
	CMapReadStream& operator=(const CMapReadStream&);
private:
	ULLong   m_ullSize;
	ULLong   m_ullPos;
	PByte    m_pBuf;
	CHandle  m_hFile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMapWriteStream
class CMapWriteStream : public CFileStream
{
public:
	// nMode = 0 share memory, nMode = STREAMM_FILE mapping file
	CMapWriteStream(Int nMode = STREAMM_FILE);
	virtual ~CMapWriteStream(void);

	virtual ULLong Write(const void* pV, ULLong ullLenBytes) OVERRIDE;

	virtual ULLong Tell(void) const OVERRIDE;
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;
	virtual bool   Flush(void) OVERRIDE;

	virtual void   Close(void) OVERRIDE;
	// pszFile == nullptr and uFlag & FILEF_ATTR_TEMP open a temp file
	bool    Create(PCXStr pszFile, ULLong ullSize, UInt uFlag, PCXStr pszName = nullptr);

	PByte   InitMap(ULLong ullSize, ULLong ullOffset = 0);
	void    ExitMap(void);
	PByte   GetMap(void);
private:
	CMapWriteStream(const CMapWriteStream&);
	CMapWriteStream& operator=(const CMapWriteStream&);
private:
	ULLong   m_ullSize;
	ULLong   m_ullPos;
	PByte    m_pBuf;
	CHandle  m_hFile;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "streamfile.inl"

#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetstreamfile.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetstreamfile.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetstreamfile.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetstreamfile.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetstreamfile.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetstreamfile.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __STREAM_FILE_H__