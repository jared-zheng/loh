///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : mobject.h                                                                             //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : managed memory alloc/free                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MOBJECT_H__
#define __MOBJECT_H__

#pragma once

#include "core.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// managed memory
#pragma push_macro("new")
#undef new
#pragma push_macro("delete")
#undef delete
#ifdef RUNTIME_DEBUG
	#define MNEW                                    new(TF(__FILE__), TF(__FUNCTION__), (size_t)(__LINE__))                         
	#define MDELETE                                 delete                                                               
	#define ALLOC( size )                           MObject::sMAlloc(size, TF(__FILE__ ), TF(__FUNCTION__), (size_t)(__LINE__))    
	#define ALLOCEX( size, type )                   MObject::sMAlloc(size, TF(__FILE__ ), TF(__FUNCTION__), (size_t)(__LINE__), type) // page type special when size >= 32K
	#define REALLOC( pOld, newsize )                MObject::sMReAlloc(pOld, newsize, TF(__FILE__ ), TF(__FUNCTION__), (size_t)(__LINE__))     
#else   // RUNTIME_DEBUG
	#define MNEW                                    new                                                                   
	#define MDELETE                                 delete                                                              
	#define ALLOC( size )                           MObject::sMAlloc(size)    
	#define ALLOCEX( size, type )                   MObject::sMAlloc(size, type) // page type special when size >= 32K
	#define REALLOC( pOld, newsize )                MObject::sMReAlloc(pOld, newsize)   
#endif  // RUNTIME_DEBUG
#define FREE( p )                                   MObject::sMFree(p, 0)    // set size == 0 if not know it's size                                                 
#define DUMP( p )                                   MObject::sMDump(p)       // dump current memory manager alloc information                                                       
#define CHECK( p )                                  MObject::sMCheck(p)      // check the pointer's memory is not overwrite by other mobject
#pragma pop_macro("delete")
#pragma pop_macro("new")

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// MM_DUMP
typedef struct tagMM_DUMP
{
public:
	tagMM_DUMP(void)
	: llUsedSize(0)
	, llMaxUsedSize(0)
	, llCacheSize(0)
	, llMaxCacheSize(0)
	, llUsedTick(0)
	, llAllocTick(0)
	, llReallocTick(0)
	, llFreeTick(0)
	{
	}

	~tagMM_DUMP(void)
	{
	}
public:
	LLong   llUsedSize;     // MB
	LLong   llMaxUsedSize;  // MB
	LLong   llCacheSize;    // MB
	LLong   llMaxCacheSize; // MB
	LLong   llUsedTick;     // MS
	LLong   llAllocTick;    // MS
	LLong   llReallocTick;  // MS
	LLong   llFreeTick;     // MS
}MM_DUMP, *PMM_DUMP;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// MM_SAFE
class MM_SAFE {
public:
	template <typename T>
	static void DELETE_PTR(T* pV);

	template <typename T>
	static void DELETE_PTRREF(T*& pV);

	template <typename X, typename T>
	static void DELETE_PTR(X* pV);

	template <typename X, typename T>
	static void DELETE_PTRREF(X*& pV);

	static Int   Memcmp(const void* pDst, const void* pSrc, size_t stSize);
	static void* Memset(void* pDst, Int nValue, size_t stSize);
	static void* Memcpy(void* pDst, size_t stDst, const void* pSrc, size_t stSrc);
	static void* Memmov(void* pDst, size_t stDst, const void* pSrc, size_t stSrc);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// MObject
// base class for allocate/free memory from managed memory pool
class CORECLASS MObject
{
public:
	enum MEM_CONST
	{
		MEM_CHUNK_OFFSET   = 72, // max chunk offset -- MM_CHUNK_WITHBLOCK
		MEM_CACHE_MAXSIZE  = 0x80000000,
	};
public:
#ifdef RUNTIME_DEBUG
	static void*  operator new(size_t stSize, PCXStr szFile, PCXStr szFunc, size_t stLine);
	static void*  operator new[](size_t stSize, PCXStr szFile, PCXStr szFunc, size_t stLine);
	static void   operator delete(void*, PCXStr, PCXStr, size_t);
	static void   operator delete[](void*, PCXStr, PCXStr, size_t);
#else   // RUNTIME_DEBUG
	static void*  operator new(size_t stSize);
	static void*  operator new[](size_t stSize);
#endif  // RUNTIME_DEBUG
	static void   operator delete(void* p, size_t stSize); 
	static void   operator delete[](void* p, size_t stSize);

#ifdef RUNTIME_DEBUG
	static void*  sMAlloc(size_t stSize, PCXStr szFile, PCXStr szFunc, size_t stLine, size_t stType = 0);
	static void*  sMReAlloc(void* pOld, size_t stNewSize, PCXStr szFile, PCXStr szFunc, size_t stLine);
#else   // RUNTIME_DEBUG
	static void*  sMAlloc(size_t stSize, size_t stType = 0);
	static void*  sMReAlloc(void* pOld, size_t stNewSize);
#endif  // RUNTIME_DEBUG
	static void   sMFree(void* p, size_t stSize = 0);
	// dump or check special memory
	static void   sMDump(MM_DUMP& Dump);
	static void   sMDump(void* p);
	static bool   sMCheck(void* p); 
	// cache : boundary to align pagesize
	static PINDEX sMCCreate(size_t stSize, size_t stGrow = 0, size_t stMaxSize = (size_t)MEM_CACHE_MAXSIZE);
	static bool   sMCDestroy(PINDEX index);

	static size_t sMCAlignSize(PINDEX index);  // cache align size
	static size_t sMCUsedSize(PINDEX index);   // cache used size

	static PByte  sMCAlloc(PINDEX index, size_t stSize = 0, size_t stMaxSize = (size_t)MEM_CACHE_MAXSIZE); // align(stSize) <= cache align size
	static bool   sMCFree(PINDEX index, PByte pData);
	static bool   sMCCheck(PINDEX index, PByte pData);
	static void   sMCDump(PINDEX index);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#if   (__PLATFORM_TARGET__ == PLATFORM_TARGET_WINDOWS)
	#include "windows/targetmobject.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_LINUX)
	#include "linux/targetmobject.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_MAC)
	#include "mac/targetmobject.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_IOS)
	#include "ios/targetmobject.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_ANDROID)
	#include "android/targetmobject.inl"
#elif (__PLATFORM_TARGET__ == PLATFORM_TARGET_HTML5)
	#include "html5/targetmobject.inl"
#else
	#error "__PLATFORM_TARGET__ No Implement"
#endif

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __MOBJECT_H__