///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : hash.h                                                                                //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : hash based on crc table                                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HASH_H__
#define __HASH_H__

#pragma once

#include "mobject.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// HASH_DUMP
typedef struct tagHASH_DUMP
{
public:
	tagHASH_DUMP(void)
	: llUsedCount(0)
	, llUsedTick(0)
	{
	}

	~tagHASH_DUMP(void)
	{
	}
public:
	LLong   llUsedCount;
	LLong   llUsedTick;     // MS
}HASH_DUMP, *PHASH_DUMP;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CHash
class CORECLASS CHash : public MObject
{
public:
	static size_t Hash(Char c);
	static size_t Hash(UChar uc);
	static size_t Hash(Short s);
	static size_t Hash(UShort us);
	static size_t Hash(WChar wc);
	static size_t Hash(Int n);
	static size_t Hash(UInt u);
	static size_t Hash(Long l);
	static size_t Hash(ULong ul);
	static size_t Hash(LLong ll);
	static size_t Hash(ULLong ull);
	static size_t Hash(void* p);

	static size_t Hash(PCXStr pszString, size_t stLen = (size_t)LMT_MAX);
	static size_t Hash(PByte  pBuffer, size_t stLen = (size_t)LMT_MAX);

	static UInt   Hash32(LLong ll);
	static UInt   Hash32(ULLong ull);
	static UInt   Hash32(void* p);

	static UInt   Hash32(PCXStr pszString, size_t stLen = (size_t)LMT_MAX);
	static UInt   Hash32(PByte  pBuffer, size_t stLen = (size_t)LMT_MAX);

	static ULLong Hash64(LLong ll);
	static ULLong Hash64(ULLong ull);
	static ULLong Hash64(void* p);

	static ULLong Hash64(PCXStr pszString, size_t stLen = (size_t)LMT_MAX);
	static ULLong Hash64(PByte  pBuffer, size_t stLen = (size_t)LMT_MAX);

	static void   Dump(HASH_DUMP& Dump);
private:
	CHash(void);
	~CHash(void);
	CHash(const CHash&);
	CHash& operator=(const CHash&);
};

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __HASH_H__