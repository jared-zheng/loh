///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : rtti.h                                                                                //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : runtime type information                                                                   //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RTTI_H__
#define __RTTI_H__

#pragma once

#include "stream.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// 
class CObject;

#ifdef RUNTIME_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// VAR_ITEM
typedef struct tagVAR_ITEM : public MObject
{
public:
	tagVAR_ITEM(void);
	~tagVAR_ITEM(void);
public:
	PCXStr        pszName;
	UInt          uType;
	UInt          uSize;
	const void*   pADDR;
}VAR_ITEM, *PVAR_ITEM;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS_ITEM
typedef struct tagCLASS_ITEM : public MObject
{
public:
	typedef CObject* (CXXFUNC CreateByRTTI)(void) ;
public:
	tagCLASS_ITEM(CPCXStr pszName, const tagCLASS_ITEM* pBase, const CreateByRTTI* pCreate, const UInt uType, const UInt uCount);
	~tagCLASS_ITEM(void);

	tagCLASS_ITEM(const tagCLASS_ITEM&);
	tagCLASS_ITEM& operator=(const tagCLASS_ITEM&);
public:
	CPCXStr                m_pszName;
	const tagCLASS_ITEM*   m_pBase;
	const CreateByRTTI*    m_pCreate;
	const UInt             m_uType;
	const UInt             m_uCount;
}CLASS_ITEM, *PCLASS_ITEM;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRTTI
class CORECLASS CRTTI : public CLASS_ITEM
{
public:
	static CRTTI*        Find(PCXStr pszName);
	static CRTTI*        Find(CStream& stream);

	static CObject*      CreateByName(PCXStr pszName);
	static CObject*      CreateByName(CStream& stream);
public:
	CRTTI(CPCXStr pszName, const CRTTI* pBase, const CreateByRTTI* pCreate, const UInt uType, const UInt uCount);
	~CRTTI(void);

	CPCXStr              GetName(void)   const;
	const  CRTTI*        GetBase(void)   const;
	const  CreateByRTTI* GetCreate(void) const;
	const  UInt          GetType(void)   const;
	const  UInt          GetCount(void)  const;

	const  bool          IsExactClass(const CRTTI& rtti) const;
	const  bool          IsKindOf(const CRTTI& rtti) const;
public:
	CObject*             Create(void);
	void                 Serialize(CStream& stream) const; // store only
private:
	CRTTI(const CRTTI&);
	CRTTI& operator=(const CRTTI&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class instance RTTI Dump
// +----------------------------------+
// |       Instance Name(String Ptr)  |   CLASS-INST
// +----------------------------------+
//
// +----------------------------------+
// |        Class Type(UInt)          |   
// +----------------------------------+
// |        Class Count(UInt) [N]     |
// +----------------------------------+
// |        Class Create(Ptr)         |
// +----------------------------------+
// |        Class Name(String Ptr)    |  CLASS-INFO
// +----------------------------------+
// |        This Type(UInt)           |   
// +----------------------------------+
// |        End  Flag(UInt)[0-end]    |
// +----------------------------------+
// |        This Ptr(pointer)         |
// +----------------------------------+
//
// +----------------------------------+
// |        Var Name(String Ptr)      |
// +----------------------------------+
// |        Var Type(UInt)            |   VAR-INFO
// +----------------------------------+
// |        Var Size(UInt)            |
// +----------------------------------+
// |        Var Ptr(pointer)          |
// +----------------------------------+
//
// class instance RTTI information
// INST + (CLASS-INFO + N * VAR-INFO)[0 <--> X]
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GHOST_ROOT_DUMP_RTTI( root )                                                                                                                       \
void root::Dump(CStream& stream) const                                                                                                                     \
{                                                                                                                                                          \
	stream << ms_RTTI.GetType() << ms_RTTI.GetCount() << (void*)ms_RTTI.GetCreate();                                                                       \
	stream << (void*)ms_RTTI.GetName() << (UInt)VART_OBJECT << (UInt)VART_ROOT << (void*)this;                                                             \
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define GHOST_DUMP_RTTI( dclass, bclass )                                                                                                                  \
void dclass::Dump(CStream& stream) const                                                                                                                   \
{                                                                                                                                                          \
	stream << ms_RTTI.GetType() << ms_RTTI.GetCount() << (void*)ms_RTTI.GetCreate();                                                                       \
	stream << (void*)ms_RTTI.GetName() << (UInt)VART_OBJECT << (UInt)VART_NONE << (void*)this;                                                             \
	bclass::Dump(stream);                                                                                                                                  \
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEGIN_ROOT_DUMP_RTTI( nclass, count )                                                                                                              \
void nclass::Dump(CStream& stream) const                                                                                                                   \
{                                                                                                                                                          \
	stream << ms_RTTI.GetType() << ms_RTTI.GetCount() << (void*)ms_RTTI.GetCreate();                                                                       \
	stream << (void*)ms_RTTI.GetName() << (UInt)VART_OBJECT << (UInt)VART_ROOT << (void*)this;                                                             \
	Int nIndex = 0;                                                                                                                                        \
	VAR_ITEM vi[count];
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define BEGIN_DUMP_RTTI( nclass, count )                                                                                                                   \
void nclass::Dump(CStream& stream) const                                                                                                                   \
{                                                                                                                                                          \
	stream << ms_RTTI.GetType() << ms_RTTI.GetCount() << (void*)ms_RTTI.GetCreate();                                                                       \
	stream << (void*)ms_RTTI.GetName() << (UInt)VART_OBJECT << (UInt)VART_NONE << (void*)this;                                                             \
	Int nIndex = 0;                                                                                                                                        \
	VAR_ITEM vi[count];
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DUMP_RTTI_STD( name, type, var )                                                                                                                   \
	vi[nIndex].pszName = TF(#name);                                                                                                                        \
	vi[nIndex].uType   = (UInt)type;                                                                                                                       \
	vi[nIndex].uSize   = sizeof(var);                                                                                                                      \
	vi[nIndex].pADDR   = &name;                                                                                                                            \
	++nIndex;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DUMP_RTTI_PTR( name, type, var )                                                                                                                   \
	vi[nIndex].pszName = TF(#name);                                                                                                                        \
	vi[nIndex].uType   = (UInt)(type|VART_PTR);                                                                                                            \
	vi[nIndex].uSize   = sizeof(var*);                                                                                                                     \
	vi[nIndex].pADDR   = &name;                                                                                                                            \
	++nIndex;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// var : data name(array size)
#ifdef __STRING_CONCATENATION__
#define DUMP_RTTI_DAT( name, type, var, size )                                                                                                             \
	vi[nIndex].pszName = TF(#name) TF("[") TF(#var) TF("]");                                                                                               \
	vi[nIndex].uType   = (UInt)(type|VART_DATA);                                                                                                           \
	vi[nIndex].uSize   = size;                                                                                                                             \
	vi[nIndex].pADDR   = &name;                                                                                                                            \
	++nIndex;
#else
#define DUMP_RTTI_DAT( name, type, var, size )                                                                                                             \
	vi[nIndex].pszName = TF(#name)##TF("[")##TF(#var)##TF("]");                                                                                            \
	vi[nIndex].uType   = (UInt)(type|VART_DATA);                                                                                                           \
	vi[nIndex].uSize   = size;                                                                                                                             \
	vi[nIndex].pADDR   = &name;                                                                                                                            \
	++nIndex;
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define END_ROOT_DUMP_RTTI( root, count )                                                                                                                  \
	stream.Write(&vi, sizeof(VAR_ITEM) * count);                                                                                                           \
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define END_DUMP_RTTI( dclass, bclass, count )                                                                                                             \
	stream.Write(&vi, sizeof(VAR_ITEM) * count);                                                                                                           \
	bclass::Dump(stream);                                                                                                                                  \
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class RTTI
#define DECLARE_RTTI( nclass )                                                                                                                             \
public:                                                                                                                                                    \
	static  const CRTTI  ms_RTTI;                                                                                                                          \
	virtual const CRTTI& GetRTTI(void) const                                                                                                               \
	{                                                                                                                                                      \
		return ms_RTTI;                                                                                                                                    \
	}                                                                                                                                                      \
	virtual void Dump(CStream&) const;
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI( root, type )                                                                                                                  \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, nullptr, (UInt)type, 0);                                                                       \
	GHOST_ROOT_DUMP_RTTI( root )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_BEGIN_DUMP( root, type, count )                                                                                                \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, nullptr, (UInt)type, (UInt)count);                                                             \
	BEGIN_ROOT_DUMP_RTTI( root, count )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI( dclass, bclass, type )                                                                                                             \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), nullptr, (UInt)type, 0);                                                        \
	GHOST_DUMP_RTTI( dclass, bclass )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_BEGIN_DUMP( dclass, bclass, type, count )                                                                                           \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), nullptr, (UInt)type, (UInt)count);                                              \
	BEGIN_DUMP_RTTI( dclass, count )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class RTTI with create
#define DECLARE_RTTI_CREATE( nclass )                                                                                                                      \
	DECLARE_RTTI( nclass )                                                                                                                                 \
	static CObject* CreateByRTTI(void);                                                                                           
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_CREATE( root, type )                                                                                                           \
	CObject* root::CreateByRTTI(void) { return (MNEW root); }                                                                                              \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, &(root::CreateByRTTI), (UInt)type, 0);                                                         \
	GHOST_ROOT_DUMP_RTTI( root )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_BEGIN_DUMP_CREATE( root, type, count )                                                                                         \
	CObject* root::CreateByRTTI(void) { return (MNEW root); }                                                                                              \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, &(root::CreateByRTTI), (UInt)type, (UInt)count);                                               \
	BEGIN_ROOT_DUMP_RTTI( root, count )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_CREATE( dclass, bclass, type )                                                                                                      \
	CObject* dclass::CreateByRTTI(void) { return (MNEW dclass); }                                                                                          \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), &(dclass::CreateByRTTI), (UInt)type, 0);                                        \
	GHOST_DUMP_RTTI( dclass, bclass )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_BEGIN_DUMP_CREATE( dclass, bclass, type, count )                                                                                    \
	CObject* dclass::CreateByRTTI(void) { return (MNEW dclass); }                                                                                          \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), &(dclass::CreateByRTTI), (UInt)type, (UInt)count);                              \
	BEGIN_DUMP_RTTI( dclass, count )

#else   // RUNTIME_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CRTTI
class CORECLASS CRTTI : public MObject
{
public:
	typedef CObject* (CXXFUNC CreateByRTTI)(void);
public:
	static CRTTI*         Find(PCXStr pszName);
	static CRTTI*         Find(CStream& stream);

	static CObject*       CreateByName(PCXStr pszName);
	static CObject*       CreateByName(CStream& stream);
public:
	CRTTI(CPCXStr pszName, const CRTTI* pBase, const CreateByRTTI* pCreate, const UInt uType, const UInt uCount);
	~CRTTI(void);

	CPCXStr               GetName(void)   const;
	const  CRTTI*         GetBase(void)   const;
	const  CreateByRTTI*  GetCreate(void) const;
	const  UInt           GetType(void)   const;
	const  UInt           GetCount(void)  const;

	const  bool           IsExactClass(const CRTTI& rtti) const;
	const  bool           IsKindOf(const CRTTI& rtti) const;
public:
	CObject*              Create(void);
	void                  Serialize(CStream& stream) const; // store only
private:
	CRTTI(const CRTTI&);
	CRTTI& operator=(const CRTTI&);
private:
	CPCXStr               m_pszName;
	const CRTTI*          m_pBase;
	const CreateByRTTI*   m_pCreate;
	const UInt            m_uType;
	const UInt            m_uCount;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class instance RTTI Dump
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DUMP_RTTI_STD( name, type, var )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DUMP_RTTI_PTR( name, type, var )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DUMP_RTTI_DAT( name, type, var, size )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define END_ROOT_DUMP_RTTI( nclass, bclass )
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define END_DUMP_RTTI( dclass, bclass, count )  

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class RTTI
#define DECLARE_RTTI( nclass )                                                                                                                             \
public:                                                                                                                                                    \
	static  const CRTTI  ms_RTTI;                                                                                                                          \
	virtual const CRTTI& GetRTTI(void) const                                                                                                               \
	{                                                                                                                                                      \
		return ms_RTTI;                                                                                                                                    \
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI( root, type )                                                                                                                  \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, nullptr, (UInt)type, 0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_BEGIN_DUMP( root, type, count )                                                                                                \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, nullptr, (UInt)type, 0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI( dclass, bclass, type )                                                                                                             \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), nullptr, (UInt)type, (UInt)(bclass::ms_RTTI.GetCount() + 1));
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_BEGIN_DUMP( dclass, bclass, type, count )                                                                                           \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), nullptr, (UInt)type, (UInt)(bclass::ms_RTTI.GetCount() + 1));
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class RTTI with create
#define DECLARE_RTTI_CREATE( nclass )                                                                                                                      \
	DECLARE_RTTI( nclass )                                                                                                                                 \
	static CObject* CreateByRTTI(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_CREATE( root, type )                                                                                                           \
	CObject* root::CreateByRTTI(void) { return (MNEW root); }                                                                                              \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, &(root::CreateByRTTI), (UInt)type, 0);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_ROOT_RTTI_BEGIN_DUMP_CREATE( root, type, count )                                                                                         \
	CObject* root::CreateByRTTI(void) { return (MNEW root); }                                                                                              \
	SELECTANY const CRTTI root::ms_RTTI(TF(#root), nullptr, &(root::CreateByRTTI), (UInt)type, (UInt)count);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_CREATE( dclass, bclass, type )                                                                                                      \
	CObject* dclass::CreateByRTTI(void) { return (MNEW dclass); }                                                                                          \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), &(dclass::CreateByRTTI), (UInt)type, (UInt)(bclass::ms_RTTI.GetCount() + 1));
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IMPLEMENT_RTTI_BEGIN_DUMP_CREATE( dclass, bclass, type, count )                                                                                    \
	CObject* dclass::CreateByRTTI(void) { return (MNEW dclass); }                                                                                          \
	SELECTANY const CRTTI dclass::ms_RTTI(TF(#dclass), &(bclass::ms_RTTI), &(dclass::CreateByRTTI), (UInt)type, (UInt)(bclass::ms_RTTI.GetCount() + 1));

#endif  // RUNTIME_DEBUG

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// RTTI_CAST
class RTTI_CAST
{
public:
	template <typename TCLASS, typename TBASE>
	static bool IsExactClass(TBASE* pObj);

	template <typename TCLASS, typename TBASE>
	static bool IsKindOf(TBASE* pObj);

	template <typename TCLASS, typename TBASE>
	static TCLASS* DynamicCast(TBASE* pObj);

	template <typename TCLASS>
	static bool Dump(TCLASS* pInst, PCXStr, CStream&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "rtti.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

#endif // __RTTI_H__