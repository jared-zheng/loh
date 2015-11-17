///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : object.h                                                                              //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : object base class                                                                          //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OBJECT_H__
#define __OBJECT_H__

#pragma once

#include "container.h"
#include "uuid.h"
#include "rtti.h"
#include "event.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CObjectTraits
class CObjectTraits
{
public:
	// Flags describing a class.
	enum OBJECT_TYPE
	{
		OBJECTT_NONE          = 0x00000000,
		OBJECTT_ABSTRACT      = 0x00000001,	// Class is abstract and can't be instantiated directly.
		OBJECTT_TRANSIENT     = 0x00000002,	// This object type can't be saved; null it out at save time.
		OBJECTT_LOCALIZED     = 0x00000004,	// Class contains localized text.
		OBJECTT_SAFEREPLACE   = 0x00000008,	// Objects of this class can be safely replaced with default or nullptr.
		OBJECTT_RUNTIMESTATIC = 0x00000010,	// Objects of this class are static during gameplay.
	};

	enum OBJECT_LOAD
	{
		OBJECTL_NONE          = 0x00000000,
		OBJECTL_NOFAIL        = 0x00000001,	// Critical error if load fails
		OBJECTL_NOWARN        = 0x00000002,	// Don't display warning if load fails
		OBJECTL_THROW         = 0x00000004,	// Throw exceptions upon failure
		OBJECTL_VERIFY        = 0x00000008,	// Only verify existance; don't actually load.
		OBJECTL_NOOUTPUT      = 0x00000010, // No warnings infomation output.
		OBJECTL_AUTOINIT      = 0x00000020, // 
		OBJECTL_AUTOEXIT      = 0x00000040,
		OBJECTL_UNIQUE        = 0x00000080,
	};
public:
	// load a new object instance, pszName should be unique, 
	// if the Name is alredy exist, return the exist object instance
	template <typename TOBJECT>
	static TOBJECT* LoadObject(Int nLoadFlags, const CUUID& uuid);

	template <typename TOBJECT>
	static TOBJECT* LoadObject(Int nLoadFlags, CString& str);

	template <typename TOBJECT, size_t stLenT>
	static TOBJECT* LoadObject(Int nLoadFlags, CTStringFix<CXChar, stLenT>& strFix);

	template <typename TOBJECT>
	static TOBJECT* LoadObject(Int nLoadFlags, PCXStr pszName);
	// find
	template <typename TOBJECT>
	static TOBJECT* FindObject(size_t stKey); 

	template <typename TOBJECT>
	static TOBJECT* FindObject(const CUUID& uuid);

	template <typename TOBJECT>
	static TOBJECT* FindObject(CString& str);

	template <typename TOBJECT, size_t stLenT>
	static TOBJECT* FindObject(CTStringFix<CXChar, stLenT>& strFix);

	template <typename TOBJECT>
	static TOBJECT* FindObject(PCXStr pszName);

	template <typename TOBJECT>
	static void     UnloadObject(TOBJECT*& pObj);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CObject
class CORECLASS NOVTABLE CObject ABSTRACT : public CEventHandler
{
	DECLARE_RTTI( CObject )
public:
	static  Int      StaticFindObject(CTArray<CObject*> objInsts, CRTTI& rtti, bool bExactClass = false);
	static  CObject* StaticFindObject(size_t stKey);
public:
	virtual UInt     Init(void);
	virtual void     Exit(void);

	virtual void     Serialize(CStream&);

	size_t  GetKey(void);
	Int     GetLoadFlags(void);

	bool    GetName(CString& strName);
	bool    GetName(CStringKey& strNameFix);
	bool    GetName(PXStr pszName, size_t stLen);
	bool    SetName(PCXStr pszNewName);

	bool    Link(Int nLoadFlags, const CUUID& uuid);
	bool    Link(Int nLoadFlags, PCXStr pszName);
	void    Unlink(void);
protected:
	CObject(void);
	virtual ~CObject(void);
	CObject(const CObject&);
	CObject& operator=(const CObject&);
private:
	size_t   m_stKey;
	PINDEX   m_Index;
	Int      m_nLoad;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "object.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __OBJECT_H__