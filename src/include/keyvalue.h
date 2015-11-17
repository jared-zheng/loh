///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : keyvalue.h                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : key-value                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __KEY_VALUE_H__
#define __KEY_VALUE_H__

#pragma once

#include "container.h"
#include "sync.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CKeyValue
class CKeyValue : public MObject
{
public:
	typedef union tagVALUE
	{
		XChar         szValue[LMT_KEY]; // value string is limited to 64 XChar
		void*         pValue;
		LLong         llValue;    
		ULLong        ullValue; 
		Long          lValue; 
		ULong         ulValue; 
		Double        dValue;
	}VALUE;
private:
	typedef struct tagKEYVALUE
	{  
	public:
		tagKEYVALUE(void)
		: stLen(0)
		, stFlags((size_t)VART_NONE)
		{
			MM_SAFE::Memset(this, 0, sizeof(tagKEYVALUE));
		}

		~tagKEYVALUE(void)
		{
		}

		void Reset(void)
		{
			stLen   = 0;
			stFlags = (size_t)VART_NONE;
			MM_SAFE::Memset(&xValue, 0, sizeof(VALUE));
		}
	public:
		size_t        stLen;            // length of szValue or pValue
		size_t        stFlags;          // string, integer or float
#ifdef RUNTIME_DEBUG
		XChar         szKey[LMT_KEY];   // debug verify only
#endif  // RUNTIME_DEBUG
		VALUE         xValue;
	} KEYVALUE, *PKEYVALUE;

	typedef CTMap<size_t, KEYVALUE>         MAP_KEYVALUE;
	typedef CTMap<size_t, KEYVALUE>::PAIR   MAP_KEYVALUE_PAIR;
public:
	CKeyValue(bool bLoadDefault = true);
	~CKeyValue(void);

	Int    GetCount(void) const;

	size_t GetLen(PCXStr pszKey); 
	size_t GetLen(size_t stKey); 
	void   SetLen(PCXStr pszKey, size_t stLen);
	void   SetLen(size_t stKey, size_t stLen);

	size_t GetFlags(PCXStr pszKey); 
	size_t GetFlags(size_t stKey); 
	void   SetFlags(PCXStr pszKey, size_t stFlags);
	void   SetFlags(size_t stKey, size_t stFlags);

	// get function, failed return NoneValue
	bool   GetValue(PCXStr pszKey, VALUE& Value);
	bool   GetValue(size_t stKey, VALUE& Value);

	// set function , key or index must exist
	bool   SetValue(PCXStr pszKey, PCXStr pszValue, size_t stLen = 0);
	bool   SetValue(size_t stKey, PCXStr pszValue, size_t stLen = 0);
	bool   SetValue(PCXStr pszKey, void* pValue, size_t stLen = 0);
	bool   SetValue(size_t stKey, void* pValue, size_t stLen = 0);
	bool   SetValue(PCXStr pszKey, LLong llValue);
	bool   SetValue(size_t stKey, LLong llValue);
	bool   SetValue(PCXStr pszKey, ULLong ullValue);
	bool   SetValue(size_t stKey, ULLong ullValue);
	bool   SetValue(PCXStr pszKey, Long lValue);
	bool   SetValue(size_t stKey, Long lValue);
	bool   SetValue(PCXStr pszKey, ULong ulValue);
	bool   SetValue(size_t stKey, ULong ulValue);
	bool   SetValue(PCXStr pszKey, Double dValue);
	bool   SetValue(size_t stKey, Double dValue);

	size_t AddItem(PCXStr pszKey, PCXStr pszValue);
	size_t AddItem(PCXStr pszKey, void* pValue);
	size_t AddItem(PCXStr pszKey, LLong llValue);
	size_t AddItem(PCXStr pszKey, ULLong ullValue);
	size_t AddItem(PCXStr pszKey, Long lValue);
	size_t AddItem(PCXStr pszKey, ULong ulValue);
	size_t AddItem(PCXStr pszKey, Double dValue);

	bool   Remove(PCXStr pszKey);
	bool   Remove(size_t stKey);

	size_t Find(PCXStr pszKey);
private:
	CKeyValue(const CKeyValue&);
	CKeyValue& operator=(const CKeyValue&);

	void   LoadDefault(void);

	MAP_KEYVALUE_PAIR* Find(size_t stKey);

	size_t AddItem(PCXStr pszKey, size_t stFlags, void* pData);
#ifdef RUNTIME_DEBUG
	size_t ConstKeyValue(size_t stKey, PCXStr pszKey, size_t stFlags, void* pData = nullptr);
#else   // RUNTIME_DEBUG
	size_t ConstKeyValue(size_t stKey, size_t stFlags, void* pData = nullptr);
#endif  // RUNTIME_DEBUG
private:
	static const VALUE NoneValue;
private:
	CSyncLock        m_KVLock; 
	MAP_KEYVALUE     m_KeyValue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "keyvalue.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __KEY_VALUE_H__