///////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CREEK engine                                                                                        //
//   Copyright(C) CREEK Studio. All Rights Reserved                                                      //
//   License :                                                                                           //
//   This software is provided 'as-is', without any express or implied warranty.                         //
//   In no event will the authors be held liable for any damages arising from the use of this software.  //
//   Permission is granted to anyone to use this software for any purpose,                               //
//   including commercial softwares, and to alter it and redistribute it freely.                         //
//                                                                                                       //
//   Header File : subsystem.h                                                                           //
//   Author : ruo.creek@outlook.com                                                                      //
//   Create : 2007-03-06     version 0.0.0.1                                                             //
//   Update : 2015-09-16     version 0.0.7.7                                                             //
//   Detail : subsystem load helper                                                                      //
//                                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __SUBSYSTEM_H__
#define __SUBSYSTEM_H__

#pragma once

#include "container.h"
#include "event.h"
#include "uuid.h"

//////////////////////////#
namespace CREEK          //
{                        //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// lib class loader
#define DECLARE_LIB_CLASS_LOADER( cls, name )                                  \
public:                                                                        \
	static CTRefCountPtr<CSubSystem>  ms_##name##LibPtr;
#define IMPLEMENT_LIB_CLASS_LOADER( cls, name )                                \
	SELECTANY CTRefCountPtr<CSubSystem> cls::ms_##name##LibPtr;
#define INIT_LIB_CLASS_LOADER( cls, name, uuid, method )                       \
	cls::ms_##name##LibPtr = CreateSubSystem((uuid))
#define EXIT_LIB_CLASS_LOADER( cls, name )                                     \
	if (cls::ms_##name##LibPtr != nullptr)                                     \
	{                                                                          \
		cls::ms_##name##LibPtr->Exit();                                        \
		cls::ms_##name##LibPtr = nullptr;                                      \
	}
#define LOADER_LIB_CLASS_CREEATE( cls, name, uuid, autoptr)                    \
	if ((cls::ms_##name##LibPtr != nullptr) &&                                 \
	    (cls::ms_##name##LibPtr->Init() == (UInt)RET_OKAY))                    \
	{                                                                          \
		cls::ms_##name##LibPtr->CreateComponent((uuid), (autoptr));            \
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// lib global loader
#define DECLARE_LIB_GLOBAL_LOADER( name )                                      \
	extern CTRefCountPtr<CSubSystem>  gs_##name##LibPtr;
#define IMPLEMENT_LIB_GLOBAL_LOADER( name )                                    \
	SELECTANY CTRefCountPtr<CSubSystem> gs_##name##LibPtr;
#define INIT_LIB_GLOBAL_LOADER( name, uuid, method )                           \
	gs_##name##LibPtr = CreateSubSystem((uuid))
#define EXIT_LIB_GLOBAL_LOADER( name )                                         \
	if (gs_##name##LibPtr != nullptr)                                          \
	{                                                                          \
		gs_##name##LibPtr->Exit();                                             \
		gs_##name##LibPtr = nullptr;                                           \
	}
#define LOADER_LIB_GLOBAL_CREEATE( name, uuid, autoptr)                        \
	if ((gs_##name##LibPtr != nullptr) &&                                      \
	    (gs_##name##LibPtr->Init() == (UInt)RET_OKAY))                         \
	{                                                                          \
		gs_##name##LibPtr->CreateComponent((uuid), (autoptr));                 \
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// dync class loader
#define DECLARE_DYNC_CLASS_LOADER( cls, name )                                 \
public:                                                                        \
	static CLoader  ms_##name##Loader;                                         \
	static CLoader& Get##name##Loader(void)                                    \
	{                                                                          \
		return ms_##name##Loader;                                              \
	}                                                               
#define IMPLEMENT_DYNC_CLASS_LOADER( cls, name )                               \
	SELECTANY CLoader cls::ms_##name##Loader;
#define INIT_DYNC_CLASS_LOADER( cls, name, uuid, path )                        \
	cls::Get##name##Loader().Load((uuid), (path))
#define EXIT_DYNC_CLASS_LOADER( cls, name )                                    \
	cls::Get##name##Loader().Unload()
#define LOADER_DYNC_CLASS_CREEATE( cls, name, uuid, autoptr)                   \
	if (cls::Get##name##Loader().IsInited())                                   \
	{                                                                          \
		cls::Get##name##Loader()->CreateComponent((uuid), (autoptr));          \
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
// dync global loader
#define DECLARE_DYNC_GLOBAL_LOADER( name )                                     \
	extern CLoader  gs_##name##Loader;                                         \
	static CLoader& Get##name##Loader(void)                                    \
	{                                                                          \
		return gs_##name##Loader;                                              \
	}                                                               
#define IMPLEMENT_DYNC_GLOBAL_LOADER( name )                                   \
	SELECTANY CLoader gs_##name##Loader;
#define INIT_DYNC_GLOBAL_LOADER( name, uuid, path )                            \
	Get##name##Loader().Load((uuid), (path))
#define EXIT_DYNC_GLOBAL_LOADER( name )                                        \
	Get##name##Loader().Unload()
#define LOADER_DYNC_GLOBAL_CREEATE( name, uuid, autoptr)                       \
	if (Get##name##Loader().IsInited())                                        \
	{                                                                          \
		Get##name##Loader()->CreateComponent((uuid), (autoptr));               \
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef RUNTIME_STATIC
	#define DECLARE_CLASS_LOADER      DECLARE_LIB_CLASS_LOADER
	#define IMPLEMENT_CLASS_LOADER    IMPLEMENT_LIB_CLASS_LOADER
	#define INIT_CLASS_LOADER         INIT_LIB_CLASS_LOADER
	#define EXIT_CLASS_LOADER         EXIT_LIB_CLASS_LOADER
	#define LOADER_CLASS_CREEATE      LOADER_LIB_CLASS_CREEATE

	#define DECLARE_GLOBAL_LOADER     DECLARE_LIB_GLOBAL_LOADER
	#define IMPLEMENT_GLOBAL_LOADER   IMPLEMENT_LIB_GLOBAL_LOADER
	#define INIT_GLOBAL_LOADER        INIT_LIB_GLOBAL_LOADER
	#define EXIT_GLOBAL_LOADER        EXIT_LIB_GLOBAL_LOADER
	#define LOADER_GLOBAL_CREEATE     LOADER_LIB_GLOBAL_CREEATE
#else
	#define DECLARE_CLASS_LOADER      DECLARE_DYNC_CLASS_LOADER
	#define IMPLEMENT_CLASS_LOADER    IMPLEMENT_DYNC_CLASS_LOADER
	#define INIT_CLASS_LOADER         INIT_DYNC_CLASS_LOADER
	#define EXIT_CLASS_LOADER         EXIT_DYNC_CLASS_LOADER
	#define LOADER_CLASS_CREEATE      LOADER_DYNC_CLASS_CREEATE

	#define DECLARE_GLOBAL_LOADER     DECLARE_DYNC_GLOBAL_LOADER
	#define IMPLEMENT_GLOBAL_LOADER   IMPLEMENT_DYNC_GLOBAL_LOADER
	#define INIT_GLOBAL_LOADER        INIT_DYNC_GLOBAL_LOADER
	#define EXIT_GLOBAL_LOADER        EXIT_DYNC_GLOBAL_LOADER
	#define LOADER_GLOBAL_CREEATE     LOADER_DYNC_GLOBAL_CREEATE
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CComponent
class NOVTABLE CComponent ABSTRACT : public CTRefCount<CComponent>
{
public:
	// execute command and time tick
	virtual UInt Command(PCXStr pszCMD, uintptr_t utParam) PURE_METHOD;
	virtual UInt Update(void) PURE_METHOD;
protected:
	CComponent(void);
	virtual ~CComponent(void);
private:
	CComponent(const CComponent&);
	CComponent& operator=(const CComponent&);
};
typedef CTRefCountPtr<CComponent> CComponentPtr;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSubSystem
class NOVTABLE CSubSystem ABSTRACT : public CComponent
{
public:
	static CPCStr CreateSubSystem;

	typedef CTMap<CUUID, CStringKey, CUUID::CUUIDTraits, CStringKey::CStringFixTraits>         MAP_COMPONENT_INFO;
	typedef CTMap<CUUID, CStringKey, CUUID::CUUIDTraits, CStringKey::CStringFixTraits>::PAIR   MAP_COMPONENT_INFO_PAIR;
public:
	virtual UInt  Init(void) PURE_METHOD;
	virtual void  Exit(void) PURE_METHOD;
	virtual Int   GetComponentInfo(MAP_COMPONENT_INFO& Components) PURE_METHOD;
	virtual bool  FindComponent(const CUUID& uuId) PURE_METHOD;
	virtual bool  CreateComponent(const CUUID& uuId, CComponentPtr& CComponentPtrRef) PURE_METHOD;
protected:
	CSubSystem(void);
	virtual ~CSubSystem(void);
private:
	CSubSystem(const CSubSystem&);
	CSubSystem& operator=(const CSubSystem&);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTLoader
typedef class CSubSystem* (CXXFUNC CreateSubSystemFunc)(const CUUID&);

template <typename T> 
class CTLoader : public MObject
{
public:
	explicit CTLoader(void);
	~CTLoader(void);

	T*     operator->(void) const;
	T*     GetSubSystem(void) const;
	LibRef GetLib(void) const;

	bool   IsInited(void) const;
	bool   IsLoaded(void) const;
	bool   Load(const CUUID& uuId, PCXStr pszLoadPath, bool bInit = true);
	void   Unload(void);

	PCXStr GetErrMsg(void) const;
private:
	CTLoader(const CTLoader&);
	CTLoader& operator=(const CTLoader&);
private:
	CModuleHandle  m_Module;
	T*             m_pSubSystem;
	bool           m_bInited;
	XChar          m_szErrMsg[LMT_MAX_PATH];
};
typedef CTLoader<CSubSystem> CLoader;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTLoaderRef
template <typename T> 
class CTLoaderRef : public MObject
{
public:
	explicit CTLoaderRef(LibRef libr);
	~CTLoaderRef(void);

	T*     operator->(void) const;
	T*     GetSubSystem(void) const;
	LibRef GetLib(void) const;

	bool   IsInited(void) const;
	bool   IsLoaded(void) const;
	bool   Load(const CUUID& uuId, bool bInit = true);
	void   Unload(void);

	PCXStr GetErrMsg(void) const;
private:
	CTLoaderRef(const CTLoaderRef&);
	CTLoaderRef& operator=(const CTLoaderRef&);
private:
	CModuleHandle  m_Module;
	T*             m_pSubSystem;
	bool           m_bInited;
	XChar          m_szErrMsg[LMT_MAX_PATH];
};
typedef CTLoaderRef<CSubSystem> CLoaderRef;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "subsystem.inl"

//////////////////////////#
} // namespace CREEK     //
//////////////////////////#

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// DECLARE
#ifdef RUNTIME_STATIC
	extern CREEK::CSubSystem* CreateSubSystem(const CREEK::CUUID&);
#endif

#endif // __SUBSYSTEM_H__