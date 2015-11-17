///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : stream.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : serialize                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __STREAM_H__
#define __STREAM_H__

#pragma once

#include "tstring.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CStream
class NOVTABLE CStream ABSTRACT : public MObject
{
public:
	enum SEEK_OP
	{
		SEEKO_BEGIN        = 0x00000000,
		SEEKO_CURRENT      = 0x00000001,
		SEEKO_END          = 0x00000002,
		SEEKO_BOUND        = 0x00000003,
	};

	enum STREAM_MODE 
	{
		STREAMM_NONE       = 0x00000000,
		STREAMM_READ       = 0x00000001,
		STREAMM_WRITE      = 0x00000002,
		STREAMM_FILE       = 0x00000004,
		STREAMM_MAPPING    = 0x00000008,
		STREAMM_BUFFER     = 0x00000010,

		STREAMM_BYTESWAP   = 0x20000000,
		STREAMM_ERROR      = 0x40000000,         
	};
public:
	virtual ULLong Read(void* pV, ULLong ullLenBytes);
	virtual ULLong ReadBits(void* pV, ULLong ullLenBits);
	virtual ULLong Write(const void* pV, ULLong ullLenBytes);
	virtual ULLong WriteBits(const void* pV, ULLong ullLenBits);

	virtual ULLong Tell(void) const; // stream current read/write pos 
	virtual ULLong Size(void) const; // read stream size, write return current data size
	// read : pos <= size; write : pos == size

	// return the pos offset from stream begin
	virtual ULLong Seek(LLong llPos, SEEK_OP eFrom = SEEKO_BEGIN) PURE_METHOD;
	virtual bool   Flush(void);
	// reset all data to init status
	virtual void   Close(void);
	// refer source stream, create a scope read stream, invalid after source stream is closed
	// refer a write stream, scope read stream size is write stream tell
	// refer a read  stream, scope read stream size is read  stream size
	virtual bool   Refer(CStream& aSrc);

	virtual ~CStream(void);

	Int   GetMode(void);
	bool  IsError(void);
	bool  IsByteSwap(void);
	bool  IsEnd(void);
	bool  IsRead(void);
	bool  IsWrite(void);

	void  SetByteSwap(bool bEnabled);

	// when read  a file after end or stop, no error flag set
	// when write a file after stop, no error flag set, too

	// write operations
	// a read stream set error flag if use write operations
	CStream& operator<<(Char   c);
	CStream& operator<<(Short  s);
	CStream& operator<<(Int    n);
	CStream& operator<<(Long   l);
	CStream& operator<<(LLong  ll);

	CStream& operator<<(UChar  uc);
	CStream& operator<<(UShort us);
	CStream& operator<<(UInt   u);
	CStream& operator<<(ULong  ul);
	CStream& operator<<(ULLong ull);

	CStream& operator<<(Float  f);
	CStream& operator<<(Double d);

	CStream& operator<<(bool   b);
	CStream& operator<<(WChar  w);
	CStream& operator<<(void*  p);

	CStream& operator<<(CString& str);
	template <size_t stLenT> CStream& operator<<(CTStringFix<CXChar, stLenT>& strFix);
	CStream& operator<<(CStringRef& strRef);
	CStream& operator<<(PCXStr psz);

	// read operations
	// a write stream set error flag if use read operations
	CStream& operator>>(Char&   c);
	CStream& operator>>(Short&  s);
	CStream& operator>>(Int&    n);
	CStream& operator>>(Long&   l);
	CStream& operator>>(LLong&  ll);

	CStream& operator>>(UChar&  uc);
	CStream& operator>>(UShort& us);
	CStream& operator>>(UInt&   u);
	CStream& operator>>(ULong&  ul);
	CStream& operator>>(ULLong& ull);

	CStream& operator>>(Float&  f);
	CStream& operator>>(Double& d);

	CStream& operator>>(bool&   b);
	CStream& operator>>(WChar&  w);
	CStream& operator>>(void*&  p);

	CStream& operator>>(CString& str);
	template <size_t stLenT> CStream& operator>>(CTStringFix<CXChar, stLenT>& strFix);
protected:
	CStream(Int nMode = 0);

	void  SetError(void);
private:
	CStream(const CStream&);
	CStream& operator=(const CStream&);
private:
	Int   m_nMode;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stream.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __STREAM_H__