///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : keyvalue.inl                                                                          //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : key-value                                                                                  //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __KEY_VALUE_INL__
#define __KEY_VALUE_INL__

#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CKeyValue
SELECTANY const CKeyValue::VALUE CKeyValue::NoneValue = { 0 };

INLINE CKeyValue::CKeyValue(bool bLoadDefault)
{
	if (bLoadDefault)
	{
		LoadDefault();
	}
}

INLINE CKeyValue::~CKeyValue(void)
{
	m_KeyValue.RemoveAll();
}

INLINE CKeyValue::CKeyValue(const CKeyValue&)
{
}

INLINE CKeyValue& CKeyValue::operator=(const CKeyValue&)
{
	return (*this);
}

INLINE Int CKeyValue::GetCount(void) const
{
	return m_KeyValue.GetSize();
}

INLINE size_t CKeyValue::GetLen(PCXStr pszKey)
{
	assert(pszKey != nullptr);
	const MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		return pPair->m_V.stLen;
	}
	return 0;
}

INLINE size_t CKeyValue::GetLen(size_t stKey)
{
	const MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		return pPair->m_V.stLen;
	}
	return 0;
}

INLINE void CKeyValue::SetLen(PCXStr pszKey, size_t stLen)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		pPair->m_V.stLen = stLen;
	}
}

INLINE void CKeyValue::SetLen(size_t stKey, size_t stLen)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		pPair->m_V.stLen = stLen;
	}
}

INLINE size_t CKeyValue::GetFlags(PCXStr pszKey)
{
	assert(pszKey != nullptr);
	const MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		return pPair->m_V.stFlags;
	}
	return 0;
}

INLINE size_t CKeyValue::GetFlags(size_t stKey)
{
	const MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		return pPair->m_V.stFlags;
	}
	return 0;
}

INLINE void CKeyValue::SetFlags(PCXStr pszKey, size_t stFlags)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		pPair->m_V.stFlags = stFlags;
	}
}

INLINE void CKeyValue::SetFlags(size_t stKey, size_t stFlags)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		pPair->m_V.stFlags = stFlags;
	}
}

INLINE bool CKeyValue::GetValue(PCXStr pszKey, CKeyValue::VALUE& Value)
{
	assert(pszKey != nullptr);
	const MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair == nullptr)
	{
		Value = NoneValue;
		return false;
	}
#ifdef RUNTIME_DEBUG
	else
	{
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
	}
#endif  // RUNTIME_DEBUG
	Value = pPair->m_V.xValue;
	return true;
}

INLINE bool CKeyValue::GetValue(size_t stKey, CKeyValue::VALUE& Value)
{
	const MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair == nullptr)
	{
		Value = NoneValue;
		return false;
	}
	Value = pPair->m_V.xValue;
	return true;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, PCXStr pszValue, size_t stLen)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_STRING) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_STRING;
		}
		if (pszValue != nullptr)
		{
			if (stLen == 0)
			{
				pPair->m_V.stLen = CXChar::Length(pszValue, (size_t)LMT_KEY);
			}
			else
			{
				pPair->m_V.stLen = DEF_BASE::Min<size_t>(stLen, (size_t) (LMT_KEY - 1));
			}
			CXChar::Copy(pPair->m_V.xValue.szValue, (size_t)LMT_KEY, pszValue);
		}
		else
		{
			pPair->m_V.stLen = 0;
			MM_SAFE::Memset(&(pPair->m_V.xValue), 0, sizeof(VALUE));
		}
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, PCXStr pszValue, size_t stLen)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_STRING) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_STRING;
		}
		if (pszValue != nullptr)
		{
			if (stLen == 0)
			{
				pPair->m_V.stLen = CXChar::Length(pszValue, (size_t)LMT_KEY);
			}
			else
			{
				pPair->m_V.stLen = DEF_BASE::Min<size_t>(stLen, (size_t) (LMT_KEY - 1));
			}
			CXChar::Copy(pPair->m_V.xValue.szValue, (size_t)LMT_KEY, pszValue);
		}
		else
		{
			pPair->m_V.stLen = 0;
			MM_SAFE::Memset(&(pPair->m_V.xValue), 0, sizeof(VALUE));
		}
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, void* pValue, size_t stLen)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_PTR) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_PTR;
		}
		pPair->m_V.stLen = stLen;
		pPair->m_V.xValue.pValue = pValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, void* pValue, size_t stLen)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_PTR) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_PTR;
		}
		pPair->m_V.stLen = stLen;
		pPair->m_V.xValue.pValue = pValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, LLong llValue)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_LLONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_LLONG;
		}
		pPair->m_V.xValue.llValue = llValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, LLong llValue)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_LLONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_LLONG;
		}
		pPair->m_V.xValue.llValue = llValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, ULLong ullValue)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_ULLONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_ULLONG;
		}
		pPair->m_V.xValue.ullValue = ullValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, ULLong ullValue)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_ULLONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_ULLONG;
		}
		pPair->m_V.xValue.ullValue = ullValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, Long lValue)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_LONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_LONG;
		}
		pPair->m_V.xValue.lValue = lValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, Long lValue)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_LONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_LONG;
		}
		pPair->m_V.xValue.lValue = lValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, ULong ulValue)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_ULONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_ULONG;
		}
		pPair->m_V.xValue.ulValue = ulValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, ULong ulValue)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_ULONG) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_ULONG;
		}
		pPair->m_V.xValue.ulValue = ulValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(PCXStr pszKey, Double dValue)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		assert((pPair->m_V.stFlags == (size_t) VART_DOUBLE) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_DOUBLE;
		}
		pPair->m_V.xValue.dValue = dValue;
		return true;
	}
	return false;
}

INLINE bool CKeyValue::SetValue(size_t stKey, Double dValue)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		assert((pPair->m_V.stFlags == (size_t) VART_DOUBLE) || (pPair->m_V.stFlags == (size_t) VART_NONE));
		if (pPair->m_V.stFlags == (size_t) VART_NONE)
		{
			pPair->m_V.stFlags = (size_t) VART_DOUBLE;
		}
		pPair->m_V.xValue.dValue = dValue;
		return true;
	}
	return false;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, PCXStr pszValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_STRING, (void*) pszValue);
	}
	else if (SetValue(stKey, pszValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, void* pValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_PTR, pValue);
	}
	else if (SetValue(stKey, pValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, LLong llValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_LLONG, &llValue);
	}
	else if (SetValue(stKey, llValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, ULLong ullValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_ULLONG, &ullValue);
	}
	else if (SetValue(stKey, ullValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, Long lValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_LONG, &lValue);
	}
	else if (SetValue(stKey, lValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, ULong ulValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_ULONG, &ulValue);
	}
	else if (SetValue(stKey, ulValue))
	{
		return stKey;
	}
	return 0;
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, Double dValue)
{
	size_t stKey = Find(pszKey);
	if (stKey == 0)
	{
		return AddItem(pszKey, (size_t) VART_DOUBLE, &dValue);
	}
	else if (SetValue(stKey, dValue))
	{
		return stKey;
	}
	return 0;
}

INLINE bool CKeyValue::Remove(PCXStr pszKey)
{
	assert(pszKey != nullptr);
	MAP_KEYVALUE_PAIR* pPair = Find(CHash::Hash(pszKey, (size_t)LMT_KEY));
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		pPair->m_V.Reset();
		return true;
	}
	return false;
}

INLINE bool CKeyValue::Remove(size_t stKey)
{
	MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
		pPair->m_V.Reset();
		return true;
	}
	return false;
}

INLINE size_t CKeyValue::Find(PCXStr pszKey)
{
	assert(pszKey != nullptr);
	size_t stKey = CHash::Hash(pszKey, (size_t)LMT_KEY);
	const MAP_KEYVALUE_PAIR* pPair = Find(stKey);
	if (pPair != nullptr)
	{
#ifdef RUNTIME_DEBUG
		assert(CXChar::Comparen(pszKey, pPair->m_V.szKey, (size_t)LMT_KEY) == 0);
#endif  // RUNTIME_DEBUG
		return stKey;
	}
	return 0;
}

INLINE void CKeyValue::LoadDefault(void)
{
#ifdef RUNTIME_DEBUG
	// Register all const key-values
	#define CONST_KEYVALUE
	#ifdef __STRING_CONCATENATION__
	#define CONST_KEYVALUE_STR( hash, key, str )      ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_STRING, str); 
	#define CONST_KEYVALUE_PTR( hash, key )           ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_PTR); 
	#define CONST_KEYVALUE_LLONG( hash, key, ll )     ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_LLONG,  &ll);
	#define CONST_KEYVALUE_ULLONG( hash, key, ull )   ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_ULLONG, &ull);
	#define CONST_KEYVALUE_LONG( hash, key, l )       ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_LONG,   &l);
	#define CONST_KEYVALUE_ULONG( hash, key, ul )     ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_ULONG,  &ul);
	#define CONST_KEYVALUE_DOUBLE( hash, key, d )     ConstKeyValue(hash, TF("CONST_KV_") TF(#key), (size_t)VART_DOUBLE, &d);
	#else
	#define CONST_KEYVALUE_STR( hash, key, str )      ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_STRING, str); 
	#define CONST_KEYVALUE_PTR( hash, key )           ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_PTR); 
	#define CONST_KEYVALUE_LLONG( hash, key, ll )     ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_LLONG,  &ll);
	#define CONST_KEYVALUE_ULLONG( hash, key, ull )   ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_ULLONG, &ull);
	#define CONST_KEYVALUE_LONG( hash, key, l )       ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_LONG,   &l);
	#define CONST_KEYVALUE_ULONG( hash, key, ul )     ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_ULONG,  &ul);
	#define CONST_KEYVALUE_DOUBLE( hash, key, d )     ConstKeyValue(hash, TF("CONST_KV_")##TF(#key), (size_t)VART_DOUBLE, &d);
	#endif
#else   // RUNTIME_DEBUG
	#define CONST_KEYVALUE
	#define CONST_KEYVALUE_STR( hash, key, str )      ConstKeyValue(hash, VART_STRING, str); 
	#define CONST_KEYVALUE_PTR( hash, key )           ConstKeyValue(hash, VART_PTR); 
	#define CONST_KEYVALUE_LLONG( hash, key, ll )     ConstKeyValue(hash, VART_LLONG,  &ll);
	#define CONST_KEYVALUE_ULLONG( hash, key, ull )   ConstKeyValue(hash, VART_ULLONG, &ull);
	#define CONST_KEYVALUE_LONG( hash, key, l )       ConstKeyValue(hash, VART_LONG,   &l);
	#define CONST_KEYVALUE_ULONG( hash, key, ul )     ConstKeyValue(hash, VART_ULONG,  &ul);
	#define CONST_KEYVALUE_DOUBLE( hash, key, d )     ConstKeyValue(hash, VART_DOUBLE, &d);
#endif  // RUNTIME_DEBUG
	#include "keyvaluedef.h"
	#undef  CONST_KEYVALUE_DOUBLE
	#undef  CONST_KEYVALUE_ULONG
	#undef  CONST_KEYVALUE_LONG
	#undef  CONST_KEYVALUE_ULLONG
	#undef  CONST_KEYVALUE_LLONG
	#undef  CONST_KEYVALUE_PTR
	#undef  CONST_KEYVALUE_STR
	#undef  CONST_KEYVALUE
}

INLINE CKeyValue::MAP_KEYVALUE_PAIR* CKeyValue::Find(size_t stKey)
{
	CSyncLockWaitScope scope(m_KVLock);
	return m_KeyValue.Find(stKey);
}

INLINE size_t CKeyValue::AddItem(PCXStr pszKey, size_t stFlags, void* pData)
{
	assert(pszKey != nullptr);
	assert(pData != nullptr);
	if (pszKey != nullptr)
	{
		CSyncLockWaitScope scope(m_KVLock);
#ifdef RUNTIME_DEBUG
		return ConstKeyValue(CHash::Hash(pszKey, (size_t)LMT_KEY), pszKey, stFlags, pData);
#else   // RUNTIME_DEBUG
		return ConstKeyValue(CHash::Hash(pszKey, (size_t)LMT_KEY), stFlags, pData);
#endif  // RUNTIME_DEBUG
	}
	return 0;
}

#ifdef RUNTIME_DEBUG
INLINE size_t CKeyValue::ConstKeyValue(size_t stKey, PCXStr pszKey, size_t stFlags, void* pData)
#else   // RUNTIME_DEBUG
INLINE size_t CKeyValue::ConstKeyValue(size_t stKey, size_t stFlags, void* pData)
#endif  // RUNTIME_DEBUG
{
	MAP_KEYVALUE_PAIR* pPair = m_KeyValue.Find(stKey);
	if (pPair == nullptr)
	{
		PINDEX index = m_KeyValue.Add(stKey);
		if (index != nullptr)
		{
			pPair = m_KeyValue.GetAt(index);
			pPair->m_V.stFlags = stFlags;
#ifdef RUNTIME_DEBUG
			CXChar::Copy(pPair->m_V.szKey, (size_t)LMT_KEY, pszKey);
			DEV_DEBUG(TF("  Key-Value[%p] Add key %s as hash=%p as type %s"), this, pszKey, (uintptr_t)stKey, DEF_BASE::StringVarType(stFlags));
#else   // RUNTIME_DEBUG
			DEV_DEBUG(TF("  Key-Value[%p] Add key hash=%p as type %s"), this, (uintptr_t)stKey, DEF_BASE::StringVarType(stFlags));
#endif  // RUNTIME_DEBUG
			if (pData != nullptr)
			{
				switch (stFlags)
				{
				case VART_STRING:
					{
						pPair->m_V.stLen = CXChar::Length(reinterpret_cast<PCXStr>(pData), (size_t)LMT_KEY);
						CXChar::Copy(pPair->m_V.xValue.szValue, (size_t)LMT_KEY, reinterpret_cast<PCXStr>(pData));
					}
					break;
				case VART_PTR:
					{
						pPair->m_V.xValue.pValue   = pData;
					}
					break;
				case VART_LLONG:
					{
						pPair->m_V.xValue.llValue  = *(reinterpret_cast<PLLong>(pData));
					}
					break;
				case VART_ULLONG:
					{
						pPair->m_V.xValue.ullValue = *(reinterpret_cast<PULLong>(pData));
					}
					break;
				case VART_LONG:
					{
						pPair->m_V.xValue.lValue   = *(reinterpret_cast<PLong>(pData));
					}
					break;
				case VART_ULONG:
					{
						pPair->m_V.xValue.ulValue  = *(reinterpret_cast<PULong>(pData));
					}
					break;
				case VART_DOUBLE:
					{
						pPair->m_V.xValue.dValue   = *(reinterpret_cast<PDouble>(pData));
					}
					break;
				default:
					{
					}
				}
			}
			return stKey;
		}
	}
#ifdef RUNTIME_DEBUG
	DEV_DEBUG(TF("  Key-Value[%p] : Find dunplicate key when add [%s]key"), this, pszKey);
#else   // RUNTIME_DEBUG
	DEV_DEBUG(TF("  Key-Value[%p] : Find dunplicate key when add [%p]key"), this, (uintptr_t)stKey);
#endif  // RUNTIME_DEBUG
	return 0;
}

#endif // __KEY_VALUE_INL__