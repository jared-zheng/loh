///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : keyvaluedef.h                                                                         //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : key-value const defined                                                                    //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __KEY_VALUE_DEF_H__
#define __KEY_VALUE_DEF_H__

#pragma once

#ifndef CONST_KEYVALUE

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

	// const key-value can not be removed
	// and should use index[INDEX_KEYVALUE( ... )] to get const value 
	#define CONST_KEYVALUE_STR( hash, key, str )      KV_##key = hash,  // value is a string
	#define CONST_KEYVALUE_PTR( hash, key )           KV_##key = hash,  // value is a pointer
	#define CONST_KEYVALUE_LLONG( hash, key, ll )     KV_##key = hash,  // value is a LLong
	#define CONST_KEYVALUE_ULLONG( hash, key, ull )   KV_##key = hash,  // value is a ULLong
	#define CONST_KEYVALUE_LONG( hash, key, l )       KV_##key = hash,  // value is a Long
	#define CONST_KEYVALUE_ULONG( hash, key, ul )     KV_##key = hash,  // value is a ULong
	#define CONST_KEYVALUE_DOUBLE( hash, key, d )     KV_##key = hash,  // value is a Double
	#define INDEX_KEYVALUE( key )                     KV_##key          // key to index 
	#define KEYVALUE_ENUM
	enum EKEYVALUE 
	{
#endif

CONST_KEYVALUE_STR( 0, None, TF("CREEK::core") ) // special reserved key

#ifdef KEYVALUE_ENUM
	};
	#undef KEYVALUE_ENUM
	#undef INDEX_KEYVALUE
	#undef CONST_KEYVALUE_DOUBLE
	#undef CONST_KEYVALUE_ULONG
	#undef CONST_KEYVALUE_LONG
	#undef CONST_KEYVALUE_ULLONG
	#undef CONST_KEYVALUE_LLONG
	#undef CONST_KEYVALUE_PTR
	#undef CONST_KEYVALUE_STR

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif

#endif // __KEY_VALUE_DEF_H__