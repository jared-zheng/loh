///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : streambuf.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : buffer serialize                                                                           //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_BUF_H__
#define __STREAM_BUF_H__

#pragma once

#include "stream.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufStreamBase
class NOVTABLE CBufStreamBase ABSTRACT: public CStream
{
public:
	virtual PByte GetBuf(void) PURE_METHOD;
protected:
	CBufStreamBase(Int nMode);
	virtual ~CBufStreamBase(void);
private:
	CBufStreamBase(const CBufStreamBase&);
	CBufStreamBase& operator=(const CBufStreamBase&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufStreamFix
template <size_t stFix = (size_t)LMT_KEY>
class NOVTABLE CTBufStreamFix ABSTRACT : public CBufStreamBase
{
public:
	virtual ULLong Tell(void) const OVERRIDE;  
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;

	virtual PByte  GetBuf(void) OVERRIDE;
protected:
	CTBufStreamFix(Int nMode);
	virtual ~CTBufStreamFix(void);
private:
	CTBufStreamFix(const CTBufStreamFix&);
	CTBufStreamFix& operator=(const CTBufStreamFix&);
protected:
	ULLong   m_ullPos;
	PByte    m_pBuf[stFix];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufReadStreamFix
template <size_t stFix = (size_t)LMT_KEY>
class CTBufReadStreamFix : public CTBufStreamFix<stFix>
{
public:
	CTBufReadStreamFix(void);
	virtual ~CTBufReadStreamFix(void);

	virtual ULLong Read(void* pV, ULLong ullLenBytes) OVERRIDE;
private:
	CTBufReadStreamFix(const CTBufReadStreamFix&);
	CTBufReadStreamFix& operator=(const CTBufReadStreamFix&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTBufWriteStreamFix
template <size_t stFix = (size_t)LMT_KEY>
class CTBufWriteStreamFix : public CTBufStreamFix<stFix>
{
public:
	CTBufWriteStreamFix(void);
	virtual ~CTBufWriteStreamFix(void);

	virtual ULLong Write(const void* pV, ULLong ullLenBytes) OVERRIDE;
private:
	CTBufWriteStreamFix(const CTBufWriteStreamFix&);
	CTBufWriteStreamFix& operator=(const CTBufWriteStreamFix&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufStream
class NOVTABLE CBufStream ABSTRACT: public CBufStreamBase
{
public:
	virtual ULLong Tell(void) const OVERRIDE;
	virtual ULLong Size(void) const OVERRIDE;

	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) OVERRIDE;

	virtual void   Close(void) OVERRIDE;

	virtual PByte  GetBuf(void) OVERRIDE;

	bool    Create(PByte pBuf, ULLong ullSize, bool bManage = false);
protected:
	CBufStream(Int nMode, PByte pBuf = nullptr, ULLong ullSize = 0, bool bManage = false);
	virtual ~CBufStream(void);
private:
	CBufStream(const CBufStream&);
	CBufStream& operator=(const CBufStream&);
protected:
	bool     m_bManage;
	ULLong   m_ullSize;
	ULLong   m_ullPos;
	PByte    m_pBuf;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufReadStream
class CBufReadStream : public CBufStream
{
public:
	CBufReadStream(PByte pBuf = nullptr, ULLong ullSize = 0, bool bManage = false);
	virtual ~CBufReadStream(void);

	virtual ULLong Read(void* pV, ULLong ullLenBytes) OVERRIDE;

	virtual bool   Refer(CStream& aSrc) OVERRIDE;
private:
	CBufReadStream(const CBufReadStream&);
	CBufReadStream& operator=(const CBufReadStream&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBufWriteStream  
class CBufWriteStream : public CBufStream
{
public:
	CBufWriteStream(PByte pBuf = nullptr, ULLong ullSize = 0, bool bManage = false);
	virtual ~CBufWriteStream(void);

	virtual ULLong Write(const void* pV, ULLong ullLenBytes) OVERRIDE;
private:
	CBufWriteStream(const CBufWriteStream&);
	CBufWriteStream& operator=(const CBufWriteStream&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "streambuf.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __STREAM_BUF_H__