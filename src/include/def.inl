///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : def.inl                                                                               //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : basic type define file                                                                     //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DEF_INL__
#define __DEF_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// global INLINE functions
template <typename T> 
INLINE T DEF_BASE::Abs(const T t)
{
	return (t >= (T)0) ? t : -t;
}

template <typename T>
INLINE T DEF_BASE::Max(const T a, const T b)
{
	return (a >= b) ? a : b;
}

template <typename T> 
INLINE T DEF_BASE::Min(const T a, const T b)
{
	return (a <= b) ? a : b;
}

template <typename T>
INLINE T DEF_BASE::Maxmin(const T maxa, const T maxb, const T minc)
{
	const T t = (maxa >= maxb) ? maxa : maxb;
	return (t <= minc) ? t : minc;
}

template <typename T>
INLINE T DEF_BASE::Align(const T t, const T align)
{
	return (T)((t + (align - 1)) & ~(align - 1));
}

template <typename T>
INLINE T DEF_BASE::Swap(T& a, T& b)
{
	const T t = a;
	a = b;
	b = t;
	return t;
}

INLINE CPCXStr DEF_BASE::StringVarType(size_t stType)
{
	switch (stType)
	{
		CASE_RET(VART_NONE,     TF("void"))
		CASE_RET(VART_CHAR,     TF("Char"))
		CASE_RET(VART_UCHAR,    TF("UChar"))
		CASE_RET(VART_SHORT,    TF("Short"))
		CASE_RET(VART_USHORT,   TF("UShort"))
		CASE_RET(VART_INT,      TF("Int"))
		CASE_RET(VART_UINT,     TF("UInt"))
		CASE_RET(VART_LONG,     TF("Long"))
		CASE_RET(VART_ULONG,    TF("ULong"))
		CASE_RET(VART_LLONG,    TF("LLong"))
		CASE_RET(VART_ULLONG,   TF("ULLong"))
		CASE_RET(VART_FLOAT,    TF("Float"))
		CASE_RET(VART_DOUBLE,   TF("Double"))

		CASE_RET(VART_WCHAR,    TF("WChar"))
		CASE_RET(VART_XCHAR,    TF("XChar"))
		CASE_RET(VART_BYTE,     TF("Byte"))
		CASE_RET(VART_SIZE_T,   TF("size_t"))

		CASE_RET(VART_KEYREF,   TF("KeyRef"))
		CASE_RET(VART_UUID,     TF("UUID"))
		CASE_RET(VART_STRING,   TF("CString"))
		CASE_RET(VART_CLASS,    TF("Class"))

		CASE_RET(VART_ARRAY,    TF("Array"))
		CASE_RET(VART_LIST,     TF("List"))
		CASE_RET(VART_STACK,    TF("Stack"))
		CASE_RET(VART_QUEUE,    TF("Queue"))
		CASE_RET(VART_MAP,      TF("Map"))
		CASE_RET(VART_RBTREE,   TF("RBTree"))
		CASE_RET(VART_REFCOUNT, TF("RefCount"))
		CASE_RET(VART_OBJECT,   TF("CObject"))
		// ptr
		CASE_RET((VART_NONE|VART_PTR),     TF("void*"))
		CASE_RET((VART_CHAR|VART_PTR),     TF("PChar"))
		CASE_RET((VART_UCHAR|VART_PTR),    TF("PUChar"))
		CASE_RET((VART_SHORT|VART_PTR),    TF("PShort"))
		CASE_RET((VART_USHORT|VART_PTR),   TF("PUShort"))
		CASE_RET((VART_INT|VART_PTR),      TF("PInt"))
		CASE_RET((VART_UINT|VART_PTR),     TF("PUInt"))
		CASE_RET((VART_LONG|VART_PTR),     TF("PLong"))
		CASE_RET((VART_ULONG|VART_PTR),    TF("PULong"))
		CASE_RET((VART_LLONG|VART_PTR),    TF("PLLong"))
		CASE_RET((VART_ULLONG|VART_PTR),   TF("PULLong"))
		CASE_RET((VART_FLOAT|VART_PTR),    TF("PFloat"))
		CASE_RET((VART_DOUBLE|VART_PTR),   TF("PDouble"))

		CASE_RET((VART_WCHAR|VART_PTR),    TF("PWChar"))
		CASE_RET((VART_XCHAR|VART_PTR),    TF("PXChar"))
		CASE_RET((VART_BYTE|VART_PTR),     TF("PByte"))
		CASE_RET((VART_SIZE_T|VART_PTR),   TF("size_t*"))

		CASE_RET((VART_KEYREF|VART_PTR),   TF("KeyRef*"))
		CASE_RET((VART_UUID|VART_PTR),     TF("UUID*"))
		CASE_RET((VART_STRING|VART_PTR),   TF("CString*"))
		CASE_RET((VART_CLASS|VART_PTR),    TF("Class*"))

		CASE_RET((VART_ARRAY|VART_PTR),    TF("Array*"))
		CASE_RET((VART_LIST|VART_PTR),     TF("List*"))
		CASE_RET((VART_STACK|VART_PTR),    TF("Stack*"))
		CASE_RET((VART_QUEUE|VART_PTR),    TF("Queue*"))
		CASE_RET((VART_MAP|VART_PTR),      TF("Map*"))
		CASE_RET((VART_RBTREE|VART_PTR),   TF("RBTree*"))
		CASE_RET((VART_REFCOUNT|VART_PTR), TF("RefCount*"))
		CASE_RET((VART_OBJECT|VART_PTR),   TF("CObject*"))
		// data
		CASE_RET((VART_NONE|VART_DATA),     TF("void Data"))
		CASE_RET((VART_CHAR|VART_DATA),     TF("Char Data"))
		CASE_RET((VART_UCHAR|VART_DATA),    TF("UChar Data"))
		CASE_RET((VART_SHORT|VART_DATA),    TF("Short Data"))
		CASE_RET((VART_USHORT|VART_DATA),   TF("UShort Data"))
		CASE_RET((VART_INT|VART_DATA),      TF("Int Data"))
		CASE_RET((VART_UINT|VART_DATA),     TF("UInt Data"))
		CASE_RET((VART_LONG|VART_DATA),     TF("Long Data"))
		CASE_RET((VART_ULONG|VART_DATA),    TF("ULong Data"))
		CASE_RET((VART_LLONG|VART_DATA),    TF("LLong Data"))
		CASE_RET((VART_ULLONG|VART_DATA),   TF("ULLong Data"))
		CASE_RET((VART_FLOAT|VART_DATA),    TF("Float Data"))
		CASE_RET((VART_DOUBLE|VART_DATA),   TF("Double Data"))

		CASE_RET((VART_WCHAR|VART_DATA),    TF("WChar Data"))
		CASE_RET((VART_XCHAR|VART_DATA),    TF("XChar Data"))
		CASE_RET((VART_BYTE|VART_DATA),     TF("Byte Data"))
		CASE_RET((VART_SIZE_T|VART_DATA),   TF("size_t Data"))

		CASE_RET((VART_KEYREF|VART_DATA),   TF("KeyRef Data"))
		CASE_RET((VART_UUID|VART_DATA),     TF("UUID Data"))
		CASE_RET((VART_STRING|VART_DATA),   TF("CString Data"))
		CASE_RET((VART_CLASS|VART_DATA),    TF("Class Data"))

		CASE_RET((VART_ARRAY|VART_DATA),    TF("Array Data"))
		CASE_RET((VART_LIST|VART_DATA),     TF("List Data"))
		CASE_RET((VART_STACK|VART_DATA),    TF("Stack Data"))
		CASE_RET((VART_QUEUE|VART_DATA),    TF("Queue Data"))
		CASE_RET((VART_MAP|VART_DATA),      TF("Map Data"))
		CASE_RET((VART_RBTREE|VART_DATA),   TF("RBTree Data"))
		CASE_RET((VART_REFCOUNT|VART_DATA), TF("RefCount Data"))
		CASE_RET((VART_OBJECT|VART_DATA),   TF("CObject Data"))
		// ptr data
		CASE_RET((VART_NONE|VART_PTR|VART_DATA),     TF("void* Data"))
		CASE_RET((VART_CHAR|VART_PTR|VART_DATA),     TF("PChar Data"))
		CASE_RET((VART_UCHAR|VART_PTR|VART_DATA),    TF("PUChar Data"))
		CASE_RET((VART_SHORT|VART_PTR|VART_DATA),    TF("PShort Data"))
		CASE_RET((VART_USHORT|VART_PTR|VART_DATA),   TF("PUShort Data"))
		CASE_RET((VART_INT|VART_PTR|VART_DATA),      TF("PInt Data"))
		CASE_RET((VART_UINT|VART_PTR|VART_DATA),     TF("PUInt Data"))
		CASE_RET((VART_LONG|VART_PTR|VART_DATA),     TF("PLong Data"))
		CASE_RET((VART_ULONG|VART_PTR|VART_DATA),    TF("PULong Data"))
		CASE_RET((VART_LLONG|VART_PTR|VART_DATA),    TF("PLLong Data"))
		CASE_RET((VART_ULLONG|VART_PTR|VART_DATA),   TF("PULLong Data"))
		CASE_RET((VART_FLOAT|VART_PTR|VART_DATA),    TF("PFloat Data"))
		CASE_RET((VART_DOUBLE|VART_PTR|VART_DATA),   TF("PDouble Data"))

		CASE_RET((VART_WCHAR|VART_PTR|VART_DATA),    TF("PWChar Data"))
		CASE_RET((VART_XCHAR|VART_PTR|VART_DATA),    TF("PXChar Data"))
		CASE_RET((VART_BYTE|VART_PTR|VART_DATA),     TF("PByte Data"))
		CASE_RET((VART_SIZE_T|VART_PTR|VART_DATA),   TF("size_t* Data"))

		CASE_RET((VART_KEYREF|VART_PTR|VART_DATA),   TF("KeyRef* Data"))
		CASE_RET((VART_UUID|VART_PTR|VART_DATA),     TF("UUID* Data"))
		CASE_RET((VART_STRING|VART_PTR|VART_DATA),   TF("CString* Data"))
		CASE_RET((VART_CLASS|VART_PTR|VART_DATA),    TF("Class* Data"))

		CASE_RET((VART_ARRAY|VART_PTR|VART_DATA),    TF("Array* Data"))
		CASE_RET((VART_LIST|VART_PTR|VART_DATA),     TF("List* Data"))
		CASE_RET((VART_STACK|VART_PTR|VART_DATA),    TF("Stack* Data"))
		CASE_RET((VART_QUEUE|VART_PTR|VART_DATA),    TF("Queue* Data"))
		CASE_RET((VART_MAP|VART_PTR|VART_DATA),      TF("Map* Data"))
		CASE_RET((VART_RBTREE|VART_PTR|VART_DATA),   TF("RBTree* Data"))
		CASE_RET((VART_REFCOUNT|VART_PTR|VART_DATA), TF("RefCount* Data"))
		CASE_RET((VART_OBJECT|VART_PTR|VART_DATA),   TF("CObject* Data"))

		default:
		{ 
			return TF("Unknown Variant Type");
		}
	}
}

#endif // __DEF_INL__