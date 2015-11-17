///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : rtti.inl                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : runtime type information                                                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RTTI_INL__
#define __RTTI_INL__

#pragma once

#ifdef RUNTIME_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// VAR_ITEM
INLINE tagVAR_ITEM::tagVAR_ITEM(void)
: pszName(nullptr)
, uType(0)
, uSize(0)
, pADDR(nullptr)
{
}

INLINE tagVAR_ITEM::~tagVAR_ITEM(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS_ITEM
INLINE tagCLASS_ITEM::tagCLASS_ITEM(CPCXStr pszName, const tagCLASS_ITEM* pBase, const CreateByRTTI* pCreate, const UInt uType, const UInt uCount)
: m_pszName(pszName)
, m_pBase(pBase)
, m_pCreate(pCreate)
, m_uType(uType)
, m_uCount(uCount)
{
}

INLINE tagCLASS_ITEM::~tagCLASS_ITEM(void)
{
}

INLINE tagCLASS_ITEM::tagCLASS_ITEM(const tagCLASS_ITEM&)
: m_pszName(nullptr)
, m_pBase(nullptr)
, m_pCreate(nullptr)
, m_uType(0)
, m_uCount(0)
{ 
}

INLINE tagCLASS_ITEM& tagCLASS_ITEM::operator=(const tagCLASS_ITEM&)
{ 
	return (*this); 
}

#else   // RUNTIME_DEBUG
#endif  // RUNTIME_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// RTTI_CAST
template <typename TCLASS, typename TBASE>
INLINE bool RTTI_CAST::IsExactClass(TBASE* pObj)
{
	return (pObj->GetRTTI().IsExactClass(TCLASS::ms_RTTI));
}

template <typename TCLASS, typename TBASE>
INLINE bool RTTI_CAST::IsKindOf(TBASE* pObj)
{
	return (pObj->GetRTTI().IsKindOf(TCLASS::ms_RTTI));
}

template <typename TCLASS, typename TBASE>
INLINE TCLASS* RTTI_CAST::DynamicCast(TBASE* pObj)
{
	return (pObj->GetRTTI().IsKindOf(TCLASS::ms_RTTI) ? static_cast<TCLASS*>(pObj) : nullptr);
}

#ifdef RUNTIME_DEBUG
template <typename TCLASS>
INLINE bool RTTI_CAST::Dump(TCLASS* pInst, PCXStr pszName, CStream& stream)
{
	stream << pszName;
	pInst->Dump(stream);
	return true;
}
#else   // RUNTIME_DEBUG
template <typename TCLASS>
INLINE bool RTTI_CAST::Dump(TCLASS*, PCXStr, CStream&)
{
	return false;
}
#endif  // RUNTIME_DEBUG

#endif // __RTTI_INL__