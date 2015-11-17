///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : commondef.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : common type define file                                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__

#pragma once

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
typedef            char                             Char  , *PChar;
typedef            short                            Short , *PShort;
typedef            int                              Int   , *PInt;
typedef            long                             Long  , *PLong;
typedef            long long                        LLong , *PLLong;

typedef   unsigned char                             UChar , *PUChar;
typedef   unsigned short                            UShort, *PUShort;
typedef   unsigned int                              UInt  , *PUInt;
typedef   unsigned long                             ULong , *PULong;
typedef   unsigned long long                        ULLong, *PULLong;

typedef   float                                     Float , *PFloat;
typedef   double                                    Double, *PDouble;

typedef   UChar                                     Byte  , *PByte;
typedef   wchar_t                                   WChar , *PWChar;

typedef   struct tagINDEX{ }                        INDEX, *PINDEX;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// radix
enum RADIX_TYPE
{
	RADIXT_BIN      = 2,
	RADIXT_DIGITS   = 5,
	RADIXT_DEC      = 10,
	RADIXT_HEX      = 16,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// limit length
enum LIMIT_LEN
{
	LMT_KEY         = 64,
	LMT_MIN         = 128,
	LMT_PATH        = 256,
	LMT_MAX_PATH    = 260,
	LMT_BUF         = 1024,
	LMT_MAX         = SHRT_MAX,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// return value
enum RET_VALUE
{
	RET_OKAY        = 0,
	RET_FAIL,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// variant type
enum VAR_TYPE
{
	VART_NONE       = 0x00000000,
	VART_CHAR       = 0x00000001,
	VART_UCHAR      = 0x00000002,
	VART_SHORT      = 0x00000004,
	VART_USHORT     = 0x00000008,

	VART_INT        = 0x00000010,
	VART_UINT       = 0x00000020,
	VART_LONG       = 0x00000040,
	VART_ULONG      = 0x00000080,

	VART_LLONG      = 0x00000100,
	VART_ULLONG     = 0x00000200,
	VART_FLOAT      = 0x00000400,
	VART_DOUBLE     = 0x00000800,

	VART_WCHAR      = 0x00001000,
	VART_XCHAR      = 0x00002000,
	VART_BYTE       = 0x00004000,
	VART_SIZE_T     = 0x00008000,

	VART_KEYREF     = 0x00010000, //
	VART_UUID       = 0x00020000,
	VART_STRING     = 0x00040000, // string class
	VART_CLASS      = 0x00080000, // class or struct

	VART_ARRAY      = 0x00100000,
	VART_LIST       = 0x00200000,
	VART_STACK      = 0x00400000,
	VART_QUEUE      = 0x00800000,

	VART_MAP        = 0x01000000,
	VART_RBTREE     = 0x02000000,
	VART_REFCOUNT   = 0x04000000,
	VART_OBJECT     = 0x08000000, // CObject class

	VART_PTR        = 0x10000000, // pointer
	VART_DATA       = 0x20000000, // user data
	VART_ROOT       = 0x40000000, // base class 
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// log level
enum LOG_LEVEL
{
	LOGL_MEMMG      = 0x00000040, // memory manager
	LOGL_TRACE      = 0x00000020, // deep info trace
	LOGL_DEBUG      = 0x00000010, // debug output
	LOGL_INFO       = 0x00000008, // standard information
	LOGL_DUMP       = 0x00000004, // dump information
	LOGL_WARN       = 0x00000002, // warnning information
	LOGL_ERROR      = 0x00000001, // error information
#ifdef RUNTIME_DEBUG
	LOGL_ALL        = 0x0000003F,
#else   // RUNTIME_DEBUG
	LOGL_ALL        = 0x0000000F,
#endif  // RUNTIME_DEBUG
	LOGL_DEVOUTPUT  = 0x00000000, //
	LOGL_DEVFILE    = 0x00001000,
	LOGL_DEVCONSOLE = 0x00002000,
	LOGL_DEVSYSTEM  = 0x00004000, // send log to component
};

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __COMMON_DEF_H__