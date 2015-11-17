///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Inline File : object.inl                                                                            //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : object base class                                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OBJECT_INL__
#define __OBJECT_INL__

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CObjectTraits
template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::LoadObject(Int nLoadFlags, const CUUID& uuid)
{
	TOBJECT* pObj = MNEW TOBJECT;
	assert(pObj != nullptr);
	if (pObj != nullptr)
	{
		if (pObj->Link(nLoadFlags, uuid) == false)
		{
			MDELETE pObj;
			pObj = nullptr;
		}
	}
	return pObj;
}

template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::LoadObject(Int nLoadFlags, CString& str)
{
	return LoadObject<TOBJECT>(nLoadFlags, *str);
}

template <typename TOBJECT, size_t stLenT>
INLINE TOBJECT* CObjectTraits::LoadObject(Int nLoadFlags, CTStringFix<CXChar, stLenT>& strFix)
{
	return LoadObject<TOBJECT>(nLoadFlags, *strFix);
}

template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::LoadObject(Int nLoadFlags, PCXStr pszName)
{
	TOBJECT* pObj = nullptr;
	if (pszName != nullptr)
	{
		pObj = MNEW TOBJECT;
		assert(pObj != nullptr);
		if (pObj != nullptr)
		{
			if (pObj->Link(nLoadFlags, pszName) == false)
			{
				MDELETE pObj;
				pObj = nullptr;
			}
		}
	}
	return pObj;
}
// find
template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::FindObject(size_t stKey)
{
	return RTTI_CAST::DynamicCast<TOBJECT, CObject>(CObject::StaticFindObject(stKey));
}

template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::FindObject(const CUUID& uuid)
{
	size_t stKey = CUUID::CUUIDTraits::HashElements(uuid);
	return RTTI_CAST::DynamicCast<TOBJECT, CObject>(CObject::StaticFindObject(stKey));
}

template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::FindObject(CString& str)
{
	size_t stKey = CString::CStringTraits::HashElements(str);
	return RTTI_CAST::DynamicCast<TOBJECT, CObject>(CObject::StaticFindObject(stKey));
}

template <typename TOBJECT, size_t stLenT>
INLINE TOBJECT* CObjectTraits::FindObject(CTStringFix<CXChar, stLenT>& strFix)
{
	size_t stKey = CTStringFix<CXChar, tLen>::CStringFixTraits::HashElements(strFix);
	return RTTI_CAST::DynamicCast<TOBJECT, CObject>(CObject::StaticFindObject(stKey));
}

template <typename TOBJECT>
INLINE TOBJECT* CObjectTraits::FindObject(PCXStr pszName)
{
	size_t stKey = CHash::Hash(pszName);
	return RTTI_CAST::DynamicCast<TOBJECT, CObject>(CObject::StaticFindObject(stKey));
}

template <typename TOBJECT>
INLINE void CObjectTraits::UnloadObject(TOBJECT*& pObj)
{
	assert(pObj != nullptr);
	if (pObj != nullptr)
	{
		assert(pObj->GetKey() != 0);
		pObj->Unlink();

		MDELETE pObj;
		pObj = nullptr;
	}
}

#endif // __OBJECT_INL__